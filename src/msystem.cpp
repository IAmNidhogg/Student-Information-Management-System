#include "../include/msystem.hpp"
#include "../include/utility.hpp"

#include <cstdlib>
#include <iostream>
#include <string>
#include <thread>

void displayMenu() {
  std::cout << "\n========== Student Information Management System =========="
            << std::endl;
  std::cout << "1.  Add new student" << std::endl;
  std::cout << "2.  Delete student" << std::endl;
  std::cout << "3.  Update student's information" << std::endl;
  std::cout << "4.  Search student by ID" << std::endl;
  std::cout << "5.  Search student by name" << std::endl;
  std::cout << "6.  Search student by age" << std::endl;
  std::cout << "7.  Search student by score" << std::endl;
  std::cout << "8.  Sort by ID" << std::endl;
  std::cout << "9.  Sort by name" << std::endl;
  std::cout << "10. Sort by age" << std::endl;
  std::cout << "11. Sort by score" << std::endl;
  std::cout << "12. Display all students' information" << std::endl;
  std::cout << "13. Statistical information" << std::endl;
  std::cout << "14. Load data from JSON" << std::endl;
  std::cout << "15. Save data to JSON" << std::endl;
  std::cout << "16. Batch add student(multi threads)" << std::endl;
  std::cout << "17. Traversal" << std::endl;
  std::cout << "0.  Exit the system" << std::endl;
  std::cout << "=====================================" << std::endl;
  std::cout << "Please choose operation: ";
}

void clearScreen() {
#ifdef _WIN32
  system("cls");
#else
  system("clear");
#endif
}

// student operation
void addStudentMenu(Manager &manager) {
  std::string id, name;
  int age;
  double score;
  int classindex;

  std::cout << "ID(unique): ";
  std::cin >> id;
  if (!utility::isValidID(id)) {
    std::cout << "ID Invalid !!!" << std::endl;
    return;
  }
  if (!manager.isIDUnique(id)) {
    std::cout << "ID exists !!!" << std::endl;
    return;
  }

  std::cout << "Name: ";
  std::cin.ignore();
  std::getline(std::cin, name);
  if (!utility::isValidName(name)) {
    std::cout << "Name Invalid !!!" << std::endl;
    return;
  }

  std::cout << "Age(1 - 200): ";
  std::cin >> age;
  if (!utility::isValidAge(age)) {
    std::cout << "Age Invalid !!!" << std::endl;
    return;
  }

  std::cout << "Score(0 - 100): ";
  std::cin >> score;
  if (!utility::isValidScore(score)) {
    std::cout << "Score Invalid !!!" << std::endl;
    return;
  }

  std::cout << "Class(1 : CLASS_A, 2 : CLASS_B, 3 : CLASS_C, 4 : CLASS_D, 5 : "
               "CLASS_E, 6 : CLASS_F)";
  std::cin >> classindex;
  ClassType classtype;
  switch (classindex) {
  case 1:
    classtype = ClassType::CLASS_A;
    break;
  case 2:
    classtype = ClassType::CLASS_B;
    break;
  case 3:
    classtype = ClassType::CLASS_C;
    break;
  case 4:
    classtype = ClassType::CLASS_D;
    break;
  case 5:
    classtype = ClassType::CLASS_E;
    break;
  case 6:
    classtype = ClassType::CLASS_F;
    break;
  default:
    std::cout << "Class Chosen Invalid !!!" << std::endl;
    return;
  }

  student s(id, name, age, score, classtype);
  if (manager.addStudent(s)) {
    std::cout << "Successfully add student" << std::endl;
  } else {
    std::cout << "Fail to add student" << std::endl;
  }
}

void deleteStudentMenu(Manager &manager) {
  std::string id;
  std::cout << "Please input the ID of the student you want to delete: ";
  std::cin >> id;

  if (manager.deleteStudent(id)) {
    std::cout << "Successfully delete student" << std::endl;
  } else {
    std::cout << "Fail to delete student" << std::endl;
  }
}

void updateStudentMenu(Manager &manager) {
  std::string id;
  std::cout << "Please input the ID of the student you want to update";
  std::cin >> id;

  if (!utility::isValidID(id)) {
    std::cout << "ID Invalid !!!" << std::endl;
    return;
  }

  auto results = manager.searchByID(id);
  if (results.empty()) {
    std::cout << "Student doesn't exist" << std::endl;
    return;
  }

  if (results.size() > 1) {
    std::cout << "There are multiple students, please complete the ID"
              << std::endl;
    return;
  }

  std::cout << "The student's information is below: " << std::endl;
  results[0]->display();

  std::cout << "Please correct student's information" << std::endl;
  std::string name;
  int age;
  double score;
  int classindex;

  std::cout << "Name: ";
  std::cin.ignore();
  std::getline(std::cin, name);
  if (!utility::isValidName(name)) {
    std::cout << "Name Invalid !!!" << std::endl;
    return;
  }

  std::cout << "Age(1 - 200): ";
  std::cin >> age;
  if (!utility::isValidAge(age)) {
    std::cout << "Age Invalid !!!" << std::endl;
    return;
  }

  std::cout << "Score(0 - 100): ";
  std::cin >> score;
  if (!utility::isValidScore(score)) {
    std::cout << "Score Invalid !!!" << std::endl;
    return;
  }

  std::cout << "Class(1 : CLASS_A, 2 : CLASS_B, 3 : CLASS_C, 4 : CLASS_D, 5 : "
               "CLASS_E, 6 : CLASS_F)";
  std::cin >> classindex;
  ClassType classtype;
  switch (classindex) {
  case 1:
    classtype = ClassType::CLASS_A;
    break;
  case 2:
    classtype = ClassType::CLASS_B;
    break;
  case 3:
    classtype = ClassType::CLASS_C;
    break;
  case 4:
    classtype = ClassType::CLASS_D;
    break;
  case 5:
    classtype = ClassType::CLASS_E;
    break;
  case 6:
    classtype = ClassType::CLASS_F;
    break;
  default:
    std::cout << "Class Chosen Invalid !!!" << std::endl;
    return;
  }

  student s(id, name, age, score, classtype);
  if (manager.updateStudent(id, s)) {
    std::cout << "Successfully update student" << std::endl;
  } else {
    std::cout << "Fail to update student" << std::endl;
  }
}

void searchByIDMenu(Manager &manager) {
  std::string keyword;
  std::cout << "Search by ID(fuzzy search supported): ";
  std::cin >> keyword;

  auto results = manager.searchByID(keyword);
  if (results.empty()) {
    std::cout << "Fail to find students" << std::endl;
    return;
  } else {
    std::cout << "Successfully find " << results.size() << " students"
              << std::endl;
    for (student *s : results) {
      s->display();
    }
  }
}

void searchByNameMenu(Manager &manager) {
  std::string keyword;
  std::cout << "Search by Name(fuzzy search supported): ";
  std::cin.ignore();
  std::getline(std::cin, keyword);

  auto results = manager.searchByName(keyword);
  if (results.empty()) {
    std::cout << "Fail to find students" << std::endl;
    return;
  } else {
    std::cout << "Successfully find " << results.size() << " students"
              << std::endl;
    for (student *s : results) {
      s->display();
    }
  }
}

void searchByAgeRangeMenu(Manager &manager) {
  double minage, maxage;
  std::cout << "The lowest score: ";
  std::cin >> minage;
  std::cout << "The highest score: ";
  std::cin >> maxage;

  auto results = manager.searchByAge(minage, maxage);
  if (results.empty()) {
    std::cout << "Fail to find students" << std::endl;
    return;
  } else {
    std::cout << "Successfully find " << results.size() << " students"
              << std::endl;
    for (student *s : results) {
      s->display();
    }
  }
}

void searchByScoreRangeMenu(Manager &manager) {
  double minscore, maxscore;
  std::cout << "The lowest score: ";
  std::cin >> minscore;
  std::cout << "The highest score: ";
  std::cin >> maxscore;

  auto results = manager.searchByScore(minscore, maxscore);
  if (results.empty()) {
    std::cout << "Fail to find students" << std::endl;
    return;
  } else {
    std::cout << "Successfully find " << results.size() << " students"
              << std::endl;
    for (student *s : results) {
      s->display();
    }
  }
}

// Sort operation
void sortByScoreMenu(Manager &manager) {
  int choice;
  std::cout << "Choose sorting method: 1. ascending; 2. decending" << std::endl;
  std::cout << "Please choose: ";
  std::cin >> choice;

  bool ascending = (choice == 1);
  manager.sortByScore(ascending);
  std::cout << "Sort complete !!!" << std::endl;
  manager.displayAll();
}

void sortByAgeMenu(Manager &manager) {
  int choice;
  std::cout << "Choose sorting method: 1. ascending; 2. decending" << std::endl;
  std::cout << "Please choose: ";
  std::cin >> choice;

  bool ascending = (choice == 1);
  manager.sortByAge(ascending);
  std::cout << "Sort complete !!!" << std::endl;
  manager.displayAll();
}

void sortByNameMenu(Manager &manager) {
  int choice;
  std::cout << "Choose sorting method: 1. ascending; 2. decending" << std::endl;
  std::cout << "Please choose: ";
  std::cin >> choice;

  bool ascending = (choice == 1);
  manager.sortByName(ascending);
  std::cout << "Sort complete !!!" << std::endl;
  manager.displayAll();
}

void sortByStudentIDMenu(Manager &manager) {
  int choice;
  std::cout << "Choose sorting method: 1. ascending; 2. decending" << std::endl;
  std::cout << "Please choose: ";
  std::cin >> choice;

  bool ascending = (choice == 1);
  manager.sortByID(ascending);
  std::cout << "Sort complete !!!" << std::endl;
  manager.displayAll();
}

// Statical operation
void showStatistics(Manager &manager) {
  std::cout << "\n========== Statical Information ==========" << std::endl;
  std::cout << "Total Students: " << manager.totalStudent() << std::endl;
  std::cout << "Average Score: " << manager.average() << std::endl;
  std::cout << "The highest score: " << manager.highest() << std::endl;
  std::cout << "The lowest score: " << manager.lowest() << std::endl;
  std::cout << "==============================" << std::endl;
}

// File operation
void loadFromJsonMenu(Manager &manager) {
  std::string filename;
  std::cout << "Please input the filename (will be saved in project root): ";
  std::cin >> filename;
  if (filename.empty()) {
    filename = "students.json";
  }

  if (manager.loadFromJson(filename)) {
    std::cout << "Successfully Load !!!" << std::endl;
  } else {
    std::cout << "Fail to load !!!" << std::endl;
  }
}

void saveToJsonMenu(Manager &manager) {
  std::string filename;
  std::cout << "Please input the filename (will be saved in project root): ";
  std::cin.ignore();
  std::getline(std::cin, filename);
  if (filename.empty()) {
    filename = "students.json";
  }

  if (manager.saveToJson(filename)) {
    std::cout << "Successfully Save !!!" << std::endl;
  } else {
    std::cout << "Fail to save !!!" << std::endl;
  }
}

// Batch operation
void addStudentsBatchMenu(Manager &manager) {
  int count;
  std::cout << "The number of students you want to add: ";
  std::cin >> count;
  if (count <= 0) {
    std::cout << "The number is invalid !!!" << std::endl;
    return;
  }

  std::vector<std::thread> threads;

  for (int index = 0; index < count; ++index) {
    threads.emplace_back([&manager, index]() {
      std::string id =
          "AUTO" +
          std::to_string(
              std::chrono::steady_clock::now().time_since_epoch().count());
      std::string name = "student" + std::to_string(index);
      int age = 18 + (index % 6);
      double score = 60 + (index % 40);
      ClassType classtype = static_cast<ClassType>((index % 6) + 1);

      student s(id, name, age, score, classtype);
      manager.addStudent(s);
    });
  }

  for (auto &t : threads) {
    t.join();
  }

  std::cout << "Successfully add multiple students(random information) !!!"
            << std::endl;
}

void forEachStudentMenu(Manager &manager) {
  std::cout << "Traverse all students" << std::endl;
  manager.forEach([](const student &s) { s.display(); });
}
