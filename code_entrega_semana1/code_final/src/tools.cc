#include "include/tools.h"

bool OPEN;
int SEARCHMODE;

/** @brief Checks if the parameters are compatible with the hash function
 *         type specified. If the hash function is close, the block size
 *         and the exploration function must be specified, if is open, it
 *         must not be specified.
 *  @param[in] parameters. The parameters to check.
 *  @return True if the parameters are compatible, false otherwise.
 */
bool CheckCompatibility(const std::map<std::string, int>& parameters) {
  if (parameters.at("-hash") == 1 && parameters.find("-bs") == parameters.end() && parameters.find("-fe") == parameters.end()) {
    ERROREXIT("If the hash function is close, the block size and the exploration function must be specified");
  }
  if (parameters.at("-hash") == 0 && (parameters.find("-bs") != parameters.end() || parameters.find("-fe") != parameters.end())) {
    ERROREXIT("If the hash function is open, the block size and the exploration function must not be specified");
  }
  return true;
}

/** @brief Checks if the parameters are correct.
 *  @param[in] argc. The number of parameters.
 *  @param[in] args. The parameters.
 *  @param[in] parameters. The map to store the parameters.
 *  @return True if the parameters are correct, false otherwise.
 */
bool CheckCorrectParameters(int argc, const std::vector<std::string>& args, std::map<std::string, int>& parameters) {
  if (argc != 9 && argc != 13) {
    ERROREXIT("Incorrect number of parameters");
  }
  for (int i = 1; i < argc; i += 2) {
    std::string param = args[i];
    if (param != "-sm" && param != "-ts" && param != "-fd" && param != "-hash" && param != "-bs" && param != "-fe") {
      ERROREXIT("Invalid parameter " + param);
    }
    int value;
    if (param != "-hash") {
      try {
        value = std::stoi(args[i + 1]);
      } catch (std::invalid_argument& error) {
        ERROREXIT("Invalid value for " + param);
      }
    }
    else {
      if (args[i + 1] == "open") {
        value = 0;
        OPEN = true;
      }
      else if (args[i + 1] == "close") {
        value = 1;
        OPEN = false;
      }
      else {
        ERROREXIT("Invalid value for " + param);
      }
    }
    if (value < 0) {
      ERROREXIT("The value of " + param + " must be positive");
    }
    // 0 -> Name; 1 -> Author; 2 -> Both
    if (param == "-sm") {
      if (value < 0 || value > 2) {
        ERROREXIT("The value of " + param + " must be between 0 and 2");
      }
      SEARCHMODE = value;
    }
    // 0 -> Mod; 1 -> Sum; 2 -> Random
    else if (param == "-fd" && (value < 0 || value > 2)) {
      ERROREXIT("The value of " + param + " must be between 0 and 2");
    }
    // 0 -> Open; 1 -> Close
    else if (param == "-hash" && (value < 0 || value > 1)) {
      ERROREXIT("The value of " + param + " must be between 0 and 1");
    }
    // 0 -> Lineal; 1 -> Quadratic; 2 -> Double dispersion; 3 -> Redispersion
    else if (param == "-fe" && (value < 0 || value > 3)) {
      ERROREXIT("The value of " + param + " must be between 0 and 3");
    }
    parameters[args[i]] = value;
  }
  return CheckCompatibility(parameters);
}

/** @brief Creates a hash table with the parameters specified.
 *  @param[in] parameters. The parameters to create the hash table.
 *  @return A pointer to the hash table created.
 */
Table<Book>* CreateHashTable(const std::map<std::string, int>& parameters) {
  DisperseFunction<Book>* disperse_function = nullptr;
  std::cout << MAGENTA << "Searching by: ";
  if (SEARCHMODE == 0)      std::cout << "Name" << RESET << std::endl;
  else if (SEARCHMODE == 1) std::cout << "Author" << RESET << std::endl;
  else                      std::cout << "Name and Author" << RESET << std::endl;
  std::cout << GREEN << "Table size: " << parameters.at("-ts") << RESET << std::endl;
  switch (parameters.at("-fd")) {
    case 0:
      std::cout << GREEN << "Disperse function: Mod" << RESET << std::endl;
      disperse_function = new ModFunction<Book>(parameters.at("-ts"));
      break;
    case 1:
      std::cout << GREEN << "Disperse function: Sum" << RESET << std::endl;
      disperse_function = new SumFunction<Book>(parameters.at("-ts"));
      break;
    case 2:
      std::cout << GREEN << "Disperse function: Random" << RESET << std::endl;
      disperse_function = new RandFunction<Book>(parameters.at("-ts"));
      break;    
    default:
      break;
  }
  if (disperse_function == nullptr) {
    std::cerr << "Error creating the hash table" << std::endl;
    return nullptr;
  }
  if (!OPEN) {
    ExplorationFunction<Book>* exploration_function = nullptr;
    std::cout << GREEN << "Block size: " << parameters.at("-bs") << RESET << std::endl;
    switch (parameters.at("-fe")) {
      case 0:
        std::cout << GREEN << "Exploration function: Linear" << RESET << std::endl;
        exploration_function = new LinearFunction<Book>(parameters.at("-ts"));
        break;
      case 1:
        std::cout << GREEN << "Exploration function: Quadratic" << RESET << std::endl;
        exploration_function = new QuadraticFunction<Book>(parameters.at("-ts"));
        break;
      case 2:
        std::cout << GREEN << "Exploration function: Double dispersion" << RESET << std::endl;
        DisperseFunction<Book>* auxiliar_function;
        int option;
        std::cout << std::endl << BLUE << "0 --> Mod; 1 --> Sum; 2 --> Rand" << std::endl;
        std::cout << RED << "WARNING: " << RESET << "Double dispersion selected, introduce an auxiliar disperse function: ";
        std::cin >> option;
        switch (option) {
          case 0: auxiliar_function = new ModFunction<Book>(parameters.at("-ts")); break;
          case 1: auxiliar_function = new SumFunction<Book>(parameters.at("-ts")); break;
          case 2: auxiliar_function = new RandFunction<Book>(parameters.at("-ts")); break;
          default: std::cout << RED << "The option is not valid" << RESET << std::endl; return nullptr;
        }
        exploration_function = new DoubleDisperseFunction<Book>(parameters.at("-ts"), auxiliar_function);
        break;
      case 3:
        std::cout << GREEN << "Exploration function: Redispersion" << RESET << std::endl;
        exploration_function = new RedispersionFunction<Book>(parameters.at("-ts"));
        break;
    }
    if (exploration_function == nullptr) {
      std::cerr << "Error creating the hash table" << std::endl;
      return nullptr;
    }
    std::cout << MAGENTA << "Hash Table: Close" << RESET << std::endl;
    return new HashTable<Book>(parameters.at("-ts"), *disperse_function, *exploration_function, parameters.at("-bs"));
  }
  std::cout << MAGENTA << "Hash Table: Open" << RESET << std::endl;
  return new HashTable<Book, DynamicSequence<Book>>(parameters.at("-ts"), *disperse_function);
}

/* Variables globales para el caso 5 */
Reservation newReservation;
Reservation previousReservation;
std::map<std::string, std::vector<Reservation>> Book::bookReservations_;
std::map<std::string, Reservation> previousReservations;

/** @brief Shows the options menu of the program.
 *  @param[in] hash_table. The hash table to show.
 */
void Menu(Table<Book>* hash_table) {
  char option;
  while (option != '3') {
    std::cout << YELLOW << std::endl;
    hash_table->Write(std::cout);
    std::cout << std::endl << std::endl;
    std::cout << "1. Insert a Book" << std::endl;
    std::cout << "2. Search a Book" << std::endl;
    std::cout << "3. Save to Database" << std::endl;
    std::cout << "5. Reserve a book not available now" << std::endl;
    std::cout << "6. Extend reservation" << std::endl;
    std::cout << "4. Quit" << std::endl;
    std::cout << "Select an option: ";
    std::cin >> option;
    std::cout << RESET << std::endl;
    if (option == '4') break;
    switch (option) {
      case '1': {
        Book* new_book;
        std::string name, author;
        std::cout << BLUE << "Insert the Book's name: " << RESET;
        std::cin.ignore();
        std::getline(std::cin, name);
        std::cout << BLUE << "Insert the Book's author: " << RESET;
        std::cin.ignore();
        std::getline(std::cin, author);
        std::cout << BLUE << "Insert the Book's price: " << RESET;
        float price;
        std::cin >> price;
        new_book = new Book(name, author, price, SEARCHMODE);
        std::cout << RED << std::endl;
        if (!OPEN && hash_table->IsFull()) {
          std::cout << "The table is full!" << std::endl;
        }
        else if (hash_table->Insert(*new_book)) {
          std::cout << GREEN << "The book has been inserted succesfully" << RESET << std::endl;
        }
        else {
          std::cout << RED << "It wasn't possible to insert the book in the table" << RESET << std::endl;
        }
        std::cout << RESET;
        delete new_book;
        break;
      }
      case '2': {
        Book* book;
        std::string name, author;
        std::cout << BLUE << "Insert the Book's name to search: " << RESET;
        std::cin.ignore();
        std::getline(std::cin, name);
        std::cout << BLUE << "Insert the Book's author to search: " << RESET;
        std::cin.ignore();
        std::getline(std::cin, author);
        int index = 0;
        std::cout << std::endl;
        book = new Book(name, author, 0.0, SEARCHMODE);
        if (hash_table->Search(*book, index)) {
          std::cout << GREEN << "The Book is in the hash table" << std::endl;
          std::cout << "Book: " << std::string(*book) << " | Position: " << index << RESET << std::endl;
        }
        else {
          std::cout << RED << "The Book is not in the hash table" << RESET << std::endl;
        }
        delete book;
        break;
      }
      case '3': {
        // Implementar la función de guardar en base de datos
      }
      case '5': {
      Book* book;
      std::string name, author;
      std::cout << BLUE << "Enter the name of the book to reserve: " << RESET;
      std::cin.ignore();
      std::getline(std::cin, name);
      std::cout << std::endl;

      // Crear un objeto Book con los datos ingresados
      book = new Book(name, author, 0.0, SEARCHMODE); // Suponiendo que el constructor de Book recibe el nombre y el autor
      
      Reservation previousReservation = previousReservations[name]; // Obtener la reserva anterior para este libro

      book->ReservationNotAvailable(newReservation, previousReservation); // Llama a MakeReservation
      previousReservation = newReservation;
      book->ShowReservations(name); // Muestra la lista de reservas y fechas de disponibilidad

      if (!OPEN && hash_table->IsFull()) {
        std::cout << "The table is full!" << std::endl;
      } else if (hash_table->Insert(*book)) {
        std::cout << GREEN << "The book has been inserted succesfully\n" << RESET << std::endl;
      }

      previousReservations[name] = newReservation; // Guarda los datos si se cambia de libro

      delete book; // Liberar la memoria del objeto Book
      break;
      }
      case '6': {
      Book* book;
      std::string name, author;
      std::cout << BLUE << "Enter the name of the book to modify: " << RESET;
      std::cin.ignore();
      std::getline(std::cin, name);
      std::cout << std::endl;

      std::string newReturnDate;
      std::cout << BLUE << "Enter the new return date for the book: " << RESET;
      std::cin >> newReturnDate;

      book = new Book(name, author, 0.0, SEARCHMODE);
      book->ModifyReturnDate(newReturnDate); // Llama a ModifyReturnDate para modificar la fecha de entrega
      
      std::cout << GREEN << "Return date modified successfully to: "<< newReturnDate << RESET << std::endl;

      break;
      }
      default:
        std::cout << RED << "Incorrect option" << RESET << std::endl;
        break;
    }
  }
}