#pragma once
#include "Person.h"
#include "Utils.h"
#include "BorrowedItem.h"
#include "BookManager.h"

#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>

using namespace std;

class UserManager
{
private:
    Person users[100];
    int userCount;
    char userFilePath[100];
    int nextAvailableID;

    bool IsUserIDExist(int id);
    bool IsValidStudentID(const char idStr[]);
    string GenerateNextUserID();

public:
    UserManager();
    ~UserManager();

    void LoadUsersFromFile();
    void SaveUsersToFile() const;

    void AddUser();
    void ShowAllUsers() const;
    void DeleteUserByID(int id);
    void UpdateUserByID(int id);
    void UpdateByID(int id);
    void ChangeUserPassword(int id);

    int GetUserCount() const { return userCount; }
    Person* GetUserByID(int userID);
    const Person* GetAllUsers() const { return users; }
};
