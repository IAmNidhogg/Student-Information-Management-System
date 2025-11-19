#include "../include/utility.hpp"

#include <cctype>
#include <chrono>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <cstdlib>
#include <unistd.h>
#include <limits.h>
#ifdef _WIN32
#include <windows.h>
#endif

namespace utility {
std::string getCurrentTimestamp() {
  auto now = std::chrono::system_clock::now();
  auto time = std::chrono::system_clock::to_time_t(now);
  auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
                now.time_since_epoch()) %
            1000;

  std::stringstream ss;
  ss << std::put_time(std::localtime(&time), "%Y-%m-%d %H:%M:%S");
  ss << '.' << std::setfill('0') << std::setw(3) << ms.count();
  return ss.str();
}

void writelog(const std::string &operation, const std::string &detail) {
  std::string logPath = getProjectRoot() + "/system.log";
  std::ofstream logFile(logPath, std::ios::app);
  if (logFile.is_open()) {
    logFile << "[" << getCurrentTimestamp() << "] " << operation << ": "
            << detail << std::endl;
    logFile.close();
  }
}

// verify
bool isValidID(const std::string &id) {
  if (id.empty())
    return false;
  for (char c : id) {
    if (!std::isalnum(c)) {
      return false;
    }
  }
  return true;
}

bool isValidName(const std::string &name) {
  if (name.empty())
    return false;
  for (char c : name) {
    if (!std::isalpha(c) && c != ' ' && c != '-' && c != '_') {
      return false;
    }
  }
  return true;
}

bool isValidAge(int age) { return age > 0 && age < 200; }

bool isValidScore(double score) { return score >= 0.0 && score <= 100; }

// string process
std::string trim(const std::string &str) {
  size_t start = str.find_first_not_of(" \t\n\r");
  if (start == std::string::npos)
    return "";

  size_t end = str.find_last_not_of(" \t\n\r");
  return str.substr(start, end - start + 1);
}

std::string toLower(const std::string &str) {
  std::string result = str;
  std::transform(result.begin(), result.end(), result.begin(), ::tolower);
  return result;
}

// file process
bool fileExists(const std::string &filename) {
  std::ifstream file(filename);
  return file.good();
}

std::string getProjectRoot() {
  // Try to get from environment variable first
  const char* project_root = std::getenv("PROJECT_ROOT");
  if (project_root != nullptr) {
    return std::string(project_root);
  }
  
  // Otherwise, try to find project root by looking for CMakeLists.txt
  // Start from current working directory
  char cwd[PATH_MAX];
  if (getcwd(cwd, sizeof(cwd)) != nullptr) {
    std::string current_path(cwd);
    
    // Check if we're in the project root (has CMakeLists.txt)
    if (fileExists(current_path + "/CMakeLists.txt")) {
      return current_path;
    }
    
    // Check if we're in build/bin (go up 2 levels)
    if (fileExists(current_path + "/../../CMakeLists.txt")) {
      return current_path + "/../..";
    }
    
    // Check if we're in build (go up 1 level)
    if (fileExists(current_path + "/../CMakeLists.txt")) {
      return current_path + "/..";
    }
  }
  
  // Fallback: return current directory
  return ".";
}
} // namespace utility
