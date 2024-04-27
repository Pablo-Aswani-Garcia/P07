#include "include/tools.h"

bool ConfigureProgram(std::vector<std::string>& args) {
  std::ifstream file("table_properties.conf");
  if (!file.is_open()) {
    ERROREXIT("'table_properties.conf' file not found");
  }
  std::string line, value;
  std::getline(file, line);
  std::stringstream ss(line);
  while (ss >> value) {
    args.push_back(value);
  }
  file.close();
  return true;
}

int main(int argc, char* argv[]) {
  srand(time(NULL));
  std::map<std::string, int> parameters;
  std::vector<std::string> args;
  if (ConfigureProgram(args) && CheckCorrectParameters(args.size(), args, parameters)) {
    Table<Book>* hash_table = CreateHashTable(parameters);
    if (hash_table == nullptr) return 1;
    Menu(hash_table);
    delete hash_table;
    std::cout << MAGENTA << "Program ended." << RESET << std::endl;
    return 0;
  }
  return 1;
}