#pragma once
#include "UserManager.h"
#include "BookManager.h"
#include "BorrowManager.h"
#include "Utils.h"

class StatisticsManager {
private:
    UserManager& userManager;
    BookManager& bookManager;
    BorrowManager& borrowManager;

public:
     StatisticsManager(UserManager& um, BookManager& bm, BorrowManager& bmgr);

    void ShowStatisticsMenu();

    void BookStatistics();
    void OverallBookStatistics();
    void CategoryStatistics();
    void AuthorStatistics();
    void BorrowingStatusStatistics();
    void TopBorrowedBooksStatistics();

    void ReaderStatistics();

    void BorrowingReturnStatistics();

    void ComprehensiveReport();
};
