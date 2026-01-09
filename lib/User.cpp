#include "User.h"
#include "Utils.h"
#include <sstream>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <algorithm>
#include <map>

using namespace std;

#define MAX_BORROWED_BOOKS 5

const int MAX_STREAM_SIZE_MANUAL = 100;

int User::getCurrentBorrowedCount() const {
    int count = 0;
    for (const auto& item : transactionHistory) {
        if (!item.getIsReturned()) {
            count++;
        }
    }
    return count;
}
const vector<BorrowedItem>& User::getTransactionHistory() const
{
    return transactionHistory; 
}
int User::FindActiveBorrowIndex(int bookID) const
{
    for (size_t i = 0; i < transactionHistory.size(); ++i)
    {
        if (transactionHistory[i].getBookID() == bookID && !transactionHistory[i].getIsReturned())
            return i;
    }
    return -1;
}

void User::SaveTransactionHistory() const
{
    if (getID() == 0) return;

    const string filename = "users_history.txt";
    
    // Load all existing histories
    map<int, vector<BorrowedItem>> allHistories = LoadAllTransactionHistories();
    
    // Update the history for this user
    allHistories[getID()] = transactionHistory;
    
    // Save all histories back to file
    ofstream outFile(filename);
    if (!outFile.is_open())
    {
        cerr << "Loi: Khong the mo tep de luu lich su giao dich: " << filename << endl;
        return;
    }

    for (const auto& userHistory : allHistories)
    {
        int userID = userHistory.first;
        const vector<BorrowedItem>& history = userHistory.second;
        
        outFile << "USER_" << userID << ":" << history.size() << "\n";
        
        for (const auto& item : history)
        {
            outFile << item.getBookID() << ","
                    << item.getBorrowDate() << ","
                    << item.getReturnDate() << ","
                    << item.getIsReturned() << "\n";
        }
    }

    outFile.close();
}

map<int, vector<BorrowedItem>> User::LoadAllTransactionHistories() const
{
    map<int, vector<BorrowedItem>> allHistories;
    const string filename = "users_history.txt";
    
    ifstream inFile(filename);
    if (!inFile.is_open())
    {
        return allHistories;
    }

    string line;
    while (getline(inFile, line))
    {
        if (line.find("USER_") == 0)
        {
            size_t colonPos = line.find(':');
            if (colonPos != string::npos)
            {
                string userIDStr = line.substr(5, colonPos - 5);
                string countStr = line.substr(colonPos + 1);
                
                int userID = 0;
                int count = 0;
                try {
                    userID = stoi(userIDStr);
                    count = stoi(countStr);
                } catch (...) {
                    continue;
                }
                
                vector<BorrowedItem> history;
                for (int i = 0; i < count && getline(inFile, line); ++i)
                {
                    stringstream ss(line);
                    string bookID_str, borrowDate_str, returnDate_str, isReturned_str;

                    if (getline(ss, bookID_str, ',') && 
                        getline(ss, borrowDate_str, ',') && 
                        getline(ss, returnDate_str, ',') && 
                        getline(ss, isReturned_str))
                    {
                        try
                        {
                            int bookID = stoi(bookID_str);
                            bool isReturned = (stoi(isReturned_str) != 0);

                            history.emplace_back(
                                bookID, 
                                borrowDate_str, 
                                returnDate_str, 
                                isReturned
                            );
                        }
                        catch (...) {}
                    }
                }
                
                allHistories[userID] = history;
            }
        }
    }

    inFile.close();
    return allHistories;
}

void User::LoadTransactionHistory()
{
    if (getID() == 0) {
        transactionHistory.clear();
        return;
    }

    map<int, vector<BorrowedItem>> allHistories = LoadAllTransactionHistories();
    auto it = allHistories.find(getID());
    if (it != allHistories.end())
    {
        transactionHistory = it->second;
    }
    else
    {
        transactionHistory.clear();
    }
}

User::User() : Person()
{
    transactionHistory = vector<BorrowedItem>();
}

User::User(int id, const char n[], const char d[], const char p[], const char e[], const char pw[])
    : Person(id, n, d, p, e, pw)
{
    transactionHistory = vector<BorrowedItem>();
    LoadTransactionHistory();
}

User::~User()
{
    SaveTransactionHistory();
}

bool User::BorrowBook(int bookID)
{
    int currentCount = getCurrentBorrowedCount();
    int activeIndex = FindActiveBorrowIndex(bookID);

    if (currentCount >= MAX_BORROWED_BOOKS)
    {
        cout << "Loi: Nguoi dung da muon toi da " << MAX_BORROWED_BOOKS << " cuon sach.\n";
        return false;
    }
    if (activeIndex != -1)
    {
        cout << "Loi: Nguoi dung da co cuon sach ID " << bookID << " nay trong danh sach dang muon.\n";
        return false;
    }

    char dateBuffer[20];
    Utils::GetCurrentDate(dateBuffer, sizeof(dateBuffer));

    transactionHistory.emplace_back(bookID, dateBuffer);
    SaveTransactionHistory();

    return true;
}

bool User::ReturnBook(int bookID)
{
    int index = FindActiveBorrowIndex(bookID);
    if (index == -1)
    {
        cout << "Loi: Nguoi dung khong muon cuon sach ID " << bookID << " nay.\n";
        return false;
    }

    char dateBuffer[20];
    Utils::GetCurrentDate(dateBuffer, sizeof(dateBuffer));

    transactionHistory[index].MarkAsReturned(dateBuffer);
    SaveTransactionHistory();
    return true;
}



void User::LoadUserByID(const string &id)
{
    ifstream file("Users.txt");
    if (!file.is_open())
    {
        cout << "Khong the mo file Users.txt\n";
        return;
    }

    string line;
    getline(file, line);

    while (getline(file, line))
    {
        // Strip BOM if present
        if (line.size() >= 3 && (unsigned char)line[0] == 0xEF && (unsigned char)line[1] == 0xBB && (unsigned char)line[2] == 0xBF) {
            line = line.substr(3);
        }

        stringstream ss(line);

        string userID, password_str, name_str, dob_str, phone_str, email_str;

        if (!getline(ss, userID, ',') || !getline(ss, password_str, ',') ||
            !getline(ss, name_str, ',') || !getline(ss, dob_str, ',') ||
            !getline(ss, phone_str, ',') || !getline(ss, email_str, ','))
            continue;

        if (userID == id)
        {
            setID(stoi(userID));
            setPassword(password_str.c_str());
            setName(name_str.c_str());
            setDate(dob_str.c_str());
            setPhone(phone_str.c_str());
            setEmail(email_str.c_str());

            LoadTransactionHistory();
            file.close();
            return;
        }
    }
    file.close();
}

void User::ShowTransactionHistory(BookManager& bookManager)
{
    if (transactionHistory.empty()) 
    {
        cout << Utils::YELLOW << "Nguoi dung nay chua co lich su giao dich." << Utils::RESET << "\n";
        return;
    }

    // Thiết lập độ rộng cột cho bảng
    const int BOOK_ID_WIDTH = 10;
    const int BOOK_TITLE_WIDTH = 35;
    const int BORROW_DATE_WIDTH = 15;
    const int RETURN_DATE_WIDTH = 15;
    const int DUE_DATE_WIDTH = 15;
    const int STATUS_WIDTH = 15;
    const vector<int> columnWidths = {BOOK_ID_WIDTH, BOOK_TITLE_WIDTH, BORROW_DATE_WIDTH, RETURN_DATE_WIDTH, DUE_DATE_WIDTH, STATUS_WIDTH};

    // cout << Utils::CYAN << Utils::BOLD;
    // Utils::PrintMenuBorder();
    // Utils::PrintMenuHeader("LICH SU GIAO DICH - " + string(getName()));
    // Utils::PrintMenuBorder();
    // cout << Utils::RESET;
    int tableWidth = 0;
    for (int width : columnWidths) {
        tableWidth += width + 1;
    }
    tableWidth -= 1;

    cout << Utils::CYAN << Utils::BOLD;
    Utils::PrintMenuBorder(tableWidth);
    Utils::PrintMenuHeader("LICH SU GIAO DICH - " + string(getName()), tableWidth);
    Utils::PrintMenuBorder(tableWidth);
    cout << Utils::RESET;
    // Table headers
    cout << Utils::BOLD << Utils::YELLOW
         << left << setw(BOOK_ID_WIDTH) << "ID Sach" << " "
         << left << setw(BOOK_TITLE_WIDTH) << "Ten Sach" << " "
         << left << setw(BORROW_DATE_WIDTH) << "Ngay muon" << " "
         << left << setw(RETURN_DATE_WIDTH) << "Ngay tra" << " "
         << left << setw(DUE_DATE_WIDTH) << "Han tra" << " "
         << left << setw(STATUS_WIDTH) << "Trang thai"
         << Utils::RESET << endl;

    // Separator line
    Utils::PrintTableLine(columnWidths);

    const int DEFAULT_BORROW_DAYS = 14; 
    char currentDateBuffer[20];
    Utils::GetCurrentDate(currentDateBuffer, sizeof(currentDateBuffer));
    tm current_tm = Utils::ParseDate(currentDateBuffer);

    for (const auto& item : transactionHistory)
    {
        int bookID = item.getBookID();
        string borrowDateStr = item.getBorrowDate();
        string returnDateStr = item.getReturnDate();
        Book* book = bookManager.GetBookByID(bookID);

        cout << left << setw(BOOK_ID_WIDTH) << bookID << " ";

        string bookTitle = (book) ? book->getTitle() : "Sach da bi xoa khoi he thong";
        cout << left << setw(BOOK_TITLE_WIDTH) << bookTitle << " "
             << left << setw(BORROW_DATE_WIDTH) << borrowDateStr << " "
             << left << setw(RETURN_DATE_WIDTH) << (item.getIsReturned() ? returnDateStr : "--") << " ";

        tm borrow_tm = Utils::ParseDate(borrowDateStr);
        tm dueDate_tm = Utils::AddDays(borrow_tm, 14);
        string dueDateStr = Utils::FormatDate(dueDate_tm);
        cout << left << setw(DUE_DATE_WIDTH) << dueDateStr << " ";

        string status;
        if (item.getIsReturned()) {
            status = "Da tra";
            tm return_tm = Utils::ParseDate(returnDateStr);
            if (Utils::CompareDates(return_tm, dueDate_tm) > 0) {
                 status += " (Tre)";
            }
        } else {
            status = "Dang muon";
            if (Utils::CompareDates(current_tm, dueDate_tm) > 0) {
                status = "QUA HAN";
            }
        }
        cout << left << setw(STATUS_WIDTH) << status;
        cout << endl;
    }

    // Bottom separator
    Utils::PrintTableLine(columnWidths);
}

void User::Show() const
{
    Person::Show();
    cout << "So sach dang muon: " << getCurrentBorrowedCount() << " / " << MAX_BORROWED_BOOKS << "\n";
}

void User::Menu(UserManager &manager, BookManager &bm)
{
    int choice;
    char input[10];

    do
    {
        cout << Utils::CYAN << Utils::BOLD;
        Utils::PrintMenuBorder();
        Utils::PrintMenuHeader(string("MENU DOC GIA - ") + getName());
        Utils::PrintMenuBorder();
        Utils::PrintMenuLine("");
        Utils::PrintMenuLine("1. Tim kiem sach");
        Utils::PrintMenuLine("2. Xem tat ca sach");
        Utils::PrintMenuLine("3. Xem lich su giao dich");
        Utils::PrintMenuLine("4. Xem thong tin ca nhan");
        Utils::PrintMenuLine("5. Chinh sua thong tin ca nhan");
        Utils::PrintMenuLine("6. Doi mat khau");
        Utils::PrintMenuLine("0. Dang xuat");
        Utils::PrintMenuLine("");
        Utils::PrintMenuBorder();
        cout << Utils::RESET;
        cout << Utils::CYAN << "Chon: " << Utils::RESET;

        if (cin.getline(input, sizeof(input)).fail())
        {
            cin.clear();
            cin.ignore(MAX_STREAM_SIZE_MANUAL, '\n');
            choice = -1;
        }
        else
            choice = Utils::StringToIntManual(input);

        switch (choice)
        {
        case 1:
        {
            int subChoice;
            char subInput[10];
            do
            {
                cout << Utils::CYAN << Utils::BOLD;
                Utils::PrintMenuBorder();
                Utils::PrintMenuHeader("MENU TIM KIEM SACH");
                Utils::PrintMenuBorder();
                Utils::PrintMenuLine("");
                Utils::PrintMenuLine("1. Theo ten");
                Utils::PrintMenuLine("2. Theo tac gia");
                Utils::PrintMenuLine("3. Theo the loai");
                Utils::PrintMenuLine("4. Theo ID");
                Utils::PrintMenuLine("0. Quay lai menu chinh");
                Utils::PrintMenuLine("");
                Utils::PrintMenuBorder();
                cout << Utils::RESET;
                cout << Utils::CYAN << "Chon: " << Utils::RESET;
                cin.getline(subInput, sizeof(subInput));
                subChoice = Utils::StringToIntManual(subInput);

                if (subChoice == 1)
                    bm.SearchBookByTitle();
                else if (subChoice == 2)
                {
                    char author[100];
                    cout << Utils::CYAN << "Nhap ten tac gia: " << Utils::RESET;
                    cin.getline(author, sizeof(author));
                    bm.SearchBookByAuthor(author);
                }
                else if (subChoice == 3)
                {
                    char category[100];
                    cout << Utils::CYAN << "Nhap the loai: " << Utils::RESET;
                    cin.getline(category, sizeof(category));
                    bm.SearchBookByCategory(category);
                }
                else if (subChoice == 4)
                {
                    char idStr[20];
                    int bookID;
                    cout << Utils::CYAN << "Nhap ID cuon sach: " << Utils::RESET;
                    cin.getline(idStr, sizeof(idStr));
                    bookID = Utils::CharArrayToIntManual(idStr);
                    const Book *bookPtr = bm.GetBookByID(bookID);
                    if (bookPtr)
                        bookPtr->Show();
                    else
                        cout << "Khong tim thay sach.\n";
                }

            } while (subChoice != 0);
            break;
        }

        case 2:
            bm.ShowAllBooks();
            Utils::AskReturnToMenu();
            break;

        case 3:
            ShowTransactionHistory(bm);
            Utils::AskReturnToMenu();
            break;

        case 4:
            Show();
            Utils::AskReturnToMenu();
            break;

        case 5:
        {
            int id;
            cout << Utils::CYAN << "Nhap lai ID de xac nhan: " << Utils::RESET;
            cin >> id;
            cin.ignore(MAX_STREAM_SIZE_MANUAL, '\n');

            if (id == getID())
            {
                manager.UpdateByID(id);
            }
            else
                cout << "ID khong dung.\n";

            Utils::AskReturnToMenu();
            break;
        }

        case 6:
            ChangePassword();
            Utils::AskReturnToMenu();
            break;

        case 0:
            cout << "Dang xuat.\n";
            break;

        default:
            cout << "Lua chon khong hop le!\n";
            break;
        }

    } while (choice != 0);
}
