#pragma once
#include "Book.h"
#include "Utils.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
using namespace std;

class BookManager
{
private:
    Book books[100];
    int bookCount;
    int nextAvailableID = 1;
    
    char filePath[100];
    bool IsBookIDExist(int id);
    char userFilePath[100];

    char toLower(char c) const;
    void trim(char* str);
    bool containsIgnoreCase(const char* text, const char* keyword) const;
    string GenerateNextBookID();
public:
    BookManager();

    void LoadBooksFromFile();
    void SaveBooksToFile() const;

    void AddBook();
    void ShowAllBooks() const;
    void SearchBookByTitle();
    void SearchBookByAuthor(const char* author) const;
    void SearchBookByCategory(const char* category) const;
    void DeleteBookByID(int id);
    void UpdateBookByID(int id);
    void ShowStockReport() const;
    void ShowStockReportPerBook() const;
    void LoadBooks();
    
    const Book* GetBookByID(int id) const; 
    Book* GetBookByID(int id);
};
