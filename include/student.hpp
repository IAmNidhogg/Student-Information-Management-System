#ifndef INCLUDE_STUDENT_HPP
#define INCLUDE_STUDENT_HPP

#include <string>

enum class ClassType {
  CLASS_A = 1,
  CLASS_B = 2,
  CLASS_C = 3,
  CLASS_D = 4,
  CLASS_E = 5,
  CLASS_F = 6
};

class student {
public:
  // Constructor and Destructor function
  student();
  student(const std::string &id, const std::string &name, int age, double score,
          ClassType classtype);
  ~student();

  // Information getter function
  std::string getID() const;
  std::string getName() const;
  int getAge() const;
  double getScore() const;
  ClassType getClassType() const;

  // Information setter function
  void setID(const std::string &id);
  void setName(const std::string &name);
  void setAge(int age);
  void setScore(double score);
  void setClass(ClassType classType);

  // Operator overload
  bool operator==(const student &other) const;
  bool operator!=(const student &other) const;

  // Class display tool
  std::string classToString() const;
  static ClassType stringToClass(const std::string &str);

  // Display student's information
  void display() const;

private:
  std::string id_;
  std::string name_;
  int age_;
  double score_;
  ClassType classType_;
};

#endif
