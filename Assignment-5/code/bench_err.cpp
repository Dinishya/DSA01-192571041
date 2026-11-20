// bench_err.cpp - B3.3 / B2-alt: return codes vs exceptions vs std::optional, 1,000,000 bills
#include <algorithm>
#include <chrono>
#include <cstdint>
#include <cstdio>
#include <optional>
#include <stdexcept>
#include <vector>
using Clock = std::chrono::steady_clock;
static std::uint64_t rs = 88172645463325252ull;
static inline std::uint64_t rnd() { rs ^= rs << 13; rs ^= rs >> 7; rs ^= rs << 17; return rs; }

enum class Status { Ok, Negative };
__attribute__((noinline)) Status validate_rc(double amount, double& out) { if (amount < 0) return Status::Negative; out = amount * 1.05; return Status::Ok; }
__attribute__((noinline)) double validate_ex(double amount) { if (amount < 0) throw std::invalid_argument("negative"); return amount * 1.05; }
__attribute__((noinline)) std::optional<double> validate_opt(double amount) { if (amount < 0) return std::nullopt; return amount * 1.05; }

int main()
{
    const std::size_t n = 1000000;
    std::printf("sizeof(std::optional<double>)=%zu sizeof(double)=%zu sizeof(Status)=%zu\n", sizeof(std::optional<double>), sizeof(double), sizeof(Status));
    std::printf("%-8s %14s %14s %14s %10s\n", "invalid", "return code ms", "optional ms", "exception ms", "exc/rc");
    for (int pct : {0, 1, 50}) {
        std::vector<double> amt(n);
        std::size_t expectBad = 0;
        for (auto& a : amt) { bool bad = (rnd() % 100) < std::uint64_t(pct); a = bad ? -1.0 : double(rnd() % 100000); expectBad += bad; }
        auto best = [&](auto f, int tries) { double b = 1e30; std::size_t bad = 0; for (int k = 0; k < tries; ++k) { auto t0 = Clock::now(); bad = f(); b = std::min(b, std::chrono::duration<double>(Clock::now() - t0).count()); } return std::pair<double, std::size_t>(b, bad); };
        volatile double sink = 0;
        auto rc = best([&] { std::size_t bad = 0; double s = 0; for (double a : amt) { double o = 0; if (validate_rc(a, o) != Status::Ok) ++bad; else s += o; } sink = s; return bad; }, 5);
        auto op = best([&] { std::size_t bad = 0; double s = 0; for (double a : amt) { auto r = validate_opt(a); if (!r) ++bad; else s += *r; } sink = s; return bad; }, 5);
        auto ex = best([&] { std::size_t bad = 0; double s = 0; for (double a : amt) { try { s += validate_ex(a); } catch (const std::invalid_argument&) { ++bad; } } sink = s; return bad; }, 3);
        std::printf("%-8d %14.3f %14.3f %14.3f %9.1fx   (invalid counted: rc=%zu opt=%zu exc=%zu expected=%zu)\n",
            pct, rc.first * 1e3, op.first * 1e3, ex.first * 1e3, ex.first / rc.first, rc.second, op.second, ex.second, expectBad);
    }
    // cost of ONE throw/catch, deeper stack (unwinding through 10 frames)
    {
        struct F { static __attribute__((noinline)) double deep(int d, double a) { if (d == 0) return validate_ex(a); return deep(d - 1, a) + 1; } };
        const int N = 100000; volatile double sink = 0; std::size_t bad = 0;
        auto t0 = Clock::now();
        for (int i = 0; i < N; ++i) { try { sink = F::deep(10, -1); } catch (const std::invalid_argument&) { ++bad; } }
        double us = std::chrono::duration<double>(Clock::now() - t0).count() / N * 1e6;
        std::printf("one throw+catch through 10 frames: %.2f microseconds (caught %zu)\n", us, bad);
    }
}
