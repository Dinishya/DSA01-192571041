// bench.cpp - B3 (virtual vs switch vs variant vs templates vs rules) and A2/A3 micro-costs
#include <algorithm>
#include <chrono>
#include <cstdint>
#include <cstdio>
#include <functional>
#include <memory>
#include <variant>
#include <vector>
using Clock = std::chrono::steady_clock;
static std::uint64_t rs = 88172645463325252ull;
static inline std::uint64_t rnd() { rs ^= rs << 13; rs ^= rs >> 7; rs ^= rs << 17; return rs; }
static volatile double sink;

// (a) virtual functions
struct VB { virtual ~VB() = default; virtual double total() const = 0; };
struct VOP : VB { double c, t; VOP(double a, double b) : c(a), t(b) {} double total() const override { return c + t; } };
struct VIP : VB { int d; double r, s; VIP(int a, double b, double c) : d(a), r(b), s(c) {} double total() const override { return d * r + s; } };
struct VDC : VB { double p; explicit VDC(double a) : p(a) {} double total() const override { return p; } };
struct VEM : VB { double b, h, r; VEM(double x, double y, double z) : b(x), h(y), r(z) {} double total() const override { return b + h * r; } };
// (b) one struct + type field + switch
struct SB { int kind; double a, b, c; };
static inline double stotal(const SB& s) {
    switch (s.kind) { case 0: return s.a + s.b; case 1: return s.a * s.b + s.c; case 2: return s.a; default: return s.a + s.b * s.c; }
}
// (c) std::variant / std::visit and (d) per-type containers (compile-time polymorphism)
struct COP { double c, t; double total() const { return c + t; } };
struct CIP { int d; double r, s; double total() const { return d * r + s; } };
struct CDC { double p; double total() const { return p; } };
struct CEM { double b, h, r; double total() const { return b + h * r; } };
using Var = std::variant<COP, CIP, CDC, CEM>;
// (e) data-driven rule: total = k0 + k1*x1 + k2*x2 + k3*x1*x2 looked up from a rule table (plug-in style)
struct Rule { double k0, k1, k2, k3; };
struct RB { int rule; double x1, x2; };

template <class F> static double timeIt(F f, int reps) {          // seconds for ONE pass (best of 3)
    double best = 1e30;
    for (int k = 0; k < 3; ++k) {
        auto t0 = Clock::now();
        for (int i = 0; i < reps; ++i) sink = f();
        double s = std::chrono::duration<double>(Clock::now() - t0).count() / reps;
        best = std::min(best, s);
    }
    return best;
}

int main()
{
    std::printf("sizeof: VB(vptr only)=%zu VOP=%zu VIP=%zu VDC=%zu VEM=%zu | SB=%zu | COP=%zu CIP=%zu CDC=%zu CEM=%zu | Variant=%zu | std::function=%zu | Rule=%zu RB=%zu\n",
        sizeof(VB), sizeof(VOP), sizeof(VIP), sizeof(VDC), sizeof(VEM), sizeof(SB), sizeof(COP), sizeof(CIP), sizeof(CDC), sizeof(CEM),
        sizeof(Var), sizeof(std::function<double(double)>), sizeof(Rule), sizeof(RB));

    // ---- B3.1 correctness on 10 bills ----
    {
        std::vector<std::unique_ptr<VB>> v; std::vector<SB> s; double va = 0, sw = 0, vr = 0;
        std::vector<Var> var;
        for (int i = 0; i < 10; ++i) {
            int k = i % 4; double a = 100 + 10 * i, b = 2 + i, c = 50 + 5 * i;
            switch (k) {
                case 0: v.emplace_back(new VOP(a, b)); var.emplace_back(COP{a, b}); break;
                case 1: v.emplace_back(new VIP(int(b), a, c)); var.emplace_back(CIP{int(b), a, c}); break;
                case 2: v.emplace_back(new VDC(a)); var.emplace_back(CDC{a}); break;
                default: v.emplace_back(new VEM(a, b, c)); var.emplace_back(CEM{a, b, c}); break;
            }
            s.push_back({k, k == 1 ? double(int(b)) : a, k == 1 ? a : b, c});
        }
        for (auto& p : v) va += p->total();
        for (auto& x : s) sw += stotal(x);
        for (auto& x : var) vr += std::visit([](const auto& t) { return t.total(); }, x);
        std::printf("B3.1 10 bills: virtual=%.1f switch=%.1f variant=%.1f agree=%s\n", va, sw, vr, (va == sw && sw == vr) ? "YES" : "NO");
    }

    // ---- B3.2 timing ----
    std::printf("B3.2 (ns per bill, -O2, best of 3; random type mix)\n");
    std::printf("%-12s %10s %10s %10s %10s %10s\n", "n", "virtual", "switch", "variant", "per-type", "rule-table");
    const Rule rules[4] = {{0, 1, 1, 0}, {0, 0, 0, 1}, {0, 1, 0, 0}, {1, 0, 0, 0.5}};
    for (std::size_t n : {std::size_t(1000), std::size_t(1000000), std::size_t(10000000)}) {
        std::vector<std::unique_ptr<VB>> v; v.reserve(n);
        std::vector<SB> s; s.reserve(n); std::vector<Var> var; var.reserve(n);
        std::vector<COP> t0; std::vector<CIP> t1; std::vector<CDC> t2; std::vector<CEM> t3;
        std::vector<RB> rb; rb.reserve(n);
        for (std::size_t i = 0; i < n; ++i) {
            int k = int(rnd() % 4); double a = double(rnd() % 1000), b = double(rnd() % 100), c = double(rnd() % 500);
            switch (k) {
                case 0: v.emplace_back(new VOP(a, b)); var.emplace_back(COP{a, b}); t0.push_back({a, b}); s.push_back({0, a, b, 0}); break;
                case 1: v.emplace_back(new VIP(int(b), a, c)); var.emplace_back(CIP{int(b), a, c}); t1.push_back({int(b), a, c}); s.push_back({1, double(int(b)), a, c}); break;
                case 2: v.emplace_back(new VDC(a)); var.emplace_back(CDC{a}); t2.push_back({a}); s.push_back({2, a, 0, 0}); break;
                default: v.emplace_back(new VEM(a, b, c)); var.emplace_back(CEM{a, b, c}); t3.push_back({a, b, c}); s.push_back({3, a, b, c}); break;
            }
            rb.push_back({k, a, b});
        }
        int reps = int(std::max<std::size_t>(3, 30000000 / n));
        double tv = timeIt([&] { double sum = 0; for (auto& p : v) sum += p->total(); return sum; }, reps);
        double ts = timeIt([&] { double sum = 0; for (auto& x : s) sum += stotal(x); return sum; }, reps);
        double tvar = timeIt([&] { double sum = 0; for (auto& x : var) sum += std::visit([](const auto& t) { return t.total(); }, x); return sum; }, reps);
        double tt = timeIt([&] { double sum = 0; for (auto& x : t0) sum += x.total(); for (auto& x : t1) sum += x.total();
                                 for (auto& x : t2) sum += x.total(); for (auto& x : t3) sum += x.total(); return sum; }, reps);
        double tr = timeIt([&] { double sum = 0; for (auto& x : rb) { const Rule& r = rules[x.rule]; sum += r.k0 + r.k1 * x.x1 + r.k2 * x.x2 + r.k3 * x.x1 * x.x2; } return sum; }, reps);
        std::printf("%-12zu %10.2f %10.2f %10.2f %10.2f %10.2f   (pass at n: virtual %.3f ms, switch %.3f ms)\n",
            n, tv / n * 1e9, ts / n * 1e9, tvar / n * 1e9, tt / n * 1e9, tr / n * 1e9, tv * 1e3, ts * 1e3);
    }

    // ---- A2 alternative: dynamic_cast chain vs one virtual call (1e6 bills, 4 types) ----
    {
        const std::size_t n = 1000000; std::vector<std::unique_ptr<VB>> v;
        for (std::size_t i = 0; i < n; ++i) {
            int k = int(rnd() % 4);
            if (k == 0) v.emplace_back(new VOP(1, 2)); else if (k == 1) v.emplace_back(new VIP(3, 4, 5));
            else if (k == 2) v.emplace_back(new VDC(6)); else v.emplace_back(new VEM(7, 8, 9));
        }
        double tv = timeIt([&] { double s = 0; for (auto& p : v) s += p->total(); return s; }, 20);
        double td = timeIt([&] {
            double s = 0;
            for (auto& p : v) {
                if (auto* a = dynamic_cast<VOP*>(p.get())) s += a->c + a->t;
                else if (auto* b = dynamic_cast<VIP*>(p.get())) s += b->d * b->r + b->s;
                else if (auto* c = dynamic_cast<VDC*>(p.get())) s += c->p;
                else if (auto* d = dynamic_cast<VEM*>(p.get())) s += d->b + d->h * d->r;
            }
            return s; }, 5);
        std::printf("A2-alt (1e6 bills): virtual call %.2f ns/bill ; dynamic_cast chain %.2f ns/bill ; ratio %.1fx\n", tv / n * 1e9, td / n * 1e9, td / tv);
    }

    // ---- A3 alternative: virtual Payer vs std::function / function pointer (1e7 calls) ----
    {
        struct P { virtual ~P() = default; virtual double cover(double t) const = 0; };
        struct Ins : P { double lim, cp; Ins(double l, double c) : lim(l), cp(c) {} double cover(double t) const override { return std::min(lim, t * (100 - cp) / 100); } };
        const std::size_t n = 10000000; std::vector<double> amt(n); for (auto& a : amt) a = double(rnd() % 100000);
        Ins ins(40000, 20); P* p = &ins;
        double lim = 40000, cp = 20;
        std::function<double(double)> fn = [lim, cp](double t) { return std::min(lim, t * (100 - cp) / 100); };
        double (*fp)(double) = [](double t) { return std::min(40000.0, t * 80 / 100); };
        volatile bool keep = true; (void)keep;
        double tv = timeIt([&] { double s = 0; for (double a : amt) s += p->cover(a); return s; }, 3);
        double tf = timeIt([&] { double s = 0; for (double a : amt) s += fn(a); return s; }, 3);
        double tp = timeIt([&] { double s = 0; for (double a : amt) s += fp(a); return s; }, 3);
        std::printf("A3-alt (1e7 calls): virtual %.2f ns ; std::function %.2f ns ; function pointer %.2f ns per call\n", tv / n * 1e9, tf / n * 1e9, tp / n * 1e9);
    }
}
