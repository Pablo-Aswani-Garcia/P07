#ifndef TOOLS_H
#define TOOLS_H

#include <random>
#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <iomanip>
#include <algorithm>
#include <ctime>
#include <atomic>
#include <vector>
#include <map>

#include "book.h"
#include "hashtable.h"

#define ERROREXIT(message) std::cout << "./Hash: " << message << "\n" << "./Hash -h to get help\n"; return false

// ./Hash -ts <s> -fd <f> -hash <open|close> (-bs <s> -fe <f>) --> ONLY IF HASH IS CLOSE

const std::string RED = "\033[91m";
const std::string GREEN = "\033[92m";
const std::string YELLOW = "\033[93m";
const std::string BLUE = "\033[94m";
const std::string MAGENTA = "\033[95m";
const std::string CYAN = "\033[96m";
const std::string RESET = "\033[0m";

bool CheckCompatibility(const std::map<std::string, int>& parameters);
bool CheckCorrectParameters(int argc, const std::vector<std::string>& args, std::map<std::string, int>& parameters);
Table<Book>* CreateHashTable(const std::map<std::string, int>& parameters);
void Menu(Table<Book>* hash_table);

#endif