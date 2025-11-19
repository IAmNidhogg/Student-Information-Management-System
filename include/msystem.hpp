#ifndef INCLUDE_MSYSTEM_HPP
#define INCLUDE_MSYSTEM_HPP

#include "manager.hpp"

void displayMenu();
void clearScreen();

// Student operation
void addStudentMenu(Manager &manager);
void deleteStudentMenu(Manager &manager);
void updateStudentMenu(Manager &manager);

// Search operation
void searchByIDMenu(Manager &manager);
void searchByNameMenu(Manager &manager);
void searchByScoreRangeMenu(Manager &manager);
void searchByAgeRangeMenu(Manager &manager);

// Sort operation
void sortByScoreMenu(Manager &manager);
void sortByAgeMenu(Manager &manager);
void sortByNameMenu(Manager &manager);
void sortByStudentIDMenu(Manager &manager);

// Statical operation
void showStatistics(Manager &manager);

// File operation
void loadFromJsonMenu(Manager &manager);
void saveToJsonMenu(Manager &manager);

// Batch operation
void addStudentsBatchMenu(Manager &manager);
void forEachStudentMenu(Manager &manager);
#endif
