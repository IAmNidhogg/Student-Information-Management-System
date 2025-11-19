#ifndef UTILITY_HEADER
#define UTILITY_HEADER

#include <string>
#include <unordered_map>

#include "myjson.hpp"

void findBracepair(const std::string &s,
                   std::unordered_map<int, int> &bracepair);

bool isSpace(char c);
bool isDouble(const std::string &s);
bool isInteger(const std::string &S);
JSON getValue(const std::string &s);

#endif
