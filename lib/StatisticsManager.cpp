#include "StatisticsManager.h"
#include "Utils.h"
#include <iostream>
#include <iomanip>
#include <cstring>

using namespace std;

StatisticsManager::StatisticsManager(UserManager& um, BookManager& bm, BorrowManager& bmgr)
    : userManager(um), bookManager(bm), borrowManager(bmgr) {}

void StatisticsManager::ShowStatisticsMenu() {
    int statChoice;

    do {
        cout << Utils::CYAN << Utils::BOLD;
        Utils::PrintMenuBorder();
        Utils::PrintMenuHeader("THONG KE THU VIEN");
        Utils::PrintMenuBorder();
        Utils::PrintMenuLine("");
        Utils::PrintMenuLine("1. Thong ke sach");
        Utils::PrintMenuLine("2. Thong ke doc gia");
        Utils::PrintMenuLine("3. Thong ke muon/tra sach");
        Utils::PrintMenuLine("0. Quay lai menu chinh");
        Utils::PrintMenuLine("");
        Utils::PrintMenuBorder();
        cout << Utils::RESET;
        cout << Utils::CYAN << "Chon: " << Utils::RESET;
        if (!(cin >> statChoice)) {
            cin.clear();
            cin.ignore(100, '\n');
            statChoice = -1;
        }
        cin.ignore(100, '\n');

        switch (statChoice) {
        case 1:
            BookStatistics();
            Utils::AskReturnToMenu();
            break;
        case 2:
            ReaderStatistics();
            Utils::AskReturnToMenu();
            break;
        case 3:
            BorrowingReturnStatistics();
            Utils::AskReturnToMenu();
            break;
        case 0:
            cout << "\nQuay lai menu chinh...";
            break;
        default:
            cout << "\nLua chon khong hop le!";
        }

    } while (statChoice != 0);
}

void StatisticsManager::BookStatistics() {
    int choice;

    do {
        cout << Utils::CYAN << Utils::BOLD;
        Utils::PrintMenuBorder();
        Utils::PrintMenuHeader("THONG KE SACH");
        Utils::PrintMenuBorder();
        Utils::PrintMenuLine("");
        Utils::PrintMenuLine("1. Tong quan sach");
        Utils::PrintMenuLine("2. Theo the loai");
        Utils::PrintMenuLine("3. Theo tac gia");
        Utils::PrintMenuLine("4. Theo tinh trang muon");
        Utils::PrintMenuLine("5. Sach duoc muon nhieu nhat");
        Utils::PrintMenuLine("0. Quay lai");
        Utils::PrintMenuLine("");
        Utils::PrintMenuBorder();
        cout << Utils::RESET;
        cout << Utils::CYAN << "Chon: " << Utils::RESET;

        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(100, '\n');
            choice = -1;
        }
        cin.ignore(100, '\n');

        switch (choice) {
        case 1: OverallBookStatistics(); Utils::AskReturnToMenu(); break;
        case 2: CategoryStatistics(); Utils::AskReturnToMenu(); break;
        case 3: AuthorStatistics(); Utils::AskReturnToMenu(); break;
        case 4: BorrowingStatusStatistics(); Utils::AskReturnToMenu(); break;
        case 5: TopBorrowedBooksStatistics(); Utils::AskReturnToMenu(); break;
        case 0: cout << "Quay lai menu thong ke.\n"; break;
        default: cout << "Lua chon khong hop le!\n";
        }

    } while (choice != 0);
}

void StatisticsManager::OverallBookStatistics() {
    bookManager.ShowStockReport();

    int totalBooks = 0;
    int availableBooks = 0;
    int lowStockBooks = 0;

    for (int i = 120; i <= 150; i++) {
        Book* book = bookManager.GetBookByID(i);
        if (book) {
            totalBooks++;
            if (book->getQuantity() > 0) availableBooks++;
            if (book->getQuantity() <= 2 && book->getQuantity() > 0) lowStockBooks++;
        }
    }

    const int WIDTH = 59;
    cout << Utils::CYAN << Utils::BOLD;
    Utils::PrintMenuBorder(WIDTH);
    Utils::PrintMenuHeader("THONG TIN TONG QUAN SACH", WIDTH);
    Utils::PrintMenuBorder(WIDTH);
    cout << Utils::RESET;

    Utils::PrintInfoRow("Tong so dau sach", to_string(totalBooks), WIDTH+2);
    Utils::PrintInfoRow("Sach con trong kho", to_string(availableBooks), WIDTH+2);
    Utils::PrintInfoRow("Sach sap het (<= 2)", to_string(lowStockBooks), WIDTH+2);

    if (totalBooks > 0) {
        stringstream ss;
        ss << fixed << setprecision(1) << (double)availableBooks / totalBooks * 100 << "%";
        Utils::PrintInfoRow("Ty le sach con trong kho", ss.str(), WIDTH+2);
    }

    cout << Utils::CYAN << Utils::BOLD;
    Utils::PrintMenuBorder(WIDTH);
    cout << Utils::RESET;
}

void StatisticsManager::CategoryStatistics() {
    struct CategoryStat {
        char category[100];
        int count;
    };

    CategoryStat stats[100];
    int numStats = 0;

    for (int i = 120; i <= 150; i++) {
        Book* book = bookManager.GetBookByID(i);
        if (book && book->getCategory()[0] != '\0') {
            char cat[100];
            strcpy(cat, book->getCategory());
            Utils::ToLowerManual(cat);
            bool found = false;
            for (int j = 0; j < numStats; j++) {
                if (strcmp(stats[j].category, cat) == 0) {
                    stats[j].count += book->getQuantity();
                    found = true;
                    break;
                }
            }
            if (!found && numStats < 100) {
                strcpy(stats[numStats].category, cat);
                stats[numStats].count = book->getQuantity();
                numStats++;
            }
        }
    }

    if (numStats == 0) {
        cout << Utils::YELLOW << "Khong co du lieu the loai." << Utils::RESET << "\n";
        return;
    }

    for (int i = 0; i < numStats - 1; i++) {
        for (int j = 0; j < numStats - i - 1; j++) {
            if (stats[j].count < stats[j + 1].count) {
                CategoryStat temp = stats[j];
                stats[j] = stats[j + 1];
                stats[j + 1] = temp;
            }
        }
    }

    int totalCopies = 0;
    for (int i = 0; i < numStats; i++) totalCopies += stats[i].count;

    const int CATEGORY_WIDTH = 25;
    const int COUNT_WIDTH = 15;
    const int PERCENTAGE_WIDTH = 10;
    const vector<int> columnWidths = {CATEGORY_WIDTH, COUNT_WIDTH, PERCENTAGE_WIDTH};

    // Tính tổng độ rộng của bảng (giống như PrintTableLine)
    int tableWidth = 0;
    for (int width : columnWidths) {
        tableWidth += width + 1;
    }
    tableWidth -= 1;

    cout << Utils::CYAN << Utils::BOLD;
    Utils::PrintMenuBorder(tableWidth);
    Utils::PrintMenuHeader("THONG KE SACH THEO THE LOAI", tableWidth);
    Utils::PrintMenuBorder(tableWidth);
    cout << Utils::RESET;

    // Table headers
    cout << Utils::BOLD << Utils::YELLOW
         << left << setw(CATEGORY_WIDTH) << "THE LOAI" << " "
         << left << setw(COUNT_WIDTH) << "SO BAN SAO" << " "
         << left << setw(PERCENTAGE_WIDTH) << "TY LE"
         << Utils::RESET << endl;

    // Separator line
    Utils::PrintTableLine(columnWidths);

    for (int i = 0; i < numStats; i++) {
        cout << left << setw(CATEGORY_WIDTH) << stats[i].category << " "
             << left << setw(COUNT_WIDTH) << stats[i].count << " "
             << fixed << setprecision(1)
             << (double)stats[i].count / totalCopies * 100 << "%"
             << endl;
    }

    // Bottom separator
    Utils::PrintTableLine(columnWidths);

    cout << left << setw(CATEGORY_WIDTH) << "TONG CONG" << " "
         << left << setw(COUNT_WIDTH) << totalCopies << " "
         << "100%" << endl;
}

void StatisticsManager::AuthorStatistics() {
    struct AuthorStat {
        char author[100];
        int count;
    };

    AuthorStat stats[100];
    int numStats = 0;

    for (int i = 120; i <= 150; i++) {
        Book* book = bookManager.GetBookByID(i);
        if (book && book->getAuthor()[0] != '\0') {
            char auth[100];
            strcpy(auth, book->getAuthor());
            bool found = false;
            for (int j = 0; j < numStats; j++) {
                if (strcmp(stats[j].author, auth) == 0) {
                    stats[j].count++;
                    found = true;
                    break;
                }
            }
            if (!found && numStats < 100) {
                strcpy(stats[numStats].author, auth);
                stats[numStats].count = 1;
                numStats++;
            }
        }
    }

    if (numStats == 0) {
        cout << Utils::YELLOW << "Khong co du lieu tac gia." << Utils::RESET << "\n";
        return;
    }

    // Bubble sort descending count
    for (int i = 0; i < numStats - 1; i++) {
        for (int j = 0; j < numStats - i - 1; j++) {
            if (stats[j].count < stats[j + 1].count) {
                AuthorStat temp = stats[j];
                stats[j] = stats[j + 1];
                stats[j + 1] = temp;
            }
        }
    }

    int totalBooks = 0;
    for (int i = 0; i < numStats; i++) totalBooks += stats[i].count;

    // Thiết lập độ rộng cột cho bảng
    const int AUTHOR_WIDTH = 30;
    const int COUNT_WIDTH = 15;
    const int PERCENTAGE_WIDTH = 10;
    const vector<int> columnWidths = {AUTHOR_WIDTH, COUNT_WIDTH, PERCENTAGE_WIDTH};

    // Tính tổng độ rộng của bảng (giống như PrintTableLine)
    int tableWidth = 0;
    for (int width : columnWidths) {
        tableWidth += width + 1;
    }
    tableWidth -= 1;

    cout << Utils::CYAN << Utils::BOLD;
    Utils::PrintMenuBorder(tableWidth);
    Utils::PrintMenuHeader("THONG KE SACH THEO TAC GIA", tableWidth);
    Utils::PrintMenuBorder(tableWidth);
    cout << Utils::RESET;

    // Table headers
    cout << Utils::BOLD << Utils::YELLOW
         << left << setw(AUTHOR_WIDTH) << "TAC GIA" << " "
         << left << setw(COUNT_WIDTH) << "SO SACH" << " "
         << left << setw(PERCENTAGE_WIDTH) << "TY LE"
         << Utils::RESET << endl;

    // Separator line
    Utils::PrintTableLine(columnWidths);

    int count = 0;
    for (int i = 0; i < numStats && count < 15; i++) {
        cout << left << setw(AUTHOR_WIDTH) << stats[i].author << " "
             << left << setw(COUNT_WIDTH) << stats[i].count << " "
             << fixed << setprecision(1)
             << (double)stats[i].count / totalBooks * 100 << "%"
             << endl;
        count++;
    }

    // Bottom separator
    Utils::PrintTableLine(columnWidths);

    cout << left << setw(AUTHOR_WIDTH) << "TONG CONG" << " "
         << left << setw(COUNT_WIDTH) << totalBooks << " "
         << "100%" << endl;
}

void StatisticsManager::BorrowingStatusStatistics() {
    // Thiết lập độ rộng cột cho bảng
    const int ID_WIDTH = 8;
    const int TITLE_WIDTH = 40;
    const int COUNT_WIDTH = 15;
    const vector<int> columnWidths = {ID_WIDTH, TITLE_WIDTH, COUNT_WIDTH};

    // Tính tổng độ rộng của bảng (giống như PrintTableLine)
    int tableWidth = 0;
    for (int width : columnWidths) {
        tableWidth += width + 1;
    }
    tableWidth -= 1;

    cout << Utils::CYAN << Utils::BOLD;
    Utils::PrintMenuBorder(tableWidth);
    Utils::PrintMenuHeader("TINH TRANG MUON SACH HIEN TAI", tableWidth);
    Utils::PrintMenuBorder(tableWidth);
    cout << Utils::RESET;

    struct BookBorrow {
        int id;
        int count;
    };

    BookBorrow borrows[200];
    int numBorrows = 0;

    const Person* users = userManager.GetAllUsers();
    int userCount = userManager.GetUserCount();

    for (int i = 0; i < userCount; i++) {
        User user;
        user.LoadUserByID(to_string(users[i].getID()));

        for (auto& item : user.getTransactionHistory()) {
            if (!item.getIsReturned()) {
                bool found = false;
                for (int j = 0; j < numBorrows; j++) {
                    if (borrows[j].id == item.getBookID()) {
                        borrows[j].count++;
                        found = true;
                        break;
                    }
                }
                if (!found && numBorrows < 200) {
                    borrows[numBorrows].id = item.getBookID();
                    borrows[numBorrows].count = 1;
                    numBorrows++;
                }
            }
        }
    }

    if (numBorrows == 0) {
        cout << Utils::YELLOW << "Khong co sach nao dang duoc muon." << Utils::RESET << "\n";
        return;
    }

    // Bubble sort descending count
    for (int i = 0; i < numBorrows - 1; i++) {
        for (int j = 0; j < numBorrows - i - 1; j++) {
            if (borrows[j].count < borrows[j + 1].count) {
                BookBorrow temp = borrows[j];
                borrows[j] = borrows[j + 1];
                borrows[j + 1] = temp;
            }
        }
    }

    // Table headers
    cout << Utils::BOLD << Utils::YELLOW
         << left << setw(ID_WIDTH) << "ID" << " "
         << left << setw(TITLE_WIDTH) << "TEN SACH" << " "
         << left << setw(COUNT_WIDTH) << "SO LUONG DANG MUON"
         << Utils::RESET << endl;

    // Separator line
    Utils::PrintTableLine(columnWidths);

    for (int i = 0; i < numBorrows; i++) {
        Book* book = bookManager.GetBookByID(borrows[i].id);
        if (!book) continue;

        string title = book->getTitle();
        if (title.size() > 39) title = title.substr(0, 36) + "...";

        cout << left << setw(ID_WIDTH) << borrows[i].id << " "
             << left << setw(TITLE_WIDTH) << title << " "
             << left << setw(COUNT_WIDTH) << borrows[i].count
             << endl;
    }

    // Bottom separator
    Utils::PrintTableLine(columnWidths);
}

void StatisticsManager::TopBorrowedBooksStatistics() {
    // Thiết lập độ rộng cột cho bảng
    const int RANK_WIDTH = 5;
    const int ID_WIDTH = 8;
    const int TITLE_WIDTH = 40;
    const int COUNT_WIDTH = 15;
    const vector<int> columnWidths = {RANK_WIDTH, ID_WIDTH, TITLE_WIDTH, COUNT_WIDTH};

    // Tính tổng độ rộng của bảng (giống như PrintTableLine)
    int tableWidth = 0;
    for (int width : columnWidths) {
        tableWidth += width + 1;
    }
    tableWidth -= 1;

    cout << Utils::CYAN << Utils::BOLD;
    Utils::PrintMenuBorder(tableWidth);
    Utils::PrintMenuHeader("TOP SACH DUOC MUON NHIEU NHAT", tableWidth);
    Utils::PrintMenuBorder(tableWidth);
    cout << Utils::RESET;

    struct BookBorrow {
        int id;
        int count;
    };

    BookBorrow borrows[200];
    int numBorrows = 0;

    const Person* users = userManager.GetAllUsers();
    int userCount = userManager.GetUserCount();

    for (int i = 0; i < userCount; i++) {
        User user;
        user.LoadUserByID(to_string(users[i].getID()));

        for (auto& item : user.getTransactionHistory()) {
            bool found = false;
            for (int j = 0; j < numBorrows; j++) {
                if (borrows[j].id == item.getBookID()) {
                    borrows[j].count++;
                    found = true;
                    break;
                }
            }
            if (!found && numBorrows < 200) {
                borrows[numBorrows].id = item.getBookID();
                borrows[numBorrows].count = 1;
                numBorrows++;
            }
        }
    }

    if (numBorrows == 0) {
        cout << Utils::YELLOW << "Khong co thong tin muon sach nao." << Utils::RESET << "\n";
        return;
    }

    // Bubble sort descending count
    for (int i = 0; i < numBorrows - 1; i++) {
        for (int j = 0; j < numBorrows - i - 1; j++) {
            if (borrows[j].count < borrows[j + 1].count) {
                BookBorrow temp = borrows[j];
                borrows[j] = borrows[j + 1];
                borrows[j + 1] = temp;
            }
        }
    }

    // Table headers
    cout << Utils::BOLD << Utils::YELLOW
         << left << setw(RANK_WIDTH) << "STT" << " "
         << left << setw(ID_WIDTH) << "ID" << " "
         << left << setw(TITLE_WIDTH) << "TEN SACH" << " "
         << left << setw(COUNT_WIDTH) << "SO LAN MUON"
         << Utils::RESET << endl;

    // Separator line
    Utils::PrintTableLine(columnWidths);

    int rank = 1;
    for (int i = 0; i < numBorrows && rank <= 10; i++) {
        Book* book = bookManager.GetBookByID(borrows[i].id);
        if (!book) continue;

        string title = book->getTitle();
        if (title.size() > 39) title = title.substr(0, 36) + "...";

        cout << left << setw(RANK_WIDTH) << rank++ << " "
             << left << setw(ID_WIDTH) << borrows[i].id << " "
             << left << setw(TITLE_WIDTH) << title << " "
             << left << setw(COUNT_WIDTH) << borrows[i].count
             << endl;
    }

    // Bottom separator
    Utils::PrintTableLine(columnWidths);
}

void StatisticsManager::ReaderStatistics() {
    const int WIDTH = 59;

    const Person* users = userManager.GetAllUsers();
    int userCount = userManager.GetUserCount();

    int usersWithBorrow = 0;
    int totalBorrowed = 0;

    for (int i = 0; i < userCount; i++) {
        User user;
        user.LoadUserByID(to_string(users[i].getID()));

        int borrowed = user.getCurrentBorrowedCount();
        if (borrowed > 0) {
            usersWithBorrow++;
            totalBorrowed += borrowed;
        }
    }

    cout << Utils::CYAN << Utils::BOLD;
    Utils::PrintMenuBorder(WIDTH);
    Utils::PrintMenuHeader("THONG KE DOC GIA", WIDTH);
    Utils::PrintMenuBorder(WIDTH);
    cout << Utils::RESET;

    Utils::PrintInfoRow("Tong so doc gia", to_string(userCount), WIDTH+2);
    Utils::PrintInfoRow("Doc gia dang muon sach", to_string(usersWithBorrow), WIDTH+2);
    Utils::PrintInfoRow("Doc gia khong muon sach",
        to_string(userCount - usersWithBorrow), WIDTH+2);
    Utils::PrintInfoRow("Tong so sach dang muon", to_string(totalBorrowed), WIDTH+2);

    if (userCount > 0) {
        stringstream ss;
        ss << fixed << setprecision(1)
           << (double)usersWithBorrow / userCount * 100 << "%";
        Utils::PrintInfoRow("Ti le doc gia dang muon", ss.str(), WIDTH+2);
    }

    cout << Utils::CYAN << Utils::BOLD;
    Utils::PrintMenuBorder(WIDTH);
    cout << Utils::RESET;
}


void StatisticsManager::BorrowingReturnStatistics() {
    int choice;
    const int WIDTH = 59;

    cout << Utils::CYAN << Utils::BOLD;
    Utils::PrintMenuBorder(WIDTH);
    Utils::PrintMenuHeader("THONG KE MUON / TRA SACH", WIDTH);
    Utils::PrintMenuBorder(WIDTH);
    Utils::PrintMenuLine("1. Danh sach muon sach");
    Utils::PrintMenuLine("2. Lich su muon sach");
    Utils::PrintMenuLine("3. Lich su tra sach");
    Utils::PrintMenuLine("4. Giao dich tat ca doc gia");
    Utils::PrintMenuLine("0. Quay lai");
    Utils::PrintMenuBorder(WIDTH);
    cout << Utils::RESET;

    cout << Utils::CYAN << "Chon: " << Utils::RESET;
    if (!(cin >> choice)) {
        cin.clear();
        cin.ignore(100, '\n');
        return;
    }
    cin.ignore(100, '\n');

    switch (choice) {
    case 1: borrowManager.ShowBorrowRecords(); break;
    case 2: borrowManager.ShowHistoryBorrow(); break;
    case 3: borrowManager.ShowHistoryReturn(); break;
    case 4:
        borrowManager.ShowAllUsersTransactionHistory(userManager, bookManager);
        break;
    case 0:
        cout << "Quay lai menu thong ke...\n";
        break;
    default:
        cout << "Lua chon khong hop le!\n";
    }
}


void StatisticsManager::ComprehensiveReport() {
    const int WIDTH = 59;

    const Person* users = userManager.GetAllUsers();
    int userCount = userManager.GetUserCount();

    int totalBooks = 0;
    for (int i = 1; i <= 200; i++) {
        if (bookManager.GetBookByID(i)) totalBooks++;
    }

    int actualBorrowed = 0;
    for (int i = 0; i < userCount; i++) {
        User user;
        user.LoadUserByID(to_string(users[i].getID()));
        actualBorrowed += user.getCurrentBorrowedCount();
    }

    cout << Utils::CYAN << Utils::BOLD;
    Utils::PrintMenuBorder(WIDTH);
    Utils::PrintMenuHeader("BAO CAO TONG HOP THU VIEN", WIDTH);
    Utils::PrintMenuBorder(WIDTH);
    cout << Utils::RESET;

    Utils::PrintInfoRow("Tong so doc gia", to_string(userCount), WIDTH+2);
    Utils::PrintInfoRow("Tong so dau sach", to_string(totalBooks), WIDTH+2);
    Utils::PrintInfoRow("Tong sach dang muon", to_string(actualBorrowed), WIDTH+2);

    if (totalBooks > 0) {
        stringstream ss;
        ss << fixed << setprecision(1)
           << (double)actualBorrowed / totalBooks * 100 << "%";
        Utils::PrintInfoRow("Ti le sach dang muon", ss.str(), WIDTH+2);
    }

    Utils::PrintInfoRow("Giao dich thang", "Dang phat trien", WIDTH+2);

    cout << Utils::CYAN << Utils::BOLD;
    Utils::PrintMenuBorder(WIDTH);
    cout << Utils::RESET;
}

