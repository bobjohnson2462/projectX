#include "FineSystem.h"

double FineSystem::getReaderDebt(int readerId) const {
    const Reader* r = library_.findReader(readerId);
    return r ? r->totalFine : 0.0;
}

std::vector<const Reader*> FineSystem::getDebtors(double minDebt) const {
    if (minDebt < 0.0) minDebt = 0.0;
    std::vector<const Reader*> out;
    out.reserve(library_.readers.size());
    for (const auto& r : library_.readers) {
        if (r.totalFine >= minDebt) {
            out.push_back(&r);
        }
    }
    return out;
}

double FineSystem::getTotalDebt() const {
    double sum = 0.0;
    for (const auto& r : library_.readers) {
        if (r.totalFine > 0.0) {
            sum += r.totalFine;
        }
    }
    return sum;
}

bool FineSystem::payDebt(int readerId, double amount, std::string* error) {
    if (amount <= 0.0) {
        if (error) *error = "Payment amount must be positive.";
        return false;
    }
    Reader* r = library_.findReader(readerId);
    if (!r) {
        if (error) *error = "Reader with this ID not found.";
        return false;
    }
    r->payFine(amount);
    return true;
}

