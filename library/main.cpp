#include <iostream>
#include <string>
#include <vector>
#include <limits>
#include <fstream>
#include <sstream>

#include "Book.h"
#include "BookCatalog.h"
#include "LibrarySystem.h"
#include "ReservationSystem.h"
#include "FineSystem.h"

std::string genreToString(Genre g) {
    switch (g) {
    case Genre::Fiction:   return "Fiction";
    case Genre::Science:   return "Science";
    case Genre::Fantasy:   return "Fantasy";
    case Genre::Detective: return "Detective";
    default:               return "Unknown";
    }
}

Genre genreFromString(const std::string& s) {
    if (s == "Fiction") return Genre::Fiction;
    if (s == "Science") return Genre::Science;
    if (s == "Fantasy") return Genre::Fantasy;
    if (s == "Detective") return Genre::Detective;
    return Genre::Fiction;
}

bool loadBooksFromCsv(const std::string& path, std::vector<Book>& books) {
    std::ifstream in(path);
    if (!in.is_open()) {
        return false;
    }

    books.clear();

    std::string line;
    if (!std::getline(in, line)) {
        return true; // empty file
    }

    // Expected columns:
    // book_id,title,author,year,genre,price,total_copies,available_copies
    while (std::getline(in, line)) {
        if (line.empty()) continue;

        std::stringstream ss(line);
        std::vector<std::string> cols;
        std::string cell;
        while (std::getline(ss, cell, ',')) {
            cols.push_back(cell);
        }

        if (cols.size() < 8) {
            continue; // skip broken line
        }

        try {
            const int id = std::stoi(cols[0]);
            const std::string title = cols[1];
            const std::string author = cols[2];
            const int year = std::stoi(cols[3]);
            const Genre genre = genreFromString(cols[4]);
            const int totalCopies = std::stoi(cols[6]);
            int availableCopies = std::stoi(cols[7]);

            if (totalCopies < 0) continue;
            if (availableCopies < 0) availableCopies = 0;
            if (availableCopies > totalCopies) availableCopies = totalCopies;

            books.emplace_back(id, author, title, year, genre, totalCopies);
            books.back().availableCopies = availableCopies;
        } catch (...) {
            // Skip invalid row silently.
            continue;
        }
    }

    return true;
}

bool saveBooksToCsv(const std::string& path, const std::vector<Book>& books) {
    std::ofstream out(path, std::ios::trunc);
    if (!out.is_open()) {
        return false;
    }

    out << "book_id,title,author,year,genre,price,total_copies,available_copies\n";
    for (const auto& b : books) {
        // price is not yet stored in Book model, keep placeholder 0.00
        out << b.id << ','
            << b.title << ','
            << b.author << ','
            << b.year << ','
            << genreToString(b.genre) << ','
            << "0.00,"
            << b.totalCopies << ','
            << b.availableCopies
            << '\n';
    }

    return true;
}

void printBook(const Book& b) {
    std::cout << "ID: " << b.id
              << " | Author: " << b.author
              << " | Title: " << b.title
              << " | Year: " << b.year
              << " | Genre: " << genreToString(b.genre)
              << " | Total copies: " << b.totalCopies
              << " | Available: " << b.availableCopies
              << '\n';
}

Genre askGenre() {
    std::cout << "Choose genre:\n";
    std::cout << "1 - Fiction\n";
    std::cout << "2 - Science\n";
    std::cout << "3 - Fantasy\n";
    std::cout << "4 - Detective\n";
    std::cout << "Your choice: ";

    int choice = 0;
    std::cin >> choice;
    switch (choice) {
    case 1: return Genre::Fiction;
    case 2: return Genre::Science;
    case 3: return Genre::Fantasy;
    case 4: return Genre::Detective;
    default:
        std::cout << "Unknown choice, using Fiction.\n";
        return Genre::Fiction;
    }
}

void addBookInteractive(BookCatalog& catalog) {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::string author;
    std::string title;
    int year = 0;
    int copies = 0;

    std::cout << "Author: ";
    std::getline(std::cin, author);

    std::cout << "Title: ";
    std::getline(std::cin, title);

    std::cout << "Publication year: ";
    std::cin >> year;

    Genre genre = askGenre();

    std::cout << "Number of copies: ";
    std::cin >> copies;

    std::string error;
    int id = catalog.addOrMergeBook(author, title, year, genre, copies, &error);
    if (id < 0) {
        std::cout << "Error while adding book: " << error << "\n";
    } else {
        std::cout << "Book added/merged, ID = " << id << "\n";
    }
}

void listAllBooks(const std::vector<Book>& books) {
    if (books.empty()) {
        std::cout << "Catalog is empty.\n";
        return;
    }
    for (const auto& b : books) {
        printBook(b);
    }
}

void searchByAuthorInteractive(const BookCatalog& catalog) {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::string query;
    std::cout << "Enter part of author name: ";
    std::getline(std::cin, query);

    auto results = catalog.searchByAuthor(query);
    if (results.empty()) {
        std::cout << "No books found.\n";
        return;
    }

    std::cout << "Books found: " << results.size() << "\n";
    for (const Book* b : results) {
        if (b) printBook(*b);
    }
}

void searchByTitleInteractive(const BookCatalog& catalog) {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::string query;
    std::cout << "Enter part of book title: ";
    std::getline(std::cin, query);

    auto results = catalog.searchByTitle(query);
    if (results.empty()) {
        std::cout << "No books found.\n";
        return;
    }

    std::cout << "Books found: " << results.size() << "\n";
    for (const Book* b : results) {
        if (b) printBook(*b);
    }
}

void registerReaderInteractive(LibrarySystem& lib) {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::string fullName;
    std::string contact;
    std::string regDate;

    std::cout << "Reader full name: ";
    std::getline(std::cin, fullName);

    std::cout << "Contacts (phone/email): ";
    std::getline(std::cin, contact);

    std::cout << "Registration date (YYYY-MM-DD): ";
    std::getline(std::cin, regDate);

    int id = lib.registerReader(fullName, contact, regDate);
    std::cout << "Reader registered, ID = " << id << "\n";
}

void listReaders(const LibrarySystem& lib) {
    if (lib.readers.empty()) {
        std::cout << "No readers.\n";
        return;
    }
    for (const auto& r : lib.readers) {
        std::cout << "ID: " << r.id
                  << " | Name: " << r.fullName
                  << " | Contacts: " << r.contact
                  << " | Registration date: " << r.registrationDate
                  << " | Total fine: " << r.totalFine
                  << "\n";
    }
}

void listLoans(const LibrarySystem& lib) {
    if (lib.loans.empty()) {
        std::cout << "No loans yet.\n";
        return;
    }
    for (const auto& l : lib.loans) {
        std::cout << "Loan ID: " << l.id
                  << " | Book ID: " << l.bookId
                  << " | Reader ID: " << l.readerId
                  << " | Issue date: " << l.issueDate
                  << " | Due date: " << l.dueDate
                  << " | Returned: " << (l.isReturned ? "yes" : "no")
                  << " | Fine: " << l.fine
                  << "\n";
    }
}

void issueBookInteractive(LibrarySystem& lib) {
    int bookId = 0;
    int readerId = 0;
    std::string issueDate;

    std::cout << "Book ID: ";
    std::cin >> bookId;
    std::cout << "Reader ID: ";
    std::cin >> readerId;

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << "Issue date (YYYY-MM-DD): ";
    std::getline(std::cin, issueDate);

    std::string error;
    int loanId = lib.issueBook(bookId, readerId, issueDate, &error);
    if (loanId < 0) {
        std::cout << "Error issuing book: " << error << "\n";
    } else {
        std::cout << "Book issued, loan ID = " << loanId << "\n";
    }
}

void returnBookInteractive(LibrarySystem& lib) {
    int loanId = 0;
    std::string returnDate;
    int damagedFlag = 0;

    std::cout << "Loan ID: ";
    std::cin >> loanId;

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << "Return date (YYYY-MM-DD): ";
    std::getline(std::cin, returnDate);

    std::cout << "Book damaged? (1 - yes, 0 - no): ";
    std::cin >> damagedFlag;

    bool damaged = (damagedFlag == 1);

    std::string error;
    if (!lib.returnBook(loanId, returnDate, damaged, &error)) {
        std::cout << "Error returning book: " << error << "\n";
    } else {
        std::cout << "Book successfully returned.\n";
    }
}

void reserveBookInteractive(LibrarySystem& lib) {
    int bookId = 0;
    int readerId = 0;
    std::string date;

    std::cout << "Book ID: ";
    std::cin >> bookId;
    std::cout << "Reader ID: ";
    std::cin >> readerId;

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << "Reservation date (YYYY-MM-DD): ";
    std::getline(std::cin, date);

    std::string error;
    int resId = lib.reserveBook(bookId, readerId, date, &error);
    if (resId < 0) {
        std::cout << "Error reserving book: " << error << "\n";
    } else {
        std::cout << "Reservation created, ID = " << resId << "\n";
    }
}

void printReservation(const Reservation& r) {
    std::cout << "Reservation ID: " << r.id
              << " | Book ID: " << r.bookId
              << " | Reader ID: " << r.readerId
              << " | Created at: " << r.createdAt
              << " | Active: " << (r.isActive ? "yes" : "no")
              << "\n";
}

void listReservationsInteractive(const ReservationSystem& rs, bool onlyActive) {
    const auto list = onlyActive ? rs.getActiveReservations() : rs.getAllReservations();
    if (list.empty()) {
        std::cout << (onlyActive ? "No active reservations.\n" : "No reservations.\n");
        return;
    }
    for (const Reservation* r : list) {
        if (r) printReservation(*r);
    }
}

void cancelReservationInteractive(ReservationSystem& rs) {
    int reservationId = 0;
    std::cout << "Reservation ID: ";
    std::cin >> reservationId;

    std::string error;
    if (!rs.cancelReservation(reservationId, &error)) {
        std::cout << "Error cancelling reservation: " << error << "\n"; // "ЭХ АТЫ БАТЫ" Борис Моисеев 
    } else {
        std::cout << "Reservation cancelled.\n";
    }
}

void printDebtor(const Reader& r) {
    std::cout << "Reader ID: " << r.id
              << " | Name: " << r.fullName
              << " | Total fine: " << r.totalFine
              << "\n";
}

void listDebtorsInteractive(const FineSystem& fs, double minDebt) {
    const auto debtors = fs.getDebtors(minDebt);
    if (debtors.empty()) {
        std::cout << "No debtors.\n";
        return;
    }
    for (const Reader* r : debtors) {
        if (r) printDebtor(*r);
    }
}

void showTotalDebtInteractive(const FineSystem& fs) {
    std::cout << "Total debt of all readers: " << fs.getTotalDebt() << "\n";
}

void payDebtInteractive(FineSystem& fs) {
    int readerId = 0;
    double amount = 0.0;
    std::cout << "Reader ID: ";
    std::cin >> readerId;
    std::cout << "Payment amount: ";
    std::cin >> amount;

    std::string error;
    if (!fs.payDebt(readerId, amount, &error)) {
        std::cout << "Error paying debt: " << error << "\n";
    } else {
        std::cout << "Debt payment applied.\n";
    }
}

int main() {
    std::setlocale(LC_ALL, "");

    LibrarySystem library;
    loadBooksFromCsv("base.csv", library.books);
    BookCatalog catalog(library.books);
    library.attachCatalog(&catalog);
    ReservationSystem reservationSystem(library);
    FineSystem fineSystem(library);

    while (true) {
        std::cout << "\n=== Library menu ===\n";
        std::cout << "1 - Add book\n";
        std::cout << "2 - List all books\n";
        std::cout << "3 - Search by author\n";
        std::cout << "4 - Search by title\n";
        std::cout << "5 - Register reader\n";
        std::cout << "6 - List readers\n";
        std::cout << "7 - Issue book\n";
        std::cout << "8 - Return book\n";
        std::cout << "9 - Reserve book\n";
        std::cout << "10 - List loans\n";
        std::cout << "11 - List reservations\n";
        std::cout << "12 - List active reservations\n";
        std::cout << "13 - Cancel reservation\n";
        std::cout << "14 - List debtors\n";
        std::cout << "15 - Show total debt\n";
        std::cout << "16 - Pay reader debt\n";
        std::cout << "0 - Exit\n";
        std::cout << "Your choice: ";

        int cmd = 0;
        std::cin >> cmd;

        if (!std::cin) {
            std::cout << "Input error, exiting.\n";
            break;
        }

        switch (cmd) {
        case 1:
            addBookInteractive(catalog);
            break;
        case 2:
            listAllBooks(library.books);
            break;
        case 3:
            searchByAuthorInteractive(catalog);
            break;
        case 4:
            searchByTitleInteractive(catalog);
            break;
        case 5:
            registerReaderInteractive(library);
            break;
        case 6:
            listReaders(library);
            break;
        case 7:
            issueBookInteractive(library);
            break;
        case 8:
            returnBookInteractive(library);
            break;
        case 9:
            reserveBookInteractive(library);
            break;
        case 10:
            listLoans(library);
            break;
        case 11:
            listReservationsInteractive(reservationSystem, false);
            break;
        case 12:
            listReservationsInteractive(reservationSystem, true);
            break;
        case 13:
            cancelReservationInteractive(reservationSystem);
            break;
        case 14:
            listDebtorsInteractive(fineSystem, 0.01);
            break;
        case 15:
            showTotalDebtInteractive(fineSystem);
            break;
        case 16:
            payDebtInteractive(fineSystem);
            break;
        case 0:
            saveBooksToCsv("base.csv", library.books);
            std::cout << "Exit.\n";
            return 0;
        default:
            std::cout << "Unknown command.\n";
            break;
        }
    }

    saveBooksToCsv("base.csv", library.books);
    return 0;
}

