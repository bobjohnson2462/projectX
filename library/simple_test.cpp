#include <iostream>
#include <string>
#include <vector>
#include "Book.h"
#include "BookCatalog.h"
#include "LibrarySystem.h"

void check(bool condition, const std::string& message, int iteration) {
    if (!condition) {
        std::cout << "Error on iteration " << iteration << ": " << message << std::endl;
        exit(1);
    }
}

int main() {
    std::cout << "How many times to run the test? ";
    int n;
    std::cin >> n;

    for (int i = 1; i <= n; i++) {
        std::cout << "Iteration " << i;

        LibrarySystem lib;
        BookCatalog catalog(lib.books);   
        lib.attachCatalog(&catalog);

        std::string error;
        int bookId = catalog.addOrMergeBook("Pushkin", "Fairy Tales", 1830, Genre::Fiction, 3, &error);
        check(bookId > 0, "Failed to add the book: " + error, i);

        int readerId = lib.registerReader("John Smith", "123-45-67", "2023-01-01");
        check(readerId > 0, "Failed to register the reader", i);

        int loanId = lib.issueBook(bookId, readerId, "2023-01-01", &error, 14);
        check(loanId > 0, "Failed to issue the book: " + error, i);

        bool ok = lib.returnBook(loanId, "2023-01-10", false, &error);
        check(ok, "Failed to return the book: " + error, i);

        std::cout << " Good " << std::endl;
    }

    std::cout << "\nAll " << n << " test runs completed successfully!" << std::endl;
    return 0;
}