#pragma once
#include "Person.h"
#include "BorrowedItem.h"
#include "Utils.h"
#include "UserManager.h"
#include "BookManager.h"
#include <vector>

class User : public Person
{
private:
    std::vector<BorrowedItem> transactionHistory;

    void LoadTransactionHistory();
    void SaveTransactionHistory() const;
    int FindActiveBorrowIndex(int bookID) const;

public:
    User();
    User(int id, const char n[], const char d[], const char p[], const char e[], const char pw[]);
    virtual ~User();

    int getCurrentBorrowedCount() const;

    void Show() const override;
    void LoadUserByID(const std::string &id);
    

    bool ReturnBook(int bookID);
    bool BorrowBook(int bookID);

    const std::vector<BorrowedItem>& getTransactionHistory() const;

    void ShowTransactionHistory(BookManager& bookManager);
    void Menu(UserManager &manager, BookManager &bm);
    
};
