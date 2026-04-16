#include "ReportSystem.h"

#include <algorithm>
#include <unordered_map>
#include <unordered_set>

std::vector<BookPopularityReportItem> ReportSystem::getPopularBooks(std::size_t limit) const {
    std::unordered_map<int, int> issuesByBook;
    std::unordered_map<int, int> activeByBook;

    for (const auto& loan : library_.loans) {
        ++issuesByBook[loan.bookId];
        if (!loan.isReturned) {
            ++activeByBook[loan.bookId];
        }
    }

    std::vector<BookPopularityReportItem> out;
    out.reserve(issuesByBook.size());

    for (const auto& [bookId, count] : issuesByBook) {
        const Book* b = library_.findBook(bookId);
        BookPopularityReportItem item{};
        item.bookId = bookId;
        item.title = b ? b->title : "Unknown";
        item.author = b ? b->author : "Unknown";
        item.issuedCount = count;
        item.activeLoans = activeByBook[bookId];
        out.push_back(item);
    }

    std::sort(out.begin(), out.end(),
              [](const BookPopularityReportItem& a, const BookPopularityReportItem& b) {
                  if (a.issuedCount != b.issuedCount) return a.issuedCount > b.issuedCount;
                  return a.bookId < b.bookId;
              });

    if (out.size() > limit) {
        out.resize(limit);
    }
    return out;
}

std::vector<DebtorReportItem> ReportSystem::getDebtors(double minDebt) const {
    if (minDebt < 0.0) minDebt = 0.0;

    std::vector<DebtorReportItem> out;
    for (const auto& reader : library_.readers) {
        if (reader.totalFine >= minDebt) {
            DebtorReportItem item{};
            item.readerId = reader.id;
            item.fullName = reader.fullName;
            item.contact = reader.contact;
            item.totalFine = reader.totalFine;
            out.push_back(item);
        }
    }

    std::sort(out.begin(), out.end(),
              [](const DebtorReportItem& a, const DebtorReportItem& b) {
                  if (a.totalFine != b.totalFine) return a.totalFine > b.totalFine;
                  return a.readerId < b.readerId;
              });
    return out;
}

TurnoverReport ReportSystem::getTurnoverReport() const {
    TurnoverReport report{};
    report.totalLoans = static_cast<int>(library_.loans.size());
    report.activeLoans = 0;
    report.returnedLoans = 0;
    report.uniqueBooksIssued = 0;
    report.uniqueReadersServed = 0;
    report.totalCollectedFines = 0.0;

    std::unordered_set<int> booksIssued;
    std::unordered_set<int> readersServed;

    for (const auto& loan : library_.loans) {
        booksIssued.insert(loan.bookId);
        readersServed.insert(loan.readerId);

        if (loan.isReturned) {
            ++report.returnedLoans;
        } else {
            ++report.activeLoans;
        }
        if (loan.fine > 0.0) {
            report.totalCollectedFines += loan.fine;
        }
    }

    report.uniqueBooksIssued = static_cast<int>(booksIssued.size());
    report.uniqueReadersServed = static_cast<int>(readersServed.size());
    return report;
}

