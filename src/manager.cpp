#include "../include/manager.hpp"
#include "../include/utility.hpp"

#include "json/myjson.hpp"
#include "json/parsing.hpp"
#include "json/serializing.hpp"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <mutex>
#include <numeric>
#include <sstream>
#include <string>

Manager::Manager() { 
  std::string datPath = utility::getProjectRoot() + "/student.dat";
  loadFromB(datPath);
}
Manager::~Manager() { 
  std::string datPath = utility::getProjectRoot() + "/student.dat";
  saveToB(datPath);
}

// Information management
bool Manager::addStudent(const student &stu) {
  std::lock_guard<std::mutex> lock(this->mtx);

  if (!isIDUnique(stu.getID())) {
    std::string detail = "ID: " + stu.getID() + " exists!";
    log("Operation add fail", detail);
    return false;
  }

  students.push_back(stu);
  std::string detail = "ID: " + stu.getID() + "; Name: " + stu.getName();
  log("Operation add success", detail);
  return true;
}

bool Manager::deleteStudent(const std::string &id) {
  std::lock_guard<std::mutex> lock(this->mtx);

  auto it =
      std::find_if(students.begin(), students.end(),
                   [&id](const student &stu) { return stu.getID() == id; });

  if (it != students.end()) {
    std::string detail = "ID: " + id + "; Name: " + it->getName();
    students.erase(it);
    log("Operation delete success", detail);
    return true;
  }

  log("Operation delete fail", "ID: " + id + " doesn't exist");
  return false;
}

bool Manager::updateStudent(const std::string &id, const student &newstu) {
  std::lock_guard<std::mutex> lock(this->mtx);

  auto it =
      std::find_if(students.begin(), students.end(),
                   [&id](const student &stu) { return stu.getID() == id; });

  if (it != students.end()) {
    std::string detail = "ID: " + id + "; Name: " + it->getName();
    *it = newstu;
    detail = detail + " changes to " + "ID: " + newstu.getID() +
             "; Name: " + newstu.getName();
    log("Operation update success", detail);
    return true;
  }

  log("Operation update fail", "ID: " + id + " doesn't exist");
  return false;
}

// File management
bool Manager::loadFromB(const std::string &filename) {
  std::ifstream file(filename, std::ios::binary);
  if (!file.is_open()) {
    log("Operation load fail", "File " + filename + " doesn't exist");
    return false;
  }

  students.clear();
  size_t count;
  if (!file.read(reinterpret_cast<char *>(&count), sizeof(count))) {
    log("Operation load fail", "File " + filename + " has something wrong");
    return false;
  }

  // Check for reasonable count to prevent memory exhaustion
  if (count > 1000000) { // Reasonable upper limit
    log("Operation load fail",
        "File " + filename + " contains invalid data (count too large)");
    return false;
  }

  for (size_t index = 0; index < count; ++index) {
    student s;
    size_t len;

    // read id
    if (!file.read(reinterpret_cast<char *>(&len), sizeof(len))) {
      log("Operation load fail",
          "File " + filename + " has something wrong while reading ID length");
      return false;
    }
    if (len > 1000) { // Reasonable upper limit for string length
      log("Operation load fail",
          "File " + filename + " contains invalid data (ID length too large)");
      return false;
    }
    std::string id(len, '\0');
    if (!file.read(&id[0], len)) {
      log("Operation load fail",
          "File " + filename + " has something wrong while reading ID");
      return false;
    }
    s.setID(id);

    // read name
    if (!file.read(reinterpret_cast<char *>(&len), sizeof(len))) {
      log("Operation load fail",
          "File " + filename +
              " has something wrong while reading name length");
      return false;
    }
    if (len > 1000) { // Reasonable upper limit for string length
      log("Operation load fail",
          "File " + filename +
              " contains invalid data (name length too large)");
      return false;
    }
    std::string name(len, '\0');
    if (!file.read(&name[0], len)) {
      log("Operation load fail",
          "File " + filename + " has something wrong while reading name");
      return false;
    }
    s.setName(name);

    // read age
    int age;
    if (!file.read(reinterpret_cast<char *>(&age), sizeof(age))) {
      log("Operation load fail",
          "File " + filename + " has something wrong while reading age");
      return false;
    }
    s.setAge(age);

    // read score
    double score;
    if (!file.read(reinterpret_cast<char *>(&score), sizeof(score))) {
      log("Operation load fail",
          "File " + filename + " has something wrong while reading score");
      return false;
    }
    s.setScore(score);

    // read class
    int classType;
    if (!file.read(reinterpret_cast<char *>(&classType), sizeof(classType))) {
      log("Operation load fail",
          "File " + filename + " has something wrong while reading class");
      return false;
    }
    s.setClass(static_cast<ClassType>(classType));

    students.push_back(s);
  }

  file.close();
  log("Operation load success", "Successfully load " +
                                    std::to_string(students.size()) +
                                    " messages from " + filename);
  return true;
}

bool Manager::saveToB(const std::string &filename) {
  std::ofstream file(filename, std::ios::binary);
  if (!file.is_open()) {
    log("Operation save fail", "Can't create or find " + filename);
    return false;
  }

  size_t count = students.size();
  file.write(reinterpret_cast<char *>(&count), sizeof(count));

  for (const auto &s : students) {
    // write id
    std::string id = s.getID();
    size_t len = id.length();
    file.write(reinterpret_cast<char *>(&len), sizeof(len));
    file.write(id.c_str(), len);

    // write name
    std::string name = s.getName();
    len = name.length();
    file.write(reinterpret_cast<char *>(&len), sizeof(len));
    file.write(name.c_str(), len);

    // write age
    int age = s.getAge();
    file.write(reinterpret_cast<char *>(&age), sizeof(age));

    // write score
    double score = s.getScore();
    file.write(reinterpret_cast<char *>(&score), sizeof(score));

    // write class
    int classType = static_cast<int>(s.getClassType());
    file.write(reinterpret_cast<char *>(&classType), sizeof(classType));
  }

  file.close();
  log("Operation save success", "Successfully save " +
                                    std::to_string(students.size()) +
                                    " messages to " + filename);
  return true;
}

bool Manager::loadFromJson(const std::string &filename) {
  // Convert to project root path if relative path
  std::string filepath = filename;
  // Check if it's an absolute path (Unix: starts with /, Windows: starts with C:\ etc.)
  bool isAbsolute = (filename.length() > 0 && filename[0] == '/') || 
                    (filename.length() > 1 && filename[1] == ':');
  if (!isAbsolute) {
    filepath = utility::getProjectRoot() + "/" + filename;
  }
  
  std::ifstream file(filepath);
  if (!file.is_open()) {
    log("Operation load fail", "File " + filename + " doesn't exist");
    return false;
  }
  std::stringstream buffer;
  buffer << file.rdbuf();
  std::string json_content = buffer.str();
  file.close();

  if (json_content.empty()) {
    log("Operation load fail", "File " + filename + " is empty");
    return false;
  }

  try {
    JSON root = parse(json_content);
    if (!root.isObject()) {
      log("Operation load fail", "Invalid JSON format: root is not an object");
      return false;
    }

    // Check if "students" key exists and is an array
    auto students_it = root._data.find("students");
    if (students_it == root._data.end()) {
      log("Operation load fail", "Invalid JSON format: 'students' key not found");
      return false;
    }
    
    if (!students_it->second.isArray()) {
      log("Operation load fail",
          "Invalid JSON format: 'students' is not an array");
      return false;
    }

    // Parse to temporary vector first, only replace original data if successful
    std::vector<student> temp_students;
    JSON &student_array = students_it->second;
    
    for (size_t i = 0; i < student_array._array.size(); ++i) {
      JSON &student_json = student_array._array[i];

      if (!student_json.isObject()) {
        continue; // Skip invalid entries
      }
      
      student s;
      bool hasRequiredFields = false;

      // Parse ID - required field
      auto id_it = student_json._data.find("id");
      if (id_it != student_json._data.end() && id_it->second.isValue()) {
        s.setID(id_it->second.get<std::string>());
        hasRequiredFields = true;
      } else {
        continue; // Skip if ID is missing
      }

      // Parse Name
      auto name_it = student_json._data.find("name");
      if (name_it != student_json._data.end() && name_it->second.isValue()) {
        s.setName(name_it->second.get<std::string>());
      }

      // Parse Age
      auto age_it = student_json._data.find("age");
      if (age_it != student_json._data.end() && age_it->second.isValue()) {
        s.setAge(age_it->second.get<int>());
      }

      // Parse Score
      auto score_it = student_json._data.find("score");
      if (score_it != student_json._data.end() && score_it->second.isValue()) {
        if (score_it->second._value._numbertype == 1) {
          s.setScore(score_it->second.get<double>());
        } else {
          s.setScore(static_cast<double>(score_it->second.get<int>()));
        }
      }

      // Parse Class
      auto class_it = student_json._data.find("class");
      if (class_it != student_json._data.end() && class_it->second.isValue()) {
        std::string class_str = class_it->second.get<std::string>();
        s.setClass(student::stringToClass(class_str));
      }

      // Only add if has required fields
      if (hasRequiredFields) {
        temp_students.push_back(s);
      }
    }
    
    // Only replace original data if parsing was successful
    std::lock_guard<std::mutex> lock(mtx);
    students = std::move(temp_students);
    
    log("Operation load success", "Successfully load " +
                                      std::to_string(students.size()) +
                                      " messages from " + filename);
    return true;
  } catch (const std::exception &e) {
    log("Operation load fail", "JSON parsing error: " + std::string(e.what()));
    return false;
  }
}

bool Manager::saveToJson(const std::string &filename) {
  // Convert to project root path if relative path
  std::string filepath = filename;
  // Check if it's an absolute path (Unix: starts with /, Windows: starts with C:\ etc.)
  bool isAbsolute = (filename.length() > 0 && filename[0] == '/') || 
                    (filename.length() > 1 && filename[1] == ':');
  if (!isAbsolute) {
    filepath = utility::getProjectRoot() + "/" + filename;
  }
  
  std::lock_guard<std::mutex> lock(mtx);
  JSON root(JSONType::OBJECT);
  JSON student_array(JSONType::ARRAY);

  for (const auto &s : students) {
    JSON student_obj(JSONType::OBJECT);
    student_obj["id"] = JSON(s.getID());
    student_obj["name"] = JSON(s.getName());
    student_obj["age"] = JSON(s.getAge());
    student_obj["score"] = JSON(s.getScore());
    student_obj["class"] = JSON(s.classToString());

    student_array.appendArray(student_obj);
  }
  root["students"] = student_array;
  std::string json_string = stringify(root);

  std::ofstream file(filepath);
  if (!file.is_open()) {
    log("Operation save fail", "Can't create or find " + filename);
    return false;
  }

  file << json_string;
  file.close();
  log("Operation save success", "Successfully save " +
                                    std::to_string(students.size()) +
                                    " messages to " + filename);
  return true;
}

// search
std::vector<student *> Manager::searchByID(const std::string &keyword) {
  std::lock_guard<std::mutex> lock(mtx);
  return searchByCriteria([&keyword](const student &s) {
    return s.getID().find(keyword) != std::string::npos;
  });
}

std::vector<student *> Manager::searchByName(const std::string &keyword) {
  std::lock_guard<std::mutex> lock(mtx);
  return searchByCriteria([&keyword](const student &s) {
    return s.getName().find(keyword) != std::string::npos;
  });
}

std::vector<student *> Manager::searchByAge(int minage, int maxage) {
  std::lock_guard<std::mutex> lock(mtx);
  return searchByCriteria([minage, maxage](const student &s) {
    return minage <= s.getAge() && s.getAge() <= maxage;
  });
}

std::vector<student *> Manager::searchByScore(double minscore,
                                              double maxscore) {
  std::lock_guard<std::mutex> lock(mtx);
  return searchByCriteria([minscore, maxscore](const student &s) {
    return minscore <= s.getScore() && s.getScore() <= maxscore;
  });
}

// sort
void Manager::sortByID(bool ascending) {
  std::lock_guard<std::mutex> lock(mtx);
  std::sort(students.begin(), students.end(),
            [ascending](const student &a, const student &b) {
              return ascending ? a.getID() < b.getID() : a.getID() > b.getID();
            });
  log("Operation sort finished", "According to ID, ascending is " +
                                     std::string(ascending ? "true" : "false"));
}

void Manager::sortByName(bool ascending) {
  std::lock_guard<std::mutex> lock(mtx);
  std::sort(students.begin(), students.end(),
            [ascending](const student &a, const student &b) {
              return ascending ? a.getName() < b.getName()
                               : a.getName() > b.getName();
            });
  log("Operation sort finished", "According to Name, ascending is " +
                                     std::string(ascending ? "true" : "false"));
}

void Manager::sortByAge(bool ascending) {
  std::lock_guard<std::mutex> lock(mtx);
  std::sort(students.begin(), students.end(),
            [ascending](const student &a, const student &b) {
              return ascending ? a.getAge() < b.getAge()
                               : a.getAge() > b.getAge();
            });
  log("Operation sort finished", "According to Age, ascending is " +
                                     std::string(ascending ? "true" : "false"));
}

void Manager::sortByScore(bool ascending) {
  std::lock_guard<std::mutex> lock(mtx);
  std::sort(students.begin(), students.end(),
            [ascending](const student &a, const student &b) {
              return ascending ? a.getScore() < b.getScore()
                               : a.getScore() > b.getScore();
            });
  log("Operation sort finished", "According to Score, ascending is " +
                                     std::string(ascending ? "true" : "false"));
}

// calculation
double Manager::average() const {
  std::lock_guard<std::mutex> lock(mtx);
  if (students.empty())
    return 0.0;

  double sum = std::accumulate(
      students.begin(), students.end(), 0.0,
      [](double acc, const student &s) { return acc + s.getScore(); });
  return sum / students.size();
}

double Manager::highest() const {
  std::lock_guard<std::mutex> lock(mtx);
  if (students.empty())
    return 0.0;

  auto it = std::max_element(students.begin(), students.end(),
                             [](const student &a, const student &b) {
                               return a.getScore() < b.getScore();
                             });
  return it->getScore();
}

double Manager::lowest() const {
  std::lock_guard<std::mutex> lock(mtx);
  if (students.empty())
    return 0.0;

  auto it = std::min_element(students.begin(), students.end(),
                             [](const student &a, const student &b) {
                               return a.getScore() < b.getScore();
                             });
  return it->getScore();
}

int Manager::totalStudent() const {
  std::lock_guard<std::mutex> lock(mtx);
  return students.size();
}

// traversal
void Manager::forEach(Callback callback) const {
  std::lock_guard<std::mutex> lock(mtx);
  for (const auto &s : students) {
    callback(s);
  }
}

// other function
const std::vector<student> &Manager::getAll() const { return students; }

void Manager::clearAll() {
  std::lock_guard<std::mutex> lock(mtx);
  students.clear();
  log("Operation clear success", "All students' information have been deleted");
}

void Manager::displayAll() const {
  std::lock_guard<std::mutex> lock(mtx);
  if (students.empty()) {
    std::cout << "No student exists" << std::endl;
    return;
  }

  std::cout << "\n========== Student Information List ==========" << std::endl;
  for (size_t index = 0; index < students.size(); ++index) {
    std::cout << "[" << (index + 1) << "] ";
    students[index].display();
  }
  std::cout << "==================================\n" << std::endl;
}

bool Manager::isIDUnique(const std::string &id) const {
  return std::none_of(students.begin(), students.end(),
                      [&id](const student &s) { return s.getID() == id; });
}

// mutex
std::mutex &Manager::getMutex() { return mtx; }

// tools
void Manager::log(const std::string &operation, const std::string &detail) {
  utility::writelog(operation, detail);
}

std::vector<student *>
Manager::searchByCriteria(std::function<bool(const student &)> criteria) {
  std::vector<student *> results;
  for (auto &s : students) {
    if (criteria(s)) {
      results.push_back(&s);
    }
  }
  return results;
}
