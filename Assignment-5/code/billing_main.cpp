// billing_main.cpp - B1.1 validation matrix, B2 error tests, B4.1 extension tests
#include <cstdlib>
#include <new>
#include "billing.h"
#include "extension.h"

#ifndef HUGE_BYTES   // an impossible allocation request
#define HUGE_BYTES (static_cast<std::size_t>(1) << 62)
#endif
// ASan replaces operator new and aborts on out-of-memory instead of throwing std::bad_alloc,
// so the bad_alloc test is compiled out of the sanitizer build (-DSKIP_BAD_ALLOC).

static int passed = 0, failed = 0;
static void check(const std::string& name, double got, double expected)
{
    bool ok = got == expected;
    (ok ? passed : failed)++;
    std::cout << (ok ? "  PASS " : "  FAIL ") << name << "  got=" << got << " expected=" << expected << '\n';
}

struct Tracer {                                          // prints in its destructor
    std::string n;
    explicit Tracer(std::string s) : n(std::move(s)) { std::cout << "  +" << n << '\n'; }
    ~Tracer() { std::cout << "  ~" << n << '\n'; }
};

static void core_tests()
{
    std::cout << "== B1.1 : 4 bill kinds x 3 payers (hand-computed expected values) ==\n";
    std::vector<std::unique_ptr<Bill>> bills;
    bills.emplace_back(new OutPatient("Ravi", 500, 1200));       // 1700
    bills.emplace_back(new InPatient("Meena", 3, 2500, 40000));  // 47500
    bills.emplace_back(new DayCare("Karthik", 30000));           // 30000
    bills.emplace_back(new Emergency("Anita", 2000, 3, 800));    // 4400
    SelfPay self; Insurance ins(40000, 20);
    GovernmentScheme gov({{"OP", 1000}, {"IP", 30000}, {"DC", 25000}, {"EM", 3000}});
    const double totals[4] = {1700, 47500, 30000, 4400};
    const double insExp[4] = {1360, 38000, 24000, 3520};
    const double govExp[4] = {1000, 30000, 25000, 3000};
    for (int i = 0; i < 4; ++i) {
        const Bill& b = *bills[i];
        check(b.type() + " total", b.total(), totals[i]);
        check(b.type() + " Self covered", self.amountCovered(b), 0);
        check(b.type() + " Insurance covered", ins.amountCovered(b), insExp[i]);
        check(b.type() + " Govt covered", gov.amountCovered(b), govExp[i]);
        check(b.type() + " patient pays (Insurance)", patientShare(b, ins), totals[i] - insExp[i]);
    }
    check("day total", dayTotal(bills), 83600);
    Insurance tight(30000, 20);                                  // limit binds: min(30000, 38000)
    check("IP Insurance when the cover limit binds", tight.amountCovered(*bills[1]), 30000);
}

static void error_tests()
{
    std::cout << "== B2 : deliberately engineered error conditions ==\n";
    try { OutPatient bad("X", -100, 50); }
    catch (const ChargeError& e) { std::cout << "  ChargeError: " << e.what() << '\n'; }

    try {
        InPatient ip("Meena", 3, 2500, 40000); Insurance ins(40000, 20);
        claim(ip, ins, 45000);
    } catch (const ClaimError& e) { std::cout << "  ClaimError: " << e.what() << '\n'; }

    try {
        Emergency em("Anita", 2000, 3, 800);
        GovernmentScheme noEM({{"OP", 1000}});
        noEM.amountCovered(em);
    } catch (const UnknownPackageError& e) { std::cout << "  UnknownPackageError: " << e.what() << '\n'; }

    try {
        PatientRegistry reg; reg.add("Ravi"); reg.require("Zoya");
    } catch (const BillingError& e) { std::cout << "  BillingError(base-class catch): " << e.what() << '\n'; }

    try { double v = std::stod("abc"); std::cout << v; }
    catch (const std::invalid_argument& e) { std::cout << "  std::invalid_argument from stod: " << e.what() << '\n'; }

#ifndef SKIP_BAD_ALLOC
    try {
        char* huge = new char[HUGE_BYTES];
        std::cout << "allocated?! " << static_cast<void*>(huge) << '\n';
        delete[] huge;
    } catch (const std::bad_alloc& e) { std::cout << "  std::bad_alloc: " << e.what() << '\n'; }
#endif
}

static void batch()                                     // two RAII objects, then a non-std exception
{
    Tracer a("invoice-file"), b("temp-buffer");
    throw 7;
}

static void extension_tests()
{
    std::cout << "== B4.1 : new bill type (Maternity) and new payer (CorporateTieUp) ==\n";
    Maternity m("Divya", 35000, 2, 3000);                        // 41000
    CorporateTieUp corp(90, 75000);
    check("MT total", m.total(), 41000);
    check("MT Corporate covered (90%, cap 75000)", corp.amountCovered(m), 36900);
    InPatient big("Meena", 3, 2500, 40000);
    check("IP Corporate covered", corp.amountCovered(big), 42750);
    Insurance ins(40000, 20);
    check("MT Insurance covered", ins.amountCovered(m), 32800);
    GovernmentScheme gov({{"OP", 1000}, {"MT", 30000}});
    check("MT Govt covered", gov.amountCovered(m), 30000);
    std::vector<std::unique_ptr<Bill>> v;                          // old and new bills in ONE list
    v.emplace_back(new OutPatient("Ravi", 500, 1200));
    v.emplace_back(new Maternity("Divya", 35000, 2, 3000));
    check("day total with new type", dayTotal(v), 42700);
}

int main()
{
    core_tests();
    error_tests();
    extension_tests();
    std::cout << "== B2 : top-level catch(...) keeps the program running ==\n";
    try { batch(); }
    catch (...) { std::cout << "  top-level handler: recovered, all objects destroyed\n"; }
    std::cout << "== program still running; passed=" << passed << " failed=" << failed << " ==\n";
    return failed == 0 ? 0 : 1;
}
