#ifndef INCLUDE_UTILITY_HPP
#define INCLUDE_UTILITY_HPP

#include <string>

// utilities
namespace utility {
std::string getCurrentTimestamp();

void writelog(const std::string &operation, const std::string &detail);

// verify
bool isValidID(const std::string &id);
bool isValidName(const std::string &name);
bool isValidAge(int age);
bool isValidScore(double score);

// string process
std::string trim(const std::string &str);
std::string toLower(const std::string &str);

// file process
bool fileExists(const std::string &filename);
std::string getProjectRoot();
} // namespace utility

#endif
