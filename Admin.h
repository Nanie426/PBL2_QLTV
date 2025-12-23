#pragma once
#include <iostream>
#include <fstream>
#include "UserManager.h"
#include "BookManager.h"
#include "BorrowManager.h" 
#include "StatisticsManager.h"
#include "Utils.h"
#include "User.h"
#include <iomanip>

using namespace std;

class Admin
{
private:
    UserManager userManager;
    BookManager bookManager;
    BorrowManager borrowManager;
    StatisticsManager statsManager; 


    void UserMenu();
    void BookMenu();
    void BorrowBookMenu();
    void StatisticsMenu(); 

public:
    Admin(); 
    void Menu();
};
