#pragma once

#include <vector>
#include <string>

#include "LibrarySystem.h"


class ReservationSystem {
public:
    explicit ReservationSystem(LibrarySystem& library)
        : library_(library) {}

   
    std::vector<const Reservation*> getAllReservations() const;

   
    std::vector<const Reservation*> getActiveReservations() const;

   
    std::vector<const Reservation*> getReservationsByBook(int bookId,
                                                          bool onlyActive = false) const;

   
    std::vector<const Reservation*> getReservationsByReader(int readerId,
                                                            bool onlyActive = false) const;

    
    int countActiveForBook(int bookId) const;

    bool cancelReservation(int reservationId, std::string* error = nullptr);

private:
    LibrarySystem& library_;
};

