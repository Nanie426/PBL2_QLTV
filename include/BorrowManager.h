#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <ctime>
#include <iomanip>
#include <limits>
#include <cctype>
#include "BorrowManager.h"
#include "UserManager.h"
#include "BookManager.h"
#include "Utils.h"
#include "User.h"
#include "Book.h"

class UserManager;
class BookManager;

using namespace std;

struct BorrowRecordData
{
    int userID;
    string userName;
    int bookID;
    string bookTitle;
    string borrowDate;
    string timeStamp;
};

struct TransactionRecord
{
    int userID;
    string userName;
    int bookID;
    string bookTitle;
    string borrowDate;
    string returnDate;
    bool isReturned;
    string timeStamp;
};

class BorrowManager
{
private:
    void LogBorrowRecord(int userID, const string &userName,
                         int bookID, const string &bookTitle,
                         const char *borrowDate);
    
    void LogReturnRecord(int userID, const string &userName,
                         int bookID, const string &bookTitle,
                         const string &returnDate);
    
    vector<TransactionRecord> LoadAllTransactions();
    
public:
    void HandleBorrowBook(UserManager& um, BookManager& bm);
    void HandleReturnBook(UserManager& um, BookManager& bm);
        
    void ShowBorrowRecords();
    void ShowRawBorrowHistory(const string& filename, const string& title);
    
    void ShowHistoryBorrow() {
        ShowRawBorrowHistory("BorrowRecords.txt", "THONG KE DANH SACH MUON SACH (RAW)");
    }
    void ShowHistoryReturn() {
        ShowRawBorrowHistory("ReturnRecords.txt", "THONG KE DANH SACH TRA SACH (RAW)");
    }
    void ShowAllUsersTransactionHistory(UserManager& userManager, BookManager& bookManager);
    void ShowActiveAndOverdueBorrows(UserManager& userManager, BookManager& bookManager);
    
};