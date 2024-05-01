#ifndef BOOK_H
#define BOOK_H

#include "tools.h"
#include <list>
//#include "hashtable.h"

  // Estructura para representar una reserva de libro
struct Reservation {
    std::string name;
    std::string startDate;
    std::string returnDate;
};

class Book {
 public:
  Book() : default_(true) {}
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
    std::cout << hash_number_ << std::endl;
  }
  bool operator==(const Book& book) const { return name_ == std::string(book); }
  operator long() const { return hash_number_; }
  operator std::string() const { return name_ + ", " + author_ + " -> " + std::to_string(price_) + "€"; }
  bool IsDefault() const { return default_; }
  std::string GetName() const { return name_; }
  std::string GetAuthor() const { return author_; }
  float GetPrice() const { return price_; }
  std::string GetReturnDate() const { return returnDate_; }
  std::list<Reservation> GetReservations() const { return book_reservations_; }
  void AddReservation(const Reservation& reservation) { book_reservations_.push_back(reservation); }
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

  std::string GetOriginalDate(std::string return_date) {
    std::tm tm = {};
    std::istringstream ss(return_date);
    ss >> std::get_time(&tm, "%d/%m/%Y");
    std::time_t time = std::mktime(&tm);
    time -= 30 * 24 * 60 * 60; // Resta 30 días (1 mes)
    tm = *std::localtime(&time);
    char buffer[11];
    std::strftime(buffer, sizeof(buffer), "%d/%m/%Y", &tm);
    return std::string(buffer);
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

  bool FindPreviousReservation(const Reservation& reservation, Reservation& previousReservation) {
    for (const Reservation& res : book_reservations_) {
      if (!res.name.empty() && res.name == reservation.name) {
        previousReservation = res;
        return true;
      }
    }
    return false;
  }

  void MakeReservation(Reservation& reservation) {
    std::string todayDate = GetDate();
    std::string name;
    std::cout << "Introduce the name of the person who will make the reservation: ";
    std::cin.ignore();
    std::getline(std::cin, name);
    reservation.name = name;
    reservation.startDate = todayDate;
    reservation.returnDate = CalculateReturnDate(todayDate);
    Reservation previous_reservation;
    if (FindPreviousReservation(reservation, previous_reservation)) {
      reservation.startDate = previous_reservation.returnDate;
      reservation.returnDate = CalculateReturnDate(reservation.startDate);
      std::cout << "Nueva reserva para: " << name_ << ".  Con fecha: "  << reservation.startDate << " - " << reservation.returnDate << std::endl;
    } 
    else {
      reservation.returnDate = CalculateReturnDate(reservation.startDate);
      std::cout << "Nueva reserva para: " << name_ << ".  Con fecha: "  << reservation.startDate << " - " << reservation.returnDate << std::endl;
    }
    // Agregar la reserva al mapa de reservas de libros
    book_reservations_.push_back(reservation);
  }

  void ShowReservations(const std::string& name_) {
    std::cout << "Reservas para el libro '" << name_ << "':" << std::endl;
    if (book_reservations_.size() > 0) {
      for (const Reservation& reservation : book_reservations_) {
        std::cout << "Fecha de inicio: " << reservation.startDate << " | Fecha de retorno: " << reservation.returnDate << std::endl;
      }
    } 
    else {
      std::cout << "No hay reservas para el libro '" << name_ << "'." << std::endl;
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
  bool default_ = false;
  std::string name_;
  std::string author_;
  float price_ = 0.0;
  long hash_number_ = 0;
  std::string returnDate_;
  std::list<Reservation> book_reservations_;
};

#endif