#ifndef INCLUDE_MANAGER_HPP
#define INCLUDE_MANAGER_HPP

#include "student.hpp"

#include <functional>
#include <mutex>
#include <string>
#include <vector>

typedef std::function<bool(const student &)> Filter;
typedef std::function<bool(const student &, const student &)> SortStrategy;
typedef std::function<void(const student &)> Callback;

class Manager {
public:
  Manager();
  ~Manager();

  // Ban Copy Constructor & Assignment Operator
  Manager(const student &stu) = delete;
  Manager &operator=(const student &other) = delete;

  // Information management
  bool addStudent(const student &stu);
  bool deleteStudent(const std::string &id);
  bool updateStudent(const std::string &id, const student &newstu);

  // File management
  bool loadFromB(const std::string &filename = "students.dat");
  bool saveToB(const std::string &filename = "students.dat");
  bool loadFromJson(const std::string &filename = "students.json");
  bool saveToJson(const std::string &filename = "students.json");

  // search
  std::vector<student *> searchByID(const std::string &keyword);
  std::vector<student *> searchByName(const std::string &keyword);
  std::vector<student *> searchByAge(int minage, int maxage);
  std::vector<student *> searchByScore(double minscore, double maxscore);

  // sort
  void sortByID(bool ascending = true);
  void sortByName(bool ascending = true);
  void sortByAge(bool ascending = true);
  void sortByScore(bool ascending = true);

  // calculation
  double average() const;
  double highest() const;
  double lowest() const;
  int totalStudent() const;

  // traversal
  void forEach(Callback callback) const;

  // other function
  const std::vector<student> &getAll() const;
  void clearAll();
  void displayAll() const;
  bool isIDUnique(const std::string &id) const;

  // mutex
  std::mutex &getMutex();

private:
  std::vector<student> students;
  mutable std::mutex mtx;

  // tools
  void log(const std::string &operation, const std::string &detail);
  std::vector<student *>
  searchByCriteria(std::function<bool(const student &)> criteria);
};
#endif
