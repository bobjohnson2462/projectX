#pragma once

#include <algorithm>
#include <cctype>
#include <string>
#include <vector>
#include "Book.h"

class BookCatalog {
public:
    explicit BookCatalog(std::vector<Book>& books)
        : books_(books) {}


    int addOrMergeBook(const std::string& author,
                       const std::string& title,
                       int year,
                       Genre genre,
                       int copiesToAdd,
                       std::string* error = nullptr) {
        if (copiesToAdd <= 0) {
            if (error) *error = "Íåëüçÿ äîáàâëÿòü 0 èëè îòðèöàòåëüíîå ÷èñëî ýêçåìïëÿðîâ.";
            return -1;
        }
        if (year <= 0) {
            if (error) *error = "Ãîä èçäàíèÿ äîëæåí áûòü ïîëîæèòåëüíûì.";
            return -1;
        }

        if (Book* existing = findSameBook(author, title, year, genre)) {
            existing->totalCopies += copiesToAdd;
            existing->availableCopies += copiesToAdd;
            return existing->id;
        }

        const int id = generateNextId();
        books_.emplace_back(id, author, title, year, genre, copiesToAdd);
        return id;
    }

    Book* findById(int id) {
        for (auto& b : books_) {
            if (b.id == id) return &b;
        }
        return nullptr;
    }

    const Book* findById(int id) const {
        for (const auto& b : books_) {
            if (b.id == id) return &b;
        }
        return nullptr;
    }

    std::vector<const Book*> searchByAuthor(const std::string& query, bool ignoreCaseAscii = true) const {
        return searchByStringField(query, ignoreCaseAscii, [](const Book& b) -> const std::string& { return b.author; });
    }

    std::vector<const Book*> searchByTitle(const std::string& query, bool ignoreCaseAscii = true) const {
        return searchByStringField(query, ignoreCaseAscii, [](const Book& b) -> const std::string& { return b.title; });
    }

    std::vector<const Book*> searchByGenre(Genre g) const {
        std::vector<const Book*> out;
        out.reserve(books_.size());
        for (const auto& b : books_) {
            if (b.genre == g) out.push_back(&b);
        }
        return out;
    }

private:
    std::vector<Book>& books_;

    static std::string toLowerAscii(std::string s) {
        for (char& ch : s) {
            ch = static_cast<char>(std::tolower(static_cast<unsigned char>(ch)));
        }
        return s;
    }

    static bool contains(const std::string& text, const std::string& sub, bool ignoreCaseAscii) {
        if (sub.empty()) return true;
        if (!ignoreCaseAscii) return text.find(sub) != std::string::npos;

       
        const std::string t = toLowerAscii(text);
        const std::string s = toLowerAscii(sub);
        return t.find(s) != std::string::npos;
    }

    template <typename Getter>
    std::vector<const Book*> searchByStringField(const std::string& query, bool ignoreCaseAscii, Getter getter) const {
        std::vector<const Book*> out;
        out.reserve(books_.size());
        for (const auto& b : books_) {
            const std::string& field = getter(b);
            if (contains(field, query, ignoreCaseAscii)) out.push_back(&b);
        }
        return out;
    }

    Book* findSameBook(const std::string& author, const std::string& title, int year, Genre genre) {
        for (auto& b : books_) {
            if (b.author == author && b.title == title && b.year == year && b.genre == genre) {
                return &b;
            }
        }
        return nullptr;
    }

    int generateNextId() const {
        int maxId = 0;
        for (const auto& b : books_) {
            if (b.id > maxId) maxId = b.id;
        }
        return maxId + 1;
    }
};

