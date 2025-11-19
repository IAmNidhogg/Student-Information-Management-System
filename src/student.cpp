#include "../include/student.hpp"
#include <iostream>

// Constructor and Destructor function definition
student::student()
    : id_(""), name_(""), age_(0), score_(0.0), classType_(ClassType::CLASS_A) {
}
student::student(const std::string &id, const std::string &name, int age,
                 double score, ClassType classtype)
    : id_(id), name_(name), age_(age), score_(score), classType_(classtype) {}
student::~student() = default;

// Information getter funciton definition
std::string student::getID() const { return this->id_; }
std::string student::getName() const { return this->name_; }
int student::getAge() const { return this->age_; }
double student::getScore() const { return this->score_; }
ClassType student::getClassType() const { return this->classType_; }

// Information setter function definition
void student::setID(const std::string &id) { this->id_ = id; }
void student::setName(const std::string &name) { this->name_ = name; }
void student::setAge(int age) { this->age_ = age; }
void student::setScore(double score) { this->score_ = score; }
void student::setClass(ClassType classtype) { this->classType_ = classtype; }

// Operator overload
bool student::operator==(const student &other) const {
  return this->id_ == other.id_;
}
bool student::operator!=(const student &other) const {
  return this->id_ != other.id_;
}

// Class display tool
std::string student::classToString() const {
  switch (this->classType_) {
  case ClassType::CLASS_A:
    return "CLASS_A";
  case ClassType::CLASS_B:
    return "CLASS_B";
  case ClassType::CLASS_C:
    return "CLASS_C";
  case ClassType::CLASS_D:
    return "CLASS_D";
  case ClassType::CLASS_E:
    return "CLASS_E";
  case ClassType::CLASS_F:
    return "CLASS_F";
  default:
    return "UNKNOWN";
  }
}
ClassType student::stringToClass(const std::string &str) {
  if (str == "CLASS_A" || str == "1") {
    return ClassType::CLASS_A;
  }
  if (str == "CLASS_B" || str == "2") {
    return ClassType::CLASS_B;
  }
  if (str == "CLASS_C" || str == "3") {
    return ClassType::CLASS_C;
  }
  if (str == "CLASS_D" || str == "4") {
    return ClassType::CLASS_D;
  }
  if (str == "CLASS_E" || str == "5") {
    return ClassType::CLASS_E;
  }
  if (str == "CLASS_F" || str == "6") {
    return ClassType::CLASS_F;
  }
  return ClassType::CLASS_A;
}

// Display student's information
void student::display() const {
  std::cout << "ID: " << this->id_ << "\n"
            << "Name: " << this->name_ << "\n"
            << "Age: " << this->age_ << "\n"
            << "Score: " << this->score_ << "\n"
            << "Class: " << classToString() << std::endl;
}
