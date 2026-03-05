#pragma once

#include <vector>
#include <string>
#include <ctime>
#include "Book.h"

class BookCatalog;

class LibrarySystem {
public:
    std::vector<Book> books;
    BookCatalog* catalog;
    std::vector<Reader> readers;
    std::vector<Loan> loans;
    std::vector<Reservation> reservations;

    LibrarySystem();
    void attachCatalog(BookCatalog* c);
    void addBook(const Book& book);
    void addReader(const Reader& reader);

    Book* findBook(int bookId);
    const Book* findBook(int bookId) const;
    Reader* findReader(int readerId);
    const Reader* findReader(int readerId) const;
    Loan* findLoan(int loanId);
    const Loan* findLoan(int loanId) const;
    Reservation* getNextActiveReservationForBook(int bookId);
    bool hasActiveReservationForBookExceptReader(int bookId, int readerId) const;

    int registerReader(const std::string& fullName,
                       const std::string& contact,
                       const std::string& registrationDate);

    static constexpr int DEFAULT_LOAN_DAYS = 14;
    static constexpr double DAILY_FINE = 1.0;
    static constexpr double MAX_ALLOWED_FINE = 100.0;
    static constexpr double DAMAGED_BOOK_EXTRA_FINE = 50.0;

    int issueBook(int bookId, int readerId, const std::string& issueDate,
                  std::string* error = nullptr, int loanDays = DEFAULT_LOAN_DAYS);
    bool returnBook(int loanId, const std::string& returnDate, bool damaged,
                    std::string* error = nullptr);
    bool extendLoan(int loanId, int extraDays, std::string* error = nullptr);
    int reserveBook(int bookId, int readerId, const std::string& createdAt,
                   std::string* error = nullptr);

private:
    int generateNextReaderId() const;
    int generateNextLoanId() const;
    int generateNextReservationId() const;
    static bool parseDate(const std::string& s, std::tm& out);
    static bool addDaysToDate(const std::string& date, int daysToAdd, std::string& out);
    static bool calculateDaysDifference(const std::string& start, const std::string& end,
                                        int& outDays);
};
