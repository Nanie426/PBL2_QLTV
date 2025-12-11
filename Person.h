#pragma once
#include <iostream>
#include <sstream>
#include <cstring>
#include <fstream>

using namespace std;

class Person
{
private:
    int ID;
    char name[100];
    char date[11];
    char phone[11];
    char email[100];
    char password[50];

    void CopyStringManual(char* dest, const char* src, size_t dest_size);
    int CompareStringManual(const char* s1, const char* s2) const;
    void IntToStringManual(int num, char* buffer, size_t buffer_size);

public:
    Person();
    Person(int id, const char n[], const char d[], const char p[], const char e[], const char pw[]);
    virtual ~Person();

    void setName(const char n[]);
    void setDate(const char d[]);
    void setPhone(const char p[]);
    void setEmail(const char e[]);
    void setID(int id);
    void setPassword(const char pw[]);

    int getID() const;
    const char* getName() const;
    const char* getDate() const;
    const char* getPhone() const;
    const char* getEmail() const;
    const char* getPassword() const;

    bool ChangePassword();

    virtual void Show() const;
    void Update();
    void UpdateByUser();
    void InputAccountDetails();
    string getLastName() const;
    void InputUserDetails();

    friend ostream& operator<<(ostream& out, const Person& p);
    friend istream& operator>>(istream& in, Person& p);
};
