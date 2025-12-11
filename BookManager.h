#pragma once
#include "Book.h"
#include "Utils.h"
#include <iostream>
#include <fstream>
#include <iomanip>
using namespace std;

class BookManager
{
private:
    Book books[100];
    int bookCount;
    int nextAvailableID;
    
    char filePath[100];
    bool IsBookIDExist(int id);
    char userFilePath[100];

    char toLower(char c);
    void trim(char* str);
    bool containsIgnoreCase(const char* text, const char* keyword);
    string GenerateNextBookID();
public:
    BookManager();

    void LoadBooksFromFile();
    void SaveBooksToFile() const;

    void AddBook();
    void ShowAllBooks() const;
    void SearchBookByTitle();
    void SearchBookByAuthor(const char* author) const;
    void DeleteBookByID(int id);
    void UpdateBookByID(int id);
    void ShowStockReport() const;
    void ShowStockReportPerBook() const;
    
    const Book* GetBookByID(int id) const; 
    Book* GetBookByID(int id);
};
