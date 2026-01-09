#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <limits>
#include "Admin.h"
#include "User.h"

using namespace std;

class UserManager;
class BookManager;

class Login
{
private:
    UserManager& userManager;
    BookManager& bookManager;
    string Trim(const string& str);
public:
    Login(UserManager& um, BookManager& bm) : userManager(um), bookManager(bm) {}
    int CompareString(const char* s1, const char* s2) const;
    bool DangNhap(const string& inputID, const string& inputPassword, const string& roleChoice, char* roleOutput);
    void ShowStartMenu();
};