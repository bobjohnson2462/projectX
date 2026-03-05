#include "LibrarySystem.h"
#include "BookCatalog.h"
#include <ctime>
#include <sstream>
#include <iomanip>

LibrarySystem::LibrarySystem()
    : books{}, catalog(nullptr), readers{}, loans{}, reservations{} {}

void LibrarySystem::attachCatalog(BookCatalog* c) { catalog = c; }
void LibrarySystem::addBook(const Book& book) { books.push_back(book); }
void LibrarySystem::addReader(const Reader& reader) { readers.push_back(reader); }

Book* LibrarySystem::findBook(int bookId) {
    for (auto& b : books) if (b.id == bookId) return &b;
    return nullptr;
}
const Book* LibrarySystem::findBook(int bookId) const {
    for (const auto& b : books) if (b.id == bookId) return &b;
    return nullptr;
}
Reader* LibrarySystem::findReader(int readerId) {
    for (auto& r : readers) if (r.id == readerId) return &r;
    return nullptr;
}
const Reader* LibrarySystem::findReader(int readerId) const {
    for (const auto& r : readers) if (r.id == readerId) return &r;
    return nullptr;
}
Loan* LibrarySystem::findLoan(int loanId) {
    for (auto& l : loans) if (l.id == loanId) return &l;
    return nullptr;
}
const Loan* LibrarySystem::findLoan(int loanId) const {
    for (const auto& l : loans) if (l.id == loanId) return &l;
    return nullptr;
}
Reservation* LibrarySystem::getNextActiveReservationForBook(int bookId) {
    for (auto& r : reservations)
        if (r.bookId == bookId && r.isActive) return &r;
    return nullptr;
}
bool LibrarySystem::hasActiveReservationForBookExceptReader(int bookId, int readerId) const {
    for (const auto& r : reservations)
        if (r.bookId == bookId && r.isActive && r.readerId != readerId) return true;
    return false;
}

int LibrarySystem::registerReader(const std::string& fullName,
        const std::string& contact, const std::string& registrationDate) {
    int id = generateNextReaderId();
    readers.emplace_back(id, fullName, contact, registrationDate);
    return id;
}

int LibrarySystem::issueBook(int bookId, int readerId, const std::string& issueDate,
        std::string* error, int loanDays) {
    if (loanDays <= 0) { if (error) *error = "Loan period must be positive."; return -1; }
    Book* book = findBook(bookId);
    if (!book) { if (error) *error = "Book with this ID not found."; return -1; }
    Reader* reader = findReader(readerId);
    if (!reader) { if (error) *error = "Reader with this ID not found."; return -1; }
    if (!reader->canBorrow(MAX_ALLOWED_FINE)) {
        if (error) *error = "Cannot issue book: reader's debt exceeds allowed limit.";
        return -1;
    }
    if (!book->hasAvailableCopy()) {
        if (error) *error = "No available copies of this book."; return -1;
    }
    if (hasActiveReservationForBookExceptReader(bookId, readerId)) {
        if (error) *error = "Book is reserved by another reader."; return -1;
    }
    std::string dueDate;
    if (!addDaysToDate(issueDate, loanDays, dueDate)) {
        if (error) *error = "Invalid issue date format (expected YYYY-MM-DD)."; return -1;
    }
    book->takeCopy();
    int loanId = generateNextLoanId();
    loans.emplace_back(loanId, bookId, readerId, issueDate, dueDate);
    if (Reservation* res = getNextActiveReservationForBook(bookId))
        if (res->readerId == readerId) res->complete();
    return loanId;
}

bool LibrarySystem::returnBook(int loanId, const std::string& returnDate,
        bool damaged, std::string* error) {
    Loan* loan = findLoan(loanId);
    if (!loan) { if (error) *error = "Loan with this ID not found."; return false; }
    if (loan->isReturned) {
        if (error) *error = "Book has already been returned."; return false;
    }
    Book* book = findBook(loan->bookId);
    if (!book) { if (error) *error = "Book linked to this loan not found."; return false; }
    int overdueDays = 0;
    if (!calculateDaysDifference(loan->dueDate, returnDate, overdueDays)) {
        if (error) *error = "Invalid return date format (expected YYYY-MM-DD)."; return false;
    }
    if (overdueDays < 0) overdueDays = 0;
    double fine = 0.0;
    if (overdueDays > 0 && DAILY_FINE > 0.0) fine += DAILY_FINE * static_cast<double>(overdueDays);
    if (damaged && DAMAGED_BOOK_EXTRA_FINE > 0.0) fine += DAMAGED_BOOK_EXTRA_FINE;
    if (fine < 0.0) fine = 0.0;
    if (fine > 0.0) {
        Reader* r = findReader(loan->readerId);
        if (r) r->addFine(fine);
        loan->setFine(fine);
    }
    book->returnCopy();
    loan->markReturned(returnDate);
    return true;
}

bool LibrarySystem::extendLoan(int loanId, int extraDays, std::string* error) {
    if (extraDays <= 0) {
        if (error) *error = "Extension period must be positive."; return false;
    }
    Loan* loan = findLoan(loanId);
    if (!loan) { if (error) *error = "Loan with this ID not found."; return false; }
    if (loan->isReturned) {
        if (error) *error = "Cannot extend a finished loan."; return false;
    }
    if (hasActiveReservationForBookExceptReader(loan->bookId, loan->readerId)) {
        if (error) *error = "Cannot extend: book has an active reservation."; return false;
    }
    std::string newDueDate;
    if (!addDaysToDate(loan->dueDate, extraDays, newDueDate)) {
        if (error) *error = "Invalid date format (expected YYYY-MM-DD)."; return false;
    }
    loan->dueDate = newDueDate;
    return true;
}

int LibrarySystem::reserveBook(int bookId, int readerId, const std::string& createdAt,
        std::string* error) {
    Book* book = findBook(bookId);
    if (!book) { if (error) *error = "Book with this ID not found."; return -1; }
    Reader* reader = findReader(readerId);
    if (!reader) { if (error) *error = "Reader with this ID not found."; return -1; }
    if (book->hasAvailableCopy()) {
        if (error) *error = "Cannot reserve a book that is currently available."; return -1;
    }
    for (const auto& r : reservations)
        if (r.bookId == bookId && r.readerId == readerId && r.isActive) {
            if (error) *error = "Reader already has an active reservation for this book.";
            return -1;
        }
    int id = generateNextReservationId();
    reservations.emplace_back(id, bookId, readerId, createdAt);
    return id;
}

int LibrarySystem::generateNextReaderId() const {
    int maxId = 0;
    for (const auto& r : readers) if (r.id > maxId) maxId = r.id;
    return maxId + 1;
}
int LibrarySystem::generateNextLoanId() const {
    int maxId = 0;
    for (const auto& l : loans) if (l.id > maxId) maxId = l.id;
    return maxId + 1;
}
int LibrarySystem::generateNextReservationId() const {
    int maxId = 0;
    for (const auto& r : reservations) if (r.id > maxId) maxId = r.id;
    return maxId + 1;
}

bool LibrarySystem::parseDate(const std::string& s, std::tm& out) {
    std::istringstream iss(s);
    iss >> std::get_time(&out, "%Y-%m-%d");
    if (iss.fail()) return false;
    out.tm_isdst = -1;
    return true;
}

bool LibrarySystem::addDaysToDate(const std::string& date, int daysToAdd, std::string& out) {
    if (daysToAdd < 0) return false;
    std::tm tm{};
    if (!parseDate(date, tm)) return false;
    std::time_t t = std::mktime(&tm);
    if (t == -1) return false;
    t += static_cast<long>(daysToAdd) * 24L * 60L * 60L;
    std::tm* newTm = std::localtime(&t);
    if (!newTm) return false;
    std::ostringstream oss;
    oss << std::put_time(newTm, "%Y-%m-%d");
    out = oss.str();
    return true;
}

bool LibrarySystem::calculateDaysDifference(const std::string& start,
        const std::string& end, int& outDays) {
    std::tm tmStart{}, tmEnd{};
    if (!parseDate(start, tmStart)) return false;
    if (!parseDate(end, tmEnd)) return false;
    std::time_t tStart = std::mktime(&tmStart);
    std::time_t tEnd = std::mktime(&tmEnd);
    if (tStart == -1 || tEnd == -1) return false;
    outDays = static_cast<int>(std::difftime(tEnd, tStart) / (24 * 60 * 60));
    return true;
}
