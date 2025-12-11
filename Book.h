#pragma once
#include <iostream>
#include "Person.h"
using namespace std;

class Book 
{
private:
    int bookID;
    char title[100];
    char author[100];
    char category[100];
    int pubYear;
    int quantity;
    bool isBorrowed;
    int year;
    int totalQuantity;

public:

    Book();
    Book(int id, const char* t, const char* a, const char* c, int y, int q, bool b);
    ~Book();

    int getID() const;
    const char* getTitle() const;
    const char* getAuthor() const;
    const char* getCategory() const;
    int getPubYear() const;
    int getQuantity() const;
    bool getBorrowed() const;
    int getTotalQuantity() const { return totalQuantity; }

    int getYear() const;   // Chỉ khai báo
    void setYear(int y);

    void setTotalQuantity(int total) { totalQuantity = total; }
    void setID(int id);
    void setTitle(const char* t);
    void setAuthor(const char* a);
    void setCategory(const char* c);
    void setPubYear(int y);
    void setQuantity(int q);
    void setBorrowed(bool b);

    void Show() const;
    void InputBook();
    void WriteToFile(ostream& out) const;
    void ReadToFile(istream& in);
    
};
 