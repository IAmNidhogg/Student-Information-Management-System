#include "../include/manager.hpp"
#include "../include/msystem.hpp"

#include <iostream>

int main() {
  Manager manager;
  int choice;

  std::cout << "Weclome to SIMS !!!" << std::endl;
  std::cout << "Data has been loaded !!!" << std::endl;

  while (true) {
    displayMenu();
    std::cin >> choice;

    if (std::cin.fail()) {
      std::cin.clear();
      std::cin.ignore(1024, '\n');
      std::cout << "Invalid Input, Please Re-enter !!!" << std::endl;
      continue;
    }

    clearScreen();

    switch (choice) {
    case 1:
      addStudentMenu(manager);
      break;
    case 2:
      deleteStudentMenu(manager);
      break;
    case 3:
      updateStudentMenu(manager);
      break;
    case 4:
      searchByIDMenu(manager);
      break;
    case 5:
      searchByNameMenu(manager);
      break;
    case 6:
      searchByAgeRangeMenu(manager);
      break;
    case 7:
      searchByScoreRangeMenu(manager);
      break;
    case 8:
      sortByStudentIDMenu(manager);
      break;
    case 9:
      sortByNameMenu(manager);
      break;
    case 10:
      sortByAgeMenu(manager);
      break;
    case 11:
      sortByScoreMenu(manager);
      break;
    case 12:
      manager.displayAll();
      break;
    case 13:
      showStatistics(manager);
      break;
    case 14:
      loadFromJsonMenu(manager);
      break;
    case 15:
      saveToJsonMenu(manager);
      break;
    case 16:
      addStudentsBatchMenu(manager);
      break;
    case 17:
      forEachStudentMenu(manager);
      break;
    case 0:
      std::cout
          << "Thanks for using SIMS! The data has been saved automatically!"
          << std::endl;
      return 0;
    default:
      std::cout << "Invalid choice! Please re-enter!" << std::endl;
    }
  }

  return 0;
}
