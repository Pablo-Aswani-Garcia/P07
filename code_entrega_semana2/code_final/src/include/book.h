#ifndef BOOK_H
#define BOOK_H

#include "tools.h"
//#include "hashtable.h"

  // Estructura para representar una reserva de libro
struct Reservation {
    std::string name;
    std::string startDate;
    std::string returnDate;
};

class Book {
 public:
  Book() = default;
  Book(const std::string& name, const std::string& author, const float& price, const int& search_mode) : name_(name), author_(author), price_(price) {
    switch (search_mode) {
      case 0:
        for (auto& i : name_) hash_number_ += i;
        break;
      case 1:
        for (auto& i : author_) hash_number_ += i;
        break;
      case 2:
        for (auto& i : name_) hash_number_ += i;
        for (auto& i : author_) hash_number_ += i;
        break;
    }
  }
  bool operator==(const Book& book) const { return name_ == std::string(book); }
  operator long() const { return hash_number_; }
  operator std::string() const { return name_ + " | by: " + author_ + " | " + std::to_string(price_) + "€"; }

// Función para obtener la fecha de tres dias a partir de hoy en formato día-mes-año
std::string GetDate() {
    std::time_t now = std::time(nullptr);
    std::tm* localTime = std::localtime(&now);
    
    int day = localTime->tm_mday + 3;
    int month = localTime->tm_mon + 1;
    int year = localTime->tm_year + 1900;

    std::stringstream dateStream;
    dateStream << std::setfill('0') << std::setw(2) << day << "/" << std::setw(2) << month << "/" << year;

    return dateStream.str();
}

// Función para calcular la fecha de devolución (1 mes después de la fecha de inicio)
std::string CalculateReturnDate(const std::string& startDate) {
    std::tm tm = {};
    std::istringstream ss(startDate);
    ss >> std::get_time(&tm, "%d/%m/%Y");
    std::time_t time = std::mktime(&tm);
    time += 30 * 24 * 60 * 60; // Agrega 30 días (1 mes)
    tm = *std::localtime(&time);
    char buffer[11];
    std::strftime(buffer, sizeof(buffer), "%d/%m/%Y", &tm);
    return std::string(buffer);
}

// Modifica la función MakeReservation
void ReservationNotAvailable(Reservation& reservation, const Reservation& previousReservation = Reservation()) {
    std::string todayDate = GetDate();
    reservation.name = name_;
    reservation.startDate = todayDate;
    reservation.returnDate = CalculateReturnDate(todayDate);

    if (!previousReservation.name.empty() && reservation.name == previousReservation.name) {
        reservation.startDate = previousReservation.returnDate;
        reservation.returnDate = CalculateReturnDate(reservation.startDate);
        std::cout << "Nueva reserva para: " << name_ << ".  Con fecha: "  << reservation.startDate << " - " << reservation.returnDate << std::endl;
    } else {
        reservation.returnDate = CalculateReturnDate(reservation.startDate);
        std::cout << "Nueva reserva para: " << name_ << ".  Con fecha: "  << reservation.startDate << " - " << reservation.returnDate << std::endl;
    }

    // Agregar la reserva al mapa de reservas de libros
    bookReservations_[name_].push_back(reservation);
}

  void ShowReservations(const std::string& name_) {
    std::cout << "Reservas para el libro '" << name_ << "':" << std::endl;
    if (bookReservations_.find(name_) != bookReservations_.end()) {
      const std::vector<Reservation>& reservations = bookReservations_[name_];
       for (const Reservation& reservation : reservations) {
           std::cout << "Fecha de inicio: " << reservation.startDate << " | Fecha de retorno: " << reservation.returnDate << std::endl;
        }
    } else {
        std::cout << "No hay reservas para el libro '" << name_<< "'." << std::endl;
    }
}

  //Modifica fecha de retorno
  void ModifyReturnDate(const std::string& newReturnDate) {
    if (newReturnDate > returnDate_){
      returnDate_ = newReturnDate;
    } else {
      std::cout << "Fecha introducida no válida\n" << std::endl;
    }
  }

 private:
  std::string name_;
  std::string author_;
  float price_ = 0.0;
  long hash_number_ = 0;
  std::string returnDate_;
  static std::map<std::string, std::vector<Reservation>> bookReservations_;
};

#endif