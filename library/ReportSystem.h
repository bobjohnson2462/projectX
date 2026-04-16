#pragma once

#include <string>
#include <vector>

#include "LibrarySystem.h"

struct BookPopularityReportItem {
    int bookId;
    std::string title;
    std::string author;
    int issuedCount;
    int activeLoans;
};

struct DebtorReportItem {
    int readerId;
    std::string fullName;
    std::string contact;
    double totalFine;
};

struct TurnoverReport {
    int totalLoans;
    int activeLoans;
    int returnedLoans;
    int uniqueBooksIssued;
    int uniqueReadersServed;
    double totalCollectedFines;
};


class ReportSystem {
public:
    explicit ReportSystem(const LibrarySystem& library)
        : library_(library) {}


    std::vector<BookPopularityReportItem> getPopularBooks(std::size_t limit = 10) const;

 
    std::vector<DebtorReportItem> getDebtors(double minDebt = 0.01) const;

  
    TurnoverReport getTurnoverReport() const;

private:
    const LibrarySystem& library_;
};

