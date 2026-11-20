// billing.h - Hospital billing: polymorphic bills, payers and exception hierarchy
#pragma once
#include <algorithm>
#include <iostream>
#include <map>
#include <memory>
#include <set>
#include <stdexcept>
#include <string>
#include <vector>

// ---------------- exception hierarchy (A4.1) ----------------
class BillingError : public std::runtime_error {
public: using std::runtime_error::runtime_error;
};
class ChargeError : public BillingError {              // negative / invalid charge
public: using BillingError::BillingError;
};
class ClaimError : public BillingError {               // claim above cover
public: using BillingError::BillingError;
};
class UnknownPackageError : public BillingError {      // no package rate for bill type
public: using BillingError::BillingError;
};
class PatientRecordError : public BillingError {       // patient not registered
public: using BillingError::BillingError;
};

inline double nonNegative(double v, const char* what)
{
    if (v < 0) throw ChargeError(std::string("negative ") + what);
    return v;
}

// ---------------- abstract Bill (A1) ----------------
class Bill {
protected:
    std::string patient;
public:
    explicit Bill(std::string p) : patient(std::move(p)) {}
    virtual ~Bill() = default;                          // virtual destructor (A2.3)
    virtual double total() const = 0;                   // pure virtual
    virtual std::string type() const = 0;               // pure virtual
    const std::string& who() const noexcept { return patient; }
    void print() const                                  // non-virtual template method
    {
        std::cout << type() << ' ' << patient << ": " << total() << '\n';
    }
};

class OutPatient : public Bill {
    double consultation, tests;
public:
    OutPatient(std::string p, double c, double t)
        : Bill(std::move(p)), consultation(nonNegative(c, "consultation")), tests(nonNegative(t, "tests")) {}
    double total() const override { return consultation + tests; }
    std::string type() const override { return "OP"; }
};
class InPatient : public Bill {
    int days; double perDay, surgery;
public:
    InPatient(std::string p, int d, double rate, double s)
        : Bill(std::move(p)), days(d), perDay(nonNegative(rate, "room rate")), surgery(nonNegative(s, "procedure"))
    { if (d < 0) throw ChargeError("negative days"); }
    double total() const override { return days * perDay + surgery; }
    std::string type() const override { return "IP"; }
};
class DayCare : public Bill {
    double package;
public:
    DayCare(std::string p, double pkg) : Bill(std::move(p)), package(nonNegative(pkg, "package")) {}
    double total() const override { return package; }
    std::string type() const override { return "DC"; }
};
class Emergency : public Bill {
    double base, hours, hourly;
public:
    Emergency(std::string p, double b, double h, double r)
        : Bill(std::move(p)), base(nonNegative(b, "base")), hours(nonNegative(h, "hours")), hourly(nonNegative(r, "hourly rate")) {}
    double total() const override { return base + hours * hourly; }
    std::string type() const override { return "EM"; }
};

// ---------------- abstract Payer (A3) ----------------
class Payer {
public:
    virtual ~Payer() = default;
    virtual double amountCovered(const Bill&) const = 0;
    virtual std::string name() const = 0;
};
class SelfPay : public Payer {
public:
    double amountCovered(const Bill&) const override { return 0; }
    std::string name() const override { return "Self"; }
};
class Insurance : public Payer {
    double limit, copayPct;
public:
    Insurance(double lim, double copay) : limit(nonNegative(lim, "cover limit")), copayPct(copay)
    { if (copay < 0 || copay > 100) throw ChargeError("co-payment must be 0..100"); }
    double amountCovered(const Bill& b) const override
    { return std::min(limit, b.total() * (100.0 - copayPct) / 100.0); }
    std::string name() const override { return "Insurance"; }
};
class GovernmentScheme : public Payer {
    std::map<std::string, double> rate;                 // fixed package rate per bill type
public:
    explicit GovernmentScheme(std::map<std::string, double> r) : rate(std::move(r)) {}
    double amountCovered(const Bill& b) const override
    {
        auto it = rate.find(b.type());
        if (it == rate.end()) throw UnknownPackageError("no package rate for " + b.type());
        return std::min(b.total(), it->second);
    }
    std::string name() const override { return "Govt"; }
};

// ---------------- services ----------------
class PatientRegistry {
    std::set<std::string> known;
public:
    void add(const std::string& n) { known.insert(n); }
    void require(const std::string& n) const
    { if (!known.count(n)) throw PatientRecordError("missing patient record: " + n); }
};

inline double patientShare(const Bill& b, const Payer& p) { return b.total() - p.amountCovered(b); }

inline double claim(const Bill& b, const Payer& p, double requested)
{
    if (requested < 0) throw ChargeError("negative claim");
    if (requested > p.amountCovered(b)) throw ClaimError("claim exceeds cover");
    return requested;
}

inline double dayTotal(const std::vector<std::unique_ptr<Bill>>& bills)
{
    double sum = 0;
    for (const auto& b : bills) sum += b->total();      // runtime polymorphism
    return sum;
}
