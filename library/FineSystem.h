#pragma once

#include <vector>
#include <string>

#include "LibrarySystem.h"


class FineSystem {
public:
    explicit FineSystem(LibrarySystem& library)
        : library_(library) {}

    
    double getReaderDebt(int readerId) const;

    
    std::vector<const Reader*> getDebtors(double minDebt = 0.01) const;

    double getTotalDebt() const;

    bool payDebt(int readerId, double amount, std::string* error = nullptr);

private:
    LibrarySystem& library_;
};

