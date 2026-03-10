#include "ReservationSystem.h"

std::vector<const Reservation*> ReservationSystem::getAllReservations() const {
    std::vector<const Reservation*> out;
    out.reserve(library_.reservations.size());
    for (const auto& r : library_.reservations) {
        out.push_back(&r);
    }
    return out;
}

std::vector<const Reservation*> ReservationSystem::getActiveReservations() const {
    std::vector<const Reservation*> out;
    out.reserve(library_.reservations.size());
    for (const auto& r : library_.reservations) {
        if (r.isActive) out.push_back(&r);
    }
    return out;
}

std::vector<const Reservation*> ReservationSystem::getReservationsByBook(int bookId,  bool onlyActive) const {
    std::vector<const Reservation*> out;
    out.reserve(library_.reservations.size());
    for (const auto& r : library_.reservations) {
        if (r.bookId != bookId) continue;
        if (onlyActive && !r.isActive) continue;
        out.push_back(&r);
    }
    return out;
}

std::vector<const Reservation*> ReservationSystem::getReservationsByReader(int readerId, bool onlyActive) const {
    std::vector<const Reservation*> out;
    out.reserve(library_.reservations.size());
    for (const auto& r : library_.reservations) {
        if (r.readerId != readerId) continue;
        if (onlyActive && !r.isActive) continue;
        out.push_back(&r);
    }
    return out;
}

int ReservationSystem::countActiveForBook(int bookId) const {
    int count = 0;
    for (const auto& r : library_.reservations) {
        if (r.bookId == bookId && r.isActive) {
            ++count;
        }
    }
    return count;
}

bool ReservationSystem::cancelReservation(int reservationId, std::string* error) {
    for (auto& r : library_.reservations) {
        if (r.id == reservationId) {
            if (!r.isActive) {
                if (error) *error = "Reservation is already inactive.";
                return false;
            }
            r.cancel();
            return true;
        }
    }
    if (error) *error = "Reservation with this ID not found.";
    return false;
}

