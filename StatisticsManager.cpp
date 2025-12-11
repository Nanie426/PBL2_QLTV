#include "StatisticsManager.h"
#include "Utils.h"
#include <iostream>
#include <iomanip>
#include <map>
#include <vector>
#include <algorithm>

using namespace std;

StatisticsManager::StatisticsManager(UserManager& um, BookManager& bm, BorrowManager& bmgr)
    : userManager(um), bookManager(bm), borrowManager(bmgr) {}

void StatisticsManager::ShowStatisticsMenu() {
    int statChoice;

    do {
        cout << "\n=== THONG KE THU VIEN ===";
        cout << "\n1. Thong ke sach";
        cout << "\n2. Thong ke doc gia";
        cout << "\n3. Thong ke muon/tra sach";
        cout << "\n0. Quay lai menu chinh";
        cout << "\n=========================";
        cout << "\nNhap lua chon: ";

        if (!(cin >> statChoice)) {
            cin.clear();
            cin.ignore(100, '\n');
            statChoice = -1;
        }
        cin.ignore(100, '\n');

        switch (statChoice) {
        case 1:
            ThongKeSach();
            Utils::AskReturnToMenu();
            break;
        case 2:
            ThongKeDocGia();
            Utils::AskReturnToMenu();
            break;
        case 3:
            ThongKeMuonTra();
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

void StatisticsManager::ThongKeSach() {
    int choice;

    do {
        cout << "\n=== THONG KE SACH ===";
        cout << "\n1. Tong quan sach";
        cout << "\n2. Theo the loai";
        cout << "\n3. Theo tac gia";
        cout << "\n4. Theo tinh trang muon";
        cout << "\n5. Sach duoc muon nhieu nhat";
        cout << "\n0. Quay lai";
        cout << "\nChon: ";

        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(100, '\n');
            choice = -1;
        }
        cin.ignore(100, '\n');

        switch (choice) {
        case 1: ThongKeTongQuanSach(); Utils::AskReturnToMenu(); break;
        case 2: ThongKeTheoTheLoai(); Utils::AskReturnToMenu(); break;
        case 3: ThongKeTheoTacGia(); Utils::AskReturnToMenu(); break;
        case 4: ThongKeTinhTrangMuon(); Utils::AskReturnToMenu(); break;
        case 5: ThongKeSachMuonNhieu(); Utils::AskReturnToMenu(); break;
        case 0: cout << "Quay lai menu thong ke.\n"; break;
        default: cout << "Lua chon khong hop le!\n";
        }

        if (choice != 0) {
            cout << "\nNhan Enter de tiep tuc...";
            cin.get();
        }

    } while (choice != 0);
}

void StatisticsManager::ThongKeTongQuanSach() {
    cout << "\n=== TONG QUAN SACH ===\n";

    bookManager.ShowStockReport();

    int totalBooks = 0;
    int availableBooks = 0;
    int lowStockBooks = 0;

    for (int i = 1; i <= 100; i++) {
        Book* book = bookManager.GetBookByID(i);
        if (book) {
            totalBooks++;
            if (book->getQuantity() > 0) availableBooks++;
            if (book->getQuantity() <= 2 && book->getQuantity() > 0) lowStockBooks++;
        }
    }

    cout << "\n--- CHI TIET TINH TRANG SACH ---\n";
    cout << "Tong so dau sach: " << totalBooks << endl;
    cout << "So sach con trong kho: " << availableBooks << endl;
    cout << "So sach sap het (<= 2): " << lowStockBooks << endl;

    if (totalBooks > 0) {
        cout << fixed << setprecision(1);
        cout << "Ty le sach con trong kho: " << (double)availableBooks / totalBooks * 100 << "%\n";
    }
}

void StatisticsManager::ThongKeTheoTheLoai() {
    cout << "\n=== THONG KE SACH THEO THE LOAI ===\n";

    map<string, int> categoryStats;

    for (int i = 1; i <= 100; i++) {
        Book* book = bookManager.GetBookByID(i);
        if (book && book->getCategory()[0] != '\0') {
            categoryStats[book->getCategory()]++;
        }
    }

    if (categoryStats.empty()) {
        cout << "Khong co du lieu the loai.\n";
        return;
    }

    cout << left << setw(25) << "THE LOAI" << setw(15) << "SO LUONG" << "TY LE\n";
    cout << setfill('-') << setw(50) << "-" << setfill(' ') << "\n";

    int totalBooks = 0;
    for (auto& p : categoryStats) totalBooks += p.second;

    vector<pair<string, int>> sorted(categoryStats.begin(), categoryStats.end());
    sort(sorted.begin(), sorted.end(), [](auto& a, auto& b) { return a.second > b.second; });

    for (auto& row : sorted) {
        cout << left << setw(25) << row.first
            << setw(15) << row.second
            << fixed << setprecision(1)
            << (double)row.second / totalBooks * 100 << "%\n";
    }

    cout << setfill('-') << setw(50) << "-" << setfill(' ') << "\n";
    cout << left << setw(25) << "TONG CONG" << setw(15) << totalBooks << "100%\n";
}

void StatisticsManager::ThongKeTheoTacGia() {
    cout << "\n=== THONG KE SACH THEO TAC GIA ===\n";

    map<string, int> authorStats;

    for (int i = 1; i <= 100; i++) {
        Book* book = bookManager.GetBookByID(i);
        if (book && book->getAuthor()[0] != '\0') {
            authorStats[book->getAuthor()]++;
        }
    }

    if (authorStats.empty()) {
        cout << "Khong co du lieu tac gia.\n";
        return;
    }

    cout << left << setw(30) << "TAC GIA" << setw(15) << "SO SACH" << "TY LE\n";
    cout << setfill('-') << setw(55) << "-" << setfill(' ') << "\n";

    int totalBooks = 0;
    for (auto& p : authorStats) totalBooks += p.second;

    vector<pair<string, int>> sorted(authorStats.begin(), authorStats.end());
    sort(sorted.begin(), sorted.end(), [](auto& a, auto& b) { return a.second > b.second; });

    int count = 0;
    for (auto& row : sorted) {
        if (count >= 15) break;
        cout << left << setw(30) << row.first
            << setw(15) << row.second
            << fixed << setprecision(1)
            << (double)row.second / totalBooks * 100 << "%\n";
        count++;
    }

    cout << setfill('-') << setw(55) << "-" << setfill(' ') << "\n";
    cout << left << setw(30) << "TONG CONG" << setw(15) << totalBooks << "100%\n";
}

void StatisticsManager::ThongKeTinhTrangMuon() {
    cout << "\n=== TINH TRANG MUON SACH HIEN TAI ===\n";

    map<int, int> bookBorrowCount;

    const Person* users = userManager.GetAllUsers();
    int userCount = userManager.GetUserCount();

    for (int i = 0; i < userCount; i++) {
        User user;
        user.LoadUserByID(to_string(users[i].getID()));

        for (auto& item : user.getTransactionHistory()) {
            if (!item.getIsReturned()) {
                bookBorrowCount[item.getBookID()]++;
            }
        }
    }

    if (bookBorrowCount.empty()) {
        cout << "Khong co sach nao dang duoc muon.\n";
        return;
    }

    vector<pair<int, int>> sorted(bookBorrowCount.begin(), bookBorrowCount.end());
    sort(sorted.begin(), sorted.end(), [](auto& a, auto& b) { return a.second > b.second; });

    cout << left << setw(8) << "ID" << setw(40) << "TEN SACH" << "SO LUONG DANG MUON\n";
    cout << setfill('-') << setw(68) << "-" << setfill(' ') << "\n";

    for (auto& row : sorted) {
        Book* book = bookManager.GetBookByID(row.first);
        if (!book) continue;

        string title = book->getTitle();
        if (title.size() > 39) title = title.substr(0, 36) + "...";

        cout << left << setw(8) << row.first
            << setw(40) << title
            << row.second << "\n";
    }

    cout << setfill('-') << setw(68) << "-" << setfill(' ') << "\n";
}

void StatisticsManager::ThongKeSachMuonNhieu() {
    cout << "\n=== TOP SACH DUOC MUON NHIEU NHAT ===\n";

    map<int, int> bookBorrowCount;

    const Person* users = userManager.GetAllUsers();
    int userCount = userManager.GetUserCount();

    for (int i = 0; i < userCount; i++) {
        User user;
        user.LoadUserByID(to_string(users[i].getID()));

        for (auto& item : user.getTransactionHistory()) {
            bookBorrowCount[item.getBookID()]++;
        }
    }

    if (bookBorrowCount.empty()) {
        cout << "Chua co thong tin muon sach.\n";
        return;
    }

    vector<pair<int, int>> sorted(bookBorrowCount.begin(), bookBorrowCount.end());
    sort(sorted.begin(), sorted.end(), [](auto& a, auto& b) { return a.second > b.second; });

    cout << left << setw(5) << "STT" << setw(8) << "ID" << setw(40) << "TEN SACH" << "SO LAN MUON\n";
    cout << setfill('-') << setw(78) << "-" << setfill(' ') << "\n";

    int rank = 1;
    for (auto& row : sorted) {
        if (rank > 10) break;

        Book* book = bookManager.GetBookByID(row.first);
        if (!book) continue;

        string title = book->getTitle();
        if (title.size() > 39) title = title.substr(0, 36) + "...";

        cout << left << setw(5) << rank++
            << setw(8) << row.first
            << setw(40) << title
            << row.second << "\n";
    }

    cout << setfill('-') << setw(78) << "-" << setfill(' ') << "\n";
}

void StatisticsManager::ThongKeDocGia() {
    cout << "\n=== THONG KE DOC GIA ===\n";

    const Person* users = userManager.GetAllUsers();
    int userCount = userManager.GetUserCount();

    cout << "Tong so doc gia: " << userCount << "\n";

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

    cout << "So doc gia dang muon sach: " << usersWithBorrow << "\n";
    cout << "So doc gia khong muon sach: " << (userCount - usersWithBorrow) << "\n";
    cout << "Tong so sach dang muon: " << totalBorrowed << "\n";

    if (userCount > 0) {
        cout << fixed << setprecision(1);
        cout << "Ti le doc gia dang muon: " << (double)usersWithBorrow / userCount * 100 << "%\n";
    }
}

void StatisticsManager::ThongKeMuonTra() {
    cout << "\n=== THONG KE MUON/TRA ===\n";

    int choice;
    cout << "1. Danh sach muon sach\n";
    cout << "2. Lich su muon\n";
    cout << "3. Lich su tra\n";
    cout << "4. Giao dich tat ca doc gia\n";
    cout << "Chon: ";

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
    case 4: borrowManager.ShowAllUsersTransactionHistory(userManager, bookManager); break;
    default: cout << "Lua chon khong hop le!\n"; break;
    }
}

void StatisticsManager::BaoCaoTongHop() {
    cout << "\n=== BAO CAO TONG HOP ===\n";

    const Person* users = userManager.GetAllUsers();
    int userCount = userManager.GetUserCount();

    int totalBooks = 0;

    for (int i = 1; i <= 100; i++) {
        if (bookManager.GetBookByID(i)) totalBooks++;
    }

    int actualBorrowed = 0;
    for (int i = 0; i < userCount; i++) {
        User user;
        user.LoadUserByID(to_string(users[i].getID()));
        actualBorrowed += user.getCurrentBorrowedCount();
    }

    cout << "Tong doc gia: " << userCount << "\n";
    cout << "Tong dau sach: " << totalBooks << "\n";
    cout << "Tong sach dang muon: " << actualBorrowed << "\n";

    if (totalBooks > 0) {
        cout << fixed << setprecision(1);
        cout << "Ti le muon: " << (double)actualBorrowed / totalBooks * 100 << "%\n";
    }

    cout << "\nSo giao dich trong thang: (Dang phat trien)\n";
}
