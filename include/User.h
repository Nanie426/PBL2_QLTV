#pragma once
#include "Person.h"
#include "BorrowedItem.h"
#include "Utils.h"
#include "UserManager.h"
#include "BookManager.h"
#include <vector>
#include <map>

class User : public Person
{
private:
     vector<BorrowedItem> transactionHistory;

    void LoadTransactionHistory();
    void SaveTransactionHistory() const;
    std::map<int, std::vector<BorrowedItem>> LoadAllTransactionHistories() const;
    int FindActiveBorrowIndex(int bookID) const;

public:
    User();
    User(int id, const char n[], const char d[], const char p[], const char e[], const char pw[]);
    virtual ~User();

    int getCurrentBorrowedCount() const;

    void Show() const override;
    void LoadUserByID(const  string &id);
    

    bool ReturnBook(int bookID);
    bool BorrowBook(int bookID);

    const  vector<BorrowedItem>& getTransactionHistory() const;

    void ShowTransactionHistory(BookManager& bookManager);
    void Menu(UserManager &manager, BookManager &bm);
    
};
