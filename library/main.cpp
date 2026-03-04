#include <iostream>
#include <string>
#include <vector>
#include <limits>

#include "Book.h"
#include "BookCatalog.h"

std::string genreToString(Genre g) {
    switch (g) {
    case Genre::Fiction:   return "Fiction";
    case Genre::Science:   return "Science";
    case Genre::Fantasy:   return "Fantasy";
    case Genre::Detective: return "Detective";
    default:               return "Unknown";
    }
}

void printBook(const Book& b) {
    std::cout << "ID: " << b.id
              << " | Автор: " << b.author
              << " | Название: " << b.title
              << " | Год: " << b.year
              << " | Жанр: " << genreToString(b.genre)
              << " | Всего экз.: " << b.totalCopies
              << " | Доступно: " << b.availableCopies
              << '\n';
}

Genre askGenre() {
    std::cout << "Выберите жанр:\n";
    std::cout << "1 - Fiction\n";
    std::cout << "2 - Science\n";
    std::cout << "3 - Fantasy\n";
    std::cout << "4 - Detective\n";
    std::cout << "Ваш выбор: ";

    int choice = 0;
    std::cin >> choice;
    switch (choice) {
    case 1: return Genre::Fiction;
    case 2: return Genre::Science;
    case 3: return Genre::Fantasy;
    case 4: return Genre::Detective;
    default:
        std::cout << "Неизвестный выбор, будет использован Fiction.\n";
        return Genre::Fiction;
    }
}

void addBookInteractive(BookCatalog& catalog) {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::string author;
    std::string title;
    int year = 0;
    int copies = 0;

    std::cout << "Автор: ";
    std::getline(std::cin, author);

    std::cout << "Название: ";
    std::getline(std::cin, title);

    std::cout << "Год издания: ";
    std::cin >> year;

    Genre genre = askGenre();

    std::cout << "Количество экземпляров: ";
    std::cin >> copies;

    std::string error;
    int id = catalog.addOrMergeBook(author, title, year, genre, copies, &error);
    if (id < 0) {
        std::cout << "Ошибка при добавлении книги: " << error << "\n";
    } else {
        std::cout << "Книга успешно добавлена/объединена, ID = " << id << "\n";
    }
}

void listAllBooks(const std::vector<Book>& books) {
    if (books.empty()) {
        std::cout << "Каталог пуст.\n";
        return;
    }
    for (const auto& b : books) {
        printBook(b);
    }
}

void searchByAuthorInteractive(const BookCatalog& catalog) {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::string query;
    std::cout << "Введите часть имени автора: ";
    std::getline(std::cin, query);

    auto results = catalog.searchByAuthor(query);
    if (results.empty()) {
        std::cout << "Книги не найдены.\n";
        return;
    }

    std::cout << "Найдено книг: " << results.size() << "\n";
    for (const Book* b : results) {
        if (b) printBook(*b);
    }
}

void searchByTitleInteractive(const BookCatalog& catalog) {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::string query;
    std::cout << "Введите часть названия книги: ";
    std::getline(std::cin, query);

    auto results = catalog.searchByTitle(query);
    if (results.empty()) {
        std::cout << "Книги не найдены.\n";
        return;
    }

    std::cout << "Найдено книг: " << results.size() << "\n";
    for (const Book* b : results) {
        if (b) printBook(*b);
    }
}

int main() {
    std::setlocale(LC_ALL, "");

    std::vector<Book> books;
    BookCatalog catalog(books);

    while (true) {
        std::cout << "\n=== Меню каталога книг ===\n";
        std::cout << "1 - Добавить книгу\n";
        std::cout << "2 - Показать все книги\n";
        std::cout << "3 - Поиск по автору\n";
        std::cout << "4 - Поиск по названию\n";
        std::cout << "0 - Выход\n";
        std::cout << "Ваш выбор: ";

        int cmd = 0;
        std::cin >> cmd;

        if (!std::cin) {
            std::cout << "Ошибка ввода, завершение.\n";
            break;
        }

        switch (cmd) {
        case 1:
            addBookInteractive(catalog);
            break;
        case 2:
            listAllBooks(books);
            break;
        case 3:
            searchByAuthorInteractive(catalog);
            break;
        case 4:
            searchByTitleInteractive(catalog);
            break;
        case 0:
            std::cout << "Выход.\n";
            return 0;
        default:
            std::cout << "Неизвестная команда.\n";
            break;
        }
    }

    return 0;
}

