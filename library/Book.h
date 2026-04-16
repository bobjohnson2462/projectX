#pragma once

#include <string>



enum class Genre {
    Fiction,      
    Science,      
    Fantasy,
    Detective,
};

class Book {
public:
    int id;                  
    std::string author;
    std::string title;
    int year;
    Genre genre;

    int totalCopies;          
    int availableCopies;      

    Book(int id,
         const std::string& author,
         const std::string& title,
         int year,
         Genre genre,
         int totalCopies)
        : id(id),
          author(author),
          title(title),
          year(year),
          genre(genre),
          totalCopies(totalCopies),
          availableCopies(totalCopies)
    {}

    bool hasAvailableCopy() const {
        return availableCopies > 0;
    }

    void takeCopy() {
        if (availableCopies > 0) {
            --availableCopies;
        }
    }

    void returnCopy() {
        if (availableCopies < totalCopies) {
            ++availableCopies;
        }
    }
};



class Reader {
public:
    int id;                    
    std::string fullName;      
    std::string contact;       
    std::string registrationDate; 

    double totalFine;          

    Reader(int id,
           const std::string& fullName,
           const std::string& contact,
           const std::string& registrationDate)
        : id(id),
          fullName(fullName),
          contact(contact),
          registrationDate(registrationDate),
          totalFine(0.0)
    {}

    bool canBorrow(double maxAllowedFine) const {
        return totalFine <= maxAllowedFine;
    }

    void addFine(double amount) {
        if (amount > 0) {
            totalFine += amount;
        }
    }

    void payFine(double amount) {
        if (amount > 0) {
            totalFine -= amount;
            if (totalFine < 0) totalFine = 0;
        }
    }
};



class Loan {
public:
    int id;                     
    int bookId;                 
    int readerId;             

    std::string issueDate;      
    std::string dueDate;        
    std::string returnDate;     

    bool isReturned;           
    double fine;               

    Loan(int id,
         int bookId,
         int readerId,
         const std::string& issueDate,
         const std::string& dueDate)
        : id(id),
          bookId(bookId),
          readerId(readerId),
          issueDate(issueDate),
          dueDate(dueDate),
          returnDate(""),
          isReturned(false),
          fine(0.0)
    {}

    bool isActive() const {
        return !isReturned;
    }

    void setFine(double amount) {
        if (amount >= 0) {
            fine = amount;
        }
    }

    void markReturned(const std::string& date) {
        isReturned = true;
        returnDate = date;
    }
};


class Reservation {
public:
    int id;                 
    int bookId;             
    int readerId;           

    std::string createdAt; 
    bool isActive;      
        
    Reservation(int id,
                int bookId,
                int readerId,
                const std::string& createdAt)
        : id(id),
          bookId(bookId),
          readerId(readerId),
          createdAt(createdAt),
          isActive(true)
    {}

    void cancel() {
        isActive = false;
    }

    void complete() {
        isActive = false;
    }
};


