// extension.h - B4.1: new bill type + new payer, WITHOUT touching billing.h
#pragma once
#include "billing.h"

class Maternity : public Bill {                          // new bill type
    double package; int days; double perDay;
public:
    Maternity(std::string p, double pkg, int d, double rate)
        : Bill(std::move(p)), package(nonNegative(pkg, "package")), days(d), perDay(nonNegative(rate, "room rate")) {}
    double total() const override { return package + days * perDay; }
    std::string type() const override { return "MT"; }
};

class CorporateTieUp : public Payer {                    // new payer
    double pct, cap;
public:
    CorporateTieUp(double percent, double capAmount) : pct(percent), cap(nonNegative(capAmount, "cap")) {}
    double amountCovered(const Bill& b) const override { return std::min(cap, b.total() * pct / 100.0); }
    std::string name() const override { return "Corporate"; }
};
