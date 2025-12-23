#include "BorrowManager.h"
#include "Utils.h"


void BorrowManager::ShowActiveAndOverdueBorrows(UserManager& userManager, BookManager& bookManager) {
    const Person* allUsers = userManager.GetAllUsers();
    int userCount = userManager.GetUserCount();

    if (userCount == 0) {
        cout << Utils::YELLOW << "Khong co doc gia nao trong he thong." << Utils::RESET << "\n";
        return;
    }

    const int BORROW_ID_WIDTH = 8;
    const int USER_ID_WIDTH = 10;
    const int USER_NAME_WIDTH = 20;
    const int BOOK_ID_WIDTH = 10;
    const int BOOK_TITLE_WIDTH = 35;
    const int BORROW_DATE_WIDTH = 12;
    const int DUE_DATE_WIDTH = 12;
    const int STATUS_WIDTH = 15;

    const vector<int> columnWidths = {
        BORROW_ID_WIDTH, USER_ID_WIDTH, USER_NAME_WIDTH,
        BOOK_ID_WIDTH, BOOK_TITLE_WIDTH,
        BORROW_DATE_WIDTH, DUE_DATE_WIDTH, STATUS_WIDTH
    };

    int tableWidth = 0;
    for (int w : columnWidths) tableWidth += w + 1;
    tableWidth -= 1;

    cout << Utils::CYAN << Utils::BOLD;
    Utils::PrintMenuBorder(tableWidth);
    Utils::PrintMenuHeader("DANH SACH SACH DANG MUON & QUA HAN", tableWidth);
    Utils::PrintMenuBorder(tableWidth);
    cout << Utils::RESET;

    cout << Utils::BOLD << Utils::YELLOW
         << left << setw(BORROW_ID_WIDTH) << "ID" << " "
         << left << setw(USER_ID_WIDTH) << "ID User" << " "
         << left << setw(USER_NAME_WIDTH) << "Ten User" << " "
         << left << setw(BOOK_ID_WIDTH) << "ID Sach" << " "
         << left << setw(BOOK_TITLE_WIDTH) << "Ten Sach" << " "
         << left << setw(BORROW_DATE_WIDTH) << "Ngay muon" << " "
         << left << setw(DUE_DATE_WIDTH) << "Han tra" << " "
         << left << setw(STATUS_WIDTH) << "Trang thai"
         << Utils::RESET << endl;

    Utils::PrintTableLine(columnWidths);

    const int DEFAULT_BORROW_DAYS = 14;
    char curDate[20];
    Utils::GetCurrentDate(curDate, sizeof(curDate));
    tm current_tm = Utils::ParseDate(curDate);

    int active = 0, overdue = 0, id = 1;

    for (int i = 0; i < userCount; i++) {
        User user;
        user.LoadUserByID(to_string(allUsers[i].getID()));

        for (const auto& item : user.getTransactionHistory()) {
            if (item.getIsReturned()) continue;

            tm borrow_tm = Utils::ParseDate(item.getBorrowDate());
            tm due_tm = Utils::AddDays(borrow_tm, DEFAULT_BORROW_DAYS);
            bool isOverdue = Utils::CompareDates(current_tm, due_tm) > 0;

            Book* book = bookManager.GetBookByID(item.getBookID());
            string title = book ? book->getTitle() : "Sach da bi xoa";
            if (title.length() > 34) title = title.substr(0, 31) + "...";

            cout << left << setw(BORROW_ID_WIDTH) << id++ << " "
                 << setw(USER_ID_WIDTH) << allUsers[i].getID() << " "
                 << setw(USER_NAME_WIDTH) << allUsers[i].getName() << " "
                 << setw(BOOK_ID_WIDTH) << item.getBookID() << " "
                 << setw(BOOK_TITLE_WIDTH) << title << " "
                 << setw(BORROW_DATE_WIDTH) << item.getBorrowDate() << " "
                 << setw(DUE_DATE_WIDTH) << Utils::FormatDate(due_tm) << " "
                 << setw(STATUS_WIDTH) << (isOverdue ? "QUA HAN" : "Dang muon")
                 << endl;

            active++;
            if (isOverdue) overdue++;
        }
    }

    Utils::PrintTableLine(columnWidths);

    const int SUM_WIDTH = 45;
    cout << Utils::CYAN << Utils::BOLD;
    Utils::PrintMenuBorder(SUM_WIDTH);
    Utils::PrintMenuHeader("TONG QUAN MUON SACH", SUM_WIDTH);
    Utils::PrintMenuBorder(SUM_WIDTH);
    cout << Utils::RESET;

    Utils::PrintInfoRow("So sach dang muon", to_string(active), SUM_WIDTH);
    Utils::PrintInfoRow("So sach qua han", to_string(overdue), SUM_WIDTH);

    cout << Utils::CYAN << Utils::BOLD;
    Utils::PrintMenuBorder(SUM_WIDTH);
    cout << Utils::RESET;
}

vector<TransactionRecord> BorrowManager::LoadAllTransactions() {
    vector<TransactionRecord> transactions;

    // Load borrow records
    ifstream borrowFile("BorrowRecords.txt");
    if (borrowFile.is_open()) {
        string line;
        TransactionRecord current;
        bool isNew = false;
        while (getline(borrowFile, line)) {
            if (line.find("=== THONG TIN MUON SACH ===") != string::npos) {
                if (isNew) {
                    current.isReturned = false;
                    current.returnDate = "";
                    transactions.push_back(current);
                }
                current = TransactionRecord();
                isNew = true;
            } else if (isNew) {
                size_t colon_pos = line.find(':');
                if (colon_pos != string::npos) {
                    string key = line.substr(0, colon_pos);
                    string value = line.substr(colon_pos + 1);
                    value.erase(0, value.find_first_not_of(" \t\r\n"));
                    value.erase(value.find_last_not_of(" \t\r\n") + 1);
                    if (key.find("Thoi gian ghi nhan") != string::npos) {
                        current.timeStamp = value;
                    } else if (key.find("ID nguoi dung") != string::npos) {
                        current.userID = stoi(value);
                    } else if (key.find("Ten nguoi dung") != string::npos) {
                        current.userName = value;
                    } else if (key.find("ID sach") != string::npos) {
                        current.bookID = stoi(value);
                    } else if (key.find("Ten sach") != string::npos) {
                        current.bookTitle = value;
                    } else if (key.find("Ngay muon") != string::npos) {
                        current.borrowDate = value;
                    }
                }
            } else if (line.find("----------------------------------------") != string::npos) {
                if (isNew) {
                    current.isReturned = false;
                    current.returnDate = "";
                    transactions.push_back(current);
                }
                isNew = false;
            }
        }
        if (isNew) {
            current.isReturned = false;
            current.returnDate = "";
            transactions.push_back(current);
        }
        borrowFile.close();
    }

    // Load return records and update transactions
    ifstream returnFile("ReturnRecords.txt");
    if (returnFile.is_open()) {
        string line;
        TransactionRecord currentReturn;
        bool isNew = false;
        while (getline(returnFile, line)) {
            if (line.find("=== THONG TIN TRA SACH ===") != string::npos) {
                if (isNew) {
                    // Find matching transaction
                    for (auto& trans : transactions) {
                        if (trans.userID == currentReturn.userID && trans.bookID == currentReturn.bookID && !trans.isReturned) {
                            trans.isReturned = true;
                            trans.returnDate = currentReturn.returnDate;
                            break;
                        }
                    }
                }
                currentReturn = TransactionRecord();
                isNew = true;
            } else if (isNew) {
                size_t colon_pos = line.find(':');
                if (colon_pos != string::npos) {
                    string key = line.substr(0, colon_pos);
                    string value = line.substr(colon_pos + 1);
                    value.erase(0, value.find_first_not_of(" \t\r\n"));
                    value.erase(value.find_last_not_of(" \t\r\n") + 1);
                    if (key.find("ID nguoi dung") != string::npos) {
                        currentReturn.userID = stoi(value);
                    } else if (key.find("ID sach") != string::npos) {
                        currentReturn.bookID = stoi(value);
                    } else if (key.find("Ngay tra") != string::npos) {
                        currentReturn.returnDate = value;
                    }
                }
            } else if (line.find("----------------------------------------") != string::npos) {
                if (isNew) {
                    // Find matching transaction
                    for (auto& trans : transactions) {
                        if (trans.userID == currentReturn.userID && trans.bookID == currentReturn.bookID && !trans.isReturned) {
                            trans.isReturned = true;
                            trans.returnDate = currentReturn.returnDate;
                            break;
                        }
                    }
                }
                isNew = false;
            }
        }
        if (isNew) {
            // Find matching transaction
            for (auto& trans : transactions) {
                if (trans.userID == currentReturn.userID && trans.bookID == currentReturn.bookID && !trans.isReturned) {
                    trans.isReturned = true;
                    trans.returnDate = currentReturn.returnDate;
                    break;
                }
            }
        }
        returnFile.close();
    }

    return transactions;
}

void BorrowManager::ShowAllUsersTransactionHistory(UserManager& userManager, BookManager& bookManager) {
   vector<TransactionRecord> transactions = LoadAllTransactions();

    if (transactions.empty()) {
        cout << Utils::YELLOW << "Khong co giao dich nao trong he thong." << Utils::RESET << "\n";
        return;
    }

    const vector<int> widths = {8, 10, 20, 10, 35, 12, 12, 12, 15};
    int tableWidth = 0;
    for (int w : widths) tableWidth += w + 1;
    tableWidth--;

    cout << Utils::CYAN << Utils::BOLD;
    Utils::PrintMenuBorder(tableWidth);
    Utils::PrintMenuHeader("TAT CA LICH SU GIAO DICH", tableWidth);
    Utils::PrintMenuBorder(tableWidth);
    cout << Utils::RESET;

    Utils::PrintTableLine(widths);

    int id = 1;
    for (const auto& t : transactions) {
        Book* book = bookManager.GetBookByID(t.bookID);
        string title = book ? book->getTitle() : t.bookTitle;
        if (title.length() > 34) title = title.substr(0, 31) + "...";

        cout << left << setw(8) << id++
             << setw(10) << t.userID
             << setw(20) << t.userName
             << setw(10) << t.bookID
             << setw(35) << title
             << setw(12) << t.borrowDate
             << setw(12) << (t.isReturned ? t.returnDate : "--")
             << setw(12) << "--"
             << setw(15) << (t.isReturned ? "Da tra" : "Dang muon")
             << endl;
    }

    Utils::PrintTableLine(widths);

    const int SUM_WIDTH = 55;
    cout << Utils::CYAN << Utils::BOLD;
    Utils::PrintMenuBorder(SUM_WIDTH);
    Utils::PrintMenuHeader("TONG QUAN HE THONG", SUM_WIDTH);
    Utils::PrintMenuBorder(SUM_WIDTH);
    cout << Utils::RESET;

    Utils::PrintInfoRow("Tong so doc gia", to_string(userManager.GetUserCount()), SUM_WIDTH+2);
    Utils::PrintInfoRow("Tong so giao dich", to_string(transactions.size()), SUM_WIDTH+2);

    cout << Utils::CYAN << Utils::BOLD;
    Utils::PrintMenuBorder(SUM_WIDTH);
    cout << Utils::RESET;
}

void BorrowManager::LogBorrowRecord(int userID, const string &userName,
                                    int bookID, const string &bookTitle,
                                    const char *borrowDate)
{
    ofstream outFile("BorrowRecords.txt", ios::app);

    if (!outFile.is_open())
    {
        cout << "LOI: Khong the mo hoac tao file BorrowRecords.txt.\n";
        return;
    }

    time_t now = time(0);
    tm *localTime = localtime(&now);
    char timeBuffer[80];
    strftime(timeBuffer, sizeof(timeBuffer), "%d/%m/%Y %H:%M:%S", localTime);

    outFile << "=== THONG TIN MUON SACH ===" << endl;
    outFile << "Thoi gian ghi nhan: " << timeBuffer << endl;
    outFile << "ID nguoi dung: " << userID << endl;
    outFile << "Ten nguoi dung: " << userName << endl;
    outFile << "ID sach: " << bookID << endl;
    outFile << "Ten sach: " << bookTitle << endl;
    outFile << "Ngay muon: " << borrowDate << endl;
    outFile << "----------------------------------------" << endl;
    outFile << endl;

    outFile.close();

    cout << "Da ghi thong tin muon sach vao BorrowRecords.txt.\n";
}

void BorrowManager::LogReturnRecord(int userID, const string &userName,
                                    int bookID, const string &bookTitle,
                                    const string &returnDate)
{
    const string fileName = "ReturnRecords.txt";

    ofstream outFile(fileName, ios::app);

    if (!outFile.is_open())
    {
        cout << "Loi: Khong the mo file " << fileName << " de ghi thong tin tra sach!\n";
        return;
    }

    time_t now = time(0);
    tm *localTime = localtime(&now);
    char timeBuffer[80];
    strftime(timeBuffer, sizeof(timeBuffer), "%d/%m/%Y %H:%M:%S", localTime);

    outFile << "=== THONG TIN TRA SACH ===" << endl;
    outFile << "Thoi gian ghi nhan: " << timeBuffer << endl;
    outFile << "ID nguoi dung: " << userID << endl;
    outFile << "Ten nguoi dung: " << userName << endl;
    outFile << "ID sach: " << bookID << endl;
    outFile << "Ten sach: " << bookTitle << endl;
    outFile << "Ngay tra: " << returnDate << endl;
    outFile << "----------------------------------------" << endl;
    outFile << endl;

    outFile.close();

    cout << "Da ghi thong tin tra sach vao file: " << fileName << "\n";
}

void BorrowManager::HandleBorrowBook(UserManager& um, BookManager& bm)
{
    cout << Utils::CYAN << Utils::BOLD;
    Utils::PrintMenuBorder();
    Utils::PrintMenuHeader("XU LY MUON SACH");
    Utils::PrintMenuBorder();
    cout << Utils::RESET;

    int userID = 0;

    cout << Utils::CYAN << "Nhap ID nguoi dung: " << Utils::RESET;
    if (!(cin >> userID))
    {
        cout << "Loi: ID nguoi dung khong hop le.\n";
        cin.clear();
        cin.ignore(100, '\n');
        return;
    }
    cin.ignore(100, '\n');

    User user;
    user.LoadUserByID(to_string(userID));
    
    if (user.getID() == 0) {
        cout << Utils::RED << "Loi: Khong tim thay nguoi dung voi ID: " << userID << "\n" << Utils::RESET;
        return;
    }

    char continueChoice;
    do
    {
        int bookID = 0;

        cout << Utils::CYAN << "Nhap ID sach muon muon: " << Utils::RESET;
        if (!(cin >> bookID))
        {
            cout << "Loi: ID sach khong hop le.\n";
            cin.clear();
            cin.ignore(100, '\n');
            break;
        }
        cin.ignore(100, '\n');

        Book *book = bm.GetBookByID(bookID);
        if (!book)
        {
            cout << Utils::RED << "Loi: Khong tim thay sach voi ID: " << bookID << "\n" << Utils::RESET;
            continue;
        }

        if (book->getQuantity() <= 0)
        {
            cout << "Loi: Sach '" << book->getTitle() << "' da het trong kho.\n";
            continue;
        }

        if (user.BorrowBook(bookID))
        {
            book->setQuantity(book->getQuantity() - 1);
            bm.SaveBooksToFile();

            char borrowDate[20];
            Utils::GetCurrentDate(borrowDate, sizeof(borrowDate));

            string currentUserName = user.getName();
            string currentBookTitle = book->getTitle();

            LogBorrowRecord(
                userID,
                currentUserName,
                bookID,
                currentBookTitle,
                borrowDate);

            cout << Utils::GREEN << Utils::BOLD << "MUON SACH THANH CONG!\n" << Utils::RESET;
            cout << Utils::CYAN << "Sach: '" << book->getTitle() << "'\n";
            cout << Utils::CYAN << "Ngay muon: " << borrowDate << "\n";
            cout << Utils::CYAN << "So luong con lai: " << book->getQuantity() << "\n" << Utils::RESET;
        }
        else
        {
            cout << Utils::RED << "Muon sach that bai!\n" << Utils::RESET;
        }

        cout << Utils::CYAN << "Ban co muon tiep tuc muon sach khac cho nguoi dung nay? (y/n): " << Utils::RESET;
        cin >> continueChoice;
        cin.ignore(100, '\n');
        continueChoice = tolower(continueChoice);

    } while (continueChoice == 'y');
}

void BorrowManager::HandleReturnBook(UserManager& um, BookManager& bm)
{
    cout << Utils::CYAN << Utils::BOLD;
    Utils::PrintMenuBorder();
    Utils::PrintMenuHeader("XU LY TRA SACH");
    Utils::PrintMenuBorder();
    cout << Utils::RESET;
    int userID = 0;

    cout << Utils::CYAN << "Nhap ID nguoi dung: " << Utils::RESET;
    if (!(cin >> userID))
    {
        cout << Utils::RED << "Loi: ID nguoi dung khong hop le.\n" << Utils::RESET;
        cin.clear();
        cin.ignore(100, '\n');
        return;
    }
    cin.ignore(100, '\n');

    User tempUser;
    char bufferID[20];
    Utils::IntToStringManual(userID, bufferID, sizeof(bufferID));
    tempUser.LoadUserByID(bufferID);

    if (tempUser.getID() == 0)
    {
        cout << Utils::RED << "Loi: Khong tim thay nguoi dung voi ID: " << userID << "\n" << Utils::RESET;
        return;
    }

    char continueChoice;
    do
    {
        int bookID = 0;

        cout << Utils::CYAN << "Nhap ID sach muon tra: " << Utils::RESET;
        if (!(cin >> bookID))
        {
            cout << Utils::RED << "Loi: ID sach khong hop le.\n" << Utils::RESET;
            cin.clear();
            cin.ignore(100, '\n');
            break;
        }
        cin.ignore(100, '\n');

        char returnDate[20];
        Utils::GetCurrentDate(returnDate, sizeof(returnDate));
        string returnDateStr(returnDate);

        Book *book = bm.GetBookByID(bookID);
        string userName = tempUser.getName();
        string bookTitle = book ? book->getTitle() : "Khong xac dinh";

        if (tempUser.ReturnBook(bookID))
        {
            if (book)
            {
                book->setQuantity(book->getQuantity() + 1);
                cout << Utils::GREEN << Utils::BOLD << "TRA SACH THANH CONG!\n" << Utils::RESET;
                cout << Utils::CYAN << "Sach: '" << book->getTitle() << "'\n";
                cout << Utils::CYAN << "Ngay tra: " << returnDateStr << "\n";
                cout << Utils::CYAN << "So luong hien tai: " << book->getQuantity() << "\n" << Utils::RESET;

                bm.SaveBooksToFile();
                LogReturnRecord(userID, userName, bookID, bookTitle, returnDateStr);
            }
            else
            {
                cout << Utils::GREEN << Utils::BOLD << "TRA SACH THANH CONG.\n" << Utils::RESET;
                cout << Utils::CYAN << "Ngay tra: " << returnDateStr << "\n" << Utils::RESET;
                LogReturnRecord(userID, userName, bookID, "Unknown Book (Deleted)", returnDateStr);
            }
        }
        else
        {
            cout << Utils::RED << "Loi: Nguoi dung ID " << userID << " khong muon sach ID " << bookID << "\n" << Utils::RESET;
        }

        cout << Utils::CYAN << "Ban co muon tiep tuc tra sach khac cho nguoi dung nay? (y/n): " << Utils::RESET;
        cin >> continueChoice;
        cin.ignore(100, '\n');
        continueChoice = tolower(continueChoice);

    } while (continueChoice == 'y');
}

void BorrowManager::ShowRawBorrowHistory(const string& filename, const string& title)
{
    cout << "\n============================================\n";
    cout << "       " << title << "       \n";
    cout << "============================================\n";

    ifstream inFile(filename);

    if (!inFile.is_open())
    {
        cout << Utils::YELLOW << "Thong bao: Khong co du lieu (File " << filename << " khong ton tai)." << Utils::RESET << "\n";
        return;
    }

    string line;
    bool isEmpty = true;

    while (getline(inFile, line))
    {
        cout << line << endl;
        isEmpty = false;
    }

    if (isEmpty) {
        cout << "Thong bao: File lich su hien dang trong.\n";
    }

    inFile.close();

    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

void BorrowManager::ShowBorrowRecords()
{
    ifstream inFile("BorrowRecords.txt");

    if (!inFile.is_open())
    {
        cout << Utils::YELLOW
             << "Thong bao: Khong co du lieu muon sach (BorrowRecords.txt khong ton tai)."
             << Utils::RESET << "\n";
        return;
    }

    vector<BorrowRecordData> records;
    string line;
    BorrowRecordData current;
    bool isNew = false;

    while (getline(inFile, line))
    {
        if (line.find("=== THONG TIN MUON SACH ===") != string::npos)
        {
            if (isNew)
                records.push_back(current);

            current = BorrowRecordData();
            isNew = true;
        }
        else if (isNew)
        {
            size_t pos = line.find(':');
            if (pos != string::npos)
            {
                string key = line.substr(0, pos);
                string value = line.substr(pos + 1);
                value.erase(0, value.find_first_not_of(" \t\r\n"));
                value.erase(value.find_last_not_of(" \t\r\n") + 1);

                if (key.find("Thoi gian ghi nhan") != string::npos)
                    current.timeStamp = value;
                else if (key.find("ID nguoi dung") != string::npos)
                    current.userID = stoi(value);
                else if (key.find("Ten nguoi dung") != string::npos)
                    current.userName = value;
                else if (key.find("ID sach") != string::npos)
                    current.bookID = stoi(value);
                else if (key.find("Ten sach") != string::npos)
                    current.bookTitle = value;
                else if (key.find("Ngay muon") != string::npos)
                    current.borrowDate = value;
            }
        }
        else if (line.find("----------------------------------------") != string::npos)
        {
            if (isNew)
                records.push_back(current);
            isNew = false;
        }
    }
    if (isNew)
        records.push_back(current);

    inFile.close();

    if (records.empty())
    {
        cout << Utils::YELLOW << "Khong co ban ghi muon sach nao." << Utils::RESET << "\n";
        return;
    }

    /* ================= GIAO DIEN ================= */

    const int TIME_WIDTH = 22;
    const int USER_ID_WIDTH = 10;
    const int USER_NAME_WIDTH = 22;
    const int BOOK_ID_WIDTH = 8;
    const int BOOK_TITLE_WIDTH = 38;
    const int DATE_WIDTH = 12;
    const int STATUS_WIDTH = 12;

    const vector<int> columnWidths = {
        TIME_WIDTH,
        USER_ID_WIDTH,
        USER_NAME_WIDTH,
        BOOK_ID_WIDTH,
        BOOK_TITLE_WIDTH,
        DATE_WIDTH,
        STATUS_WIDTH
    };

    int tableWidth = 0;
    for (int w : columnWidths) tableWidth += w + 1;
    tableWidth--;

    cout << Utils::CYAN << Utils::BOLD;
    Utils::PrintMenuBorder(tableWidth);
    Utils::PrintMenuHeader("THONG KE DANH SACH MUON SACH", tableWidth);
    Utils::PrintMenuBorder(tableWidth);
    cout << Utils::RESET;

    // Header table
    cout << Utils::BOLD << Utils::YELLOW
         << left << setw(TIME_WIDTH) << "Thoi gian"
         << setw(USER_ID_WIDTH) << "ID User"
         << setw(USER_NAME_WIDTH) << "Ten User"
         << setw(BOOK_ID_WIDTH) << "ID Sach"
         << setw(BOOK_TITLE_WIDTH) << "Ten Sach"
         << setw(DATE_WIDTH) << "Ngay muon"
         << setw(STATUS_WIDTH) << "Trang thai"
         << Utils::RESET << endl;

    Utils::PrintTableLine(columnWidths);

    for (const auto& rec : records)
    {
        string title = rec.bookTitle;
        if (title.length() > BOOK_TITLE_WIDTH - 3)
            title = title.substr(0, BOOK_TITLE_WIDTH - 6) + "...";

        cout << left << setw(TIME_WIDTH) << rec.timeStamp
             << setw(USER_ID_WIDTH) << rec.userID
             << setw(USER_NAME_WIDTH) << rec.userName
             << setw(BOOK_ID_WIDTH) << rec.bookID
             << setw(BOOK_TITLE_WIDTH) << title
             << setw(DATE_WIDTH) << rec.borrowDate
             << setw(STATUS_WIDTH) << "Da muon"
             << endl;
    }

    Utils::PrintTableLine(columnWidths);

    /* ================= TONG QUAN ================= */

    const int SUMMARY_WIDTH = 50;
    cout << Utils::CYAN << Utils::BOLD;
    Utils::PrintMenuBorder(SUMMARY_WIDTH);
    Utils::PrintMenuHeader("TONG QUAN MUON SACH", SUMMARY_WIDTH);
    Utils::PrintMenuBorder(SUMMARY_WIDTH);
    cout << Utils::RESET;

    Utils::PrintInfoRow("Tong so giao dich muon",
                        to_string(records.size()), SUMMARY_WIDTH);

    cout << Utils::CYAN << Utils::BOLD;
    Utils::PrintMenuBorder(SUMMARY_WIDTH);
    cout << Utils::RESET;

    cout << Utils::CYAN << "\nNhan Enter de quay lai..." << Utils::RESET;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}
