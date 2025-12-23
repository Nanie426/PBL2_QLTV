#include "UserManager.h"
#include "Utils.h"
#include <cstring>

string UserManager::GenerateNextUserID()
{
    int newID = nextAvailableID;

    stringstream ss;
    ss << setfill('0') << newID;

    nextAvailableID++;
    return ss.str();
}

Person* UserManager::GetUserByID(int userID)
{
    for (int i = 0; i < userCount; ++i)
        if (users[i].getID() == userID)
            return &users[i];
    return nullptr;
}

UserManager::UserManager()
{
    userCount = 0;
    Utils::CopyStringManual(userFilePath, "users.txt", sizeof(userFilePath));
    LoadUsersFromFile();
}

UserManager::~UserManager()
{
    SaveUsersToFile();
}

bool UserManager::IsUserIDExist(int id)
{
    for (int i = 0; i < userCount; ++i)
        if (users[i].getID() == id)
            return true;
    return false;
}

bool UserManager::IsValidStudentID(const char idStr[])
{
    int len = strlen(idStr);
    if (len != 9) return false;

    for (int i = 0; i < len; ++i)
        if (!isdigit(idStr[i])) return false;

    return true;
}

void UserManager::LoadUsersFromFile()
{
    ifstream inFile(userFilePath);
    if (!inFile.is_open()) return;

    char line[300];
    while (inFile.getline(line, sizeof(line)))
    {
        // Strip BOM if present
        if (strlen(line) >= 3 && (unsigned char)line[0] == 0xEF && (unsigned char)line[1] == 0xBB && (unsigned char)line[2] == 0xBF) {
            memmove(line, line + 3, strlen(line) - 3 + 1);
        }

        char fields[6][100];
        int fieldCount = Utils::SplitLineManual(line, fields, 6, ',');

        for (int i = 0; i < fieldCount; i++) {
            int len = strlen(fields[i]);
            while (len > 0 && (fields[i][len - 1] == '\n' ||
                               fields[i][len - 1] == '\r' ||
                               fields[i][len - 1] == ' '))
            {
                fields[i][--len] = '\0';
            }
        }

        int id = Utils::CharArrayToIntManual(fields[0]);
        users[userCount].setID(id);
        users[userCount].setPassword(fields[1]);
        users[userCount].setName(fields[2]);
        users[userCount].setDate(fields[3]);
        users[userCount].setPhone(fields[4]);
        users[userCount].setEmail(fields[5]);
        userCount++;
    }

    inFile.close();

    if (userCount > 0) {
        int maxID = 0;
        for (int i = 0; i < userCount; ++i)
            if (users[i].getID() > maxID)
                maxID = users[i].getID();
        nextAvailableID = maxID + 1;
    }
    else {
        nextAvailableID = 1;
    }
}

void UserManager::SaveUsersToFile() const
{
    ofstream outFile(userFilePath);

    for (int i = 0; i < userCount; ++i)
    {
        outFile << users[i].getID() << ","
                << users[i].getPassword() << ","
                << users[i].getName() << ","
                << users[i].getDate() << ","
                << users[i].getPhone() << ","
                << users[i].getEmail() << "\n";
    }

    outFile.close();
}

void UserManager::AddUser()
{
    char choice;
    do {
        cout << Utils::GREEN << Utils::BOLD;
        Utils::PrintMenuBorder();
        Utils::PrintMenuHeader("THEM NGUOI DUNG MOI");
        Utils::PrintMenuBorder();
        cout << Utils::RESET;

        string newIDStr = GenerateNextUserID();
        int tempID = Utils::StringToIntManual(newIDStr.c_str());

        cout << Utils::CYAN << "ID nguoi dung moi: " << Utils::YELLOW << newIDStr << Utils::RESET << "\n\n";

        users[userCount].setID(tempID);

        cout << Utils::BOLD << "Hay nhap thong tin:" << Utils::RESET << "\n";
        cout << Utils::CYAN << string(59, '-') << Utils::RESET << "\n";
        users[userCount].InputUserDetails();

        string lastName = users[userCount].getLastName();
        string dob = users[userCount].getDate();
        string newPassword = Utils::GeneratePassword(lastName, dob);

        users[userCount].setPassword(newPassword.c_str());

        cout << Utils::GREEN << Utils::BOLD;
        Utils::PrintMenuBorder();
        Utils::PrintMenuLine("THEM NGUOI DUNG THANH CONG!");
        Utils::PrintMenuBorder();
        cout << Utils::RESET;

        cout << Utils::YELLOW << "Mat khau duoc tao: " << Utils::RED << Utils::BOLD << newPassword << Utils::RESET << "\n\n";
        // Show user information with nice formatting
        cout << Utils::CYAN << Utils::BOLD << "THONG TIN NGUOI DUNG:" << Utils::RESET << "\n";
        cout << Utils::CYAN << string(59, '-') << Utils::RESET << "\n";
        cout << Utils::WHITE << "ID: " << Utils::YELLOW << users[userCount].getID() << Utils::RESET << "\n";
        cout << Utils::WHITE << "Ho Ten: " << Utils::YELLOW << users[userCount].getName() << Utils::RESET << "\n";
        cout << Utils::WHITE << "Ngay Sinh: " << Utils::YELLOW << users[userCount].getDate() << Utils::RESET << "\n";
        cout << Utils::WHITE << "So Dien Thoai: " << Utils::YELLOW << users[userCount].getPhone() << Utils::RESET << "\n";
        cout << Utils::WHITE << "Email: " << Utils::YELLOW << users[userCount].getEmail() << Utils::RESET << "\n";
        cout << Utils::CYAN << string(59, '-') << Utils::RESET << "\n\n";

        userCount++;
        SaveUsersToFile();

        cout << Utils::CYAN << "Ban co muon them nguoi dung moi khac khong? (Y/N): " << Utils::RESET;
        cin >> choice;
        cin.ignore(100, '\n');

        choice = toupper(choice);

    } while (choice == 'Y');
}

void UserManager::ShowAllUsers() const
{
    if (userCount == 0)
    {
        cout << Utils::YELLOW << "Khong tim thay nguoi dung nao trong he thong!" << Utils::RESET << "\n";
        return;
    }

    const int ID_WIDTH = 10;
    const int NAME_WIDTH = 30;
    const int DATE_WIDTH = 12;
    const int PHONE_WIDTH = 12;
    const int EMAIL_WIDTH = 30;
    const vector<int> columnWidths = {ID_WIDTH, NAME_WIDTH, DATE_WIDTH, PHONE_WIDTH, EMAIL_WIDTH};

    // Tính tổng độ rộng của bảng (giống như PrintTableLine)
    int tableWidth = 0;
    for (int width : columnWidths) {
        tableWidth += width + 1;
    }
    tableWidth -= 1;

    cout << Utils::CYAN << Utils::BOLD;
    Utils::PrintMenuBorder(tableWidth);
    Utils::PrintMenuHeader("Tat ca nguoi dung (" + to_string(userCount) + " nguoi dung)", tableWidth);
    Utils::PrintMenuBorder(tableWidth);
    cout << Utils::RESET;

    // Table headers
    cout << Utils::BOLD << Utils::YELLOW
         << left << setw(ID_WIDTH) << "ID" << " "
         << left << setw(NAME_WIDTH) << "Ten" << " "
         << left << setw(DATE_WIDTH) << "Ngay sinh" << " "
         << left << setw(PHONE_WIDTH) << "So dien thoai" << " "
         << left << setw(EMAIL_WIDTH) << "Email"
         << Utils::RESET << endl;

    Utils::PrintTableLine(columnWidths);

    for (int i = 0; i < userCount; ++i)
    {
        const Person& user = users[i];
        cout << left << setw(ID_WIDTH) << user.getID() << " "
             << left << setw(NAME_WIDTH) << user.getName() << " "
             << left << setw(DATE_WIDTH) << user.getDate() << " "
             << left << setw(PHONE_WIDTH) << user.getPhone() << " "
             << left << setw(EMAIL_WIDTH) << user.getEmail()
             << endl;
    }

    Utils::PrintTableLine(columnWidths);
}

void UserManager::DeleteUserByID(int id)
{
    char choice;
    do {
        int pos = -1;
        for (int i = 0; i < userCount; ++i)
            if (users[i].getID() == id) { pos = i; break; }

        if (pos == -1)
        {
            cout << Utils::RED << "Khong tim thay nguoi dung co ID " << id << Utils::RESET << endl;
        }
        else
        {
            cout << Utils::CYAN << Utils::BOLD;
            Utils::PrintMenuBorder();
            Utils::PrintMenuHeader("XAC NHAN XOA NGUOI DUNG");
            Utils::PrintMenuBorder();
            cout << Utils::RESET;

            cout << Utils::YELLOW << "Ban co chac chan muon xoa nguoi dung sau khong?" << Utils::RESET << "\n\n";
            users[pos].Show();

            Utils::PrintMenuBorder();
            cout << Utils::RESET;

            cout << Utils::CYAN << "Ban co chac chan muon xoa nguoi nay khong? (Y/N): " << Utils::RESET;

            char confirm;
            cin >> confirm;
            cin.ignore(100, '\n');
            confirm = toupper(confirm);

            if (confirm == 'Y')
            {
                for (int i = pos; i < userCount - 1; ++i)
                    users[i] = users[i + 1];

                userCount--;
                SaveUsersToFile();

                cout << Utils::GREEN << Utils::BOLD << "XOA NGUOI DUNG THANH CONG!" << Utils::RESET << endl;

                cout << Utils::RESET << endl;
            }
            else
            {
                cout << Utils::YELLOW << "Da huy thao tac xoa." << Utils::RESET << endl;
            }
        }

        cout << Utils::CYAN << "Ban co muon xoa nguoi dung khac khong? (Y/N): " << Utils::RESET;
        cin >> choice;
        cin.ignore(100, '\n');

        choice = toupper(choice);

        if (choice == 'Y')
        {
            cout << Utils::CYAN << "Nhap ID nguoi dung can xoa: " << Utils::RESET;
            if (!(cin >> id)) { cin.clear(); cin.ignore(100, '\n'); break; }
        }

    } while (choice == 'Y');
}

void UserManager::UpdateUserByID(int id)
{
    char choice;
    do {
        bool found = false;
        for (int i = 0; i < userCount; ++i)
        {
            if (users[i].getID() == id)
            {
                cout << Utils::CYAN << Utils::BOLD;
                Utils::PrintMenuBorder();
                Utils::PrintMenuHeader("CAP NHAT THONG TIN NGUOI DUNG");
                Utils::PrintMenuBorder();
                cout << Utils::RESET;

                users[i].UpdateByUser();
                SaveUsersToFile();
                found = true;
                break;
            }
        }

        if (!found)
            cout << Utils::RED << "Khong tim thay nguoi dung co ID " << id << Utils::RESET << endl;

        cout << Utils::CYAN << "Ban co muon sua nguoi dung khac khong? (Y/N): " << Utils::RESET;
        cin >> choice;
        cin.ignore(100, '\n');

        choice = toupper(choice);

        if (choice == 'Y')
        {
            cout << Utils::CYAN << "Nhap ID nguoi dung can sua: " << Utils::RESET;
            if (!(cin >> id)) { cin.clear(); cin.ignore(100, '\n'); break; }
        }

    } while (choice == 'Y');
}

void UserManager::UpdateByID(int id)
{
    char choice;
    do {
        bool found = false;
        for (int i = 0; i < userCount; ++i)
        {
            if (users[i].getID() == id)
            {
                users[i].UpdateByUser();
                SaveUsersToFile();
                found = true;
                break;
            }
        }

        if (!found)
            cout << "Khong tim thay nguoi dung co ID " << id << endl;

       
    } while (choice == 'Y');
}

void UserManager::ChangeUserPassword(int id)
{
    char new1[50], new2[50];

    bool found = false;
    for (int i = 0; i < userCount; ++i)
    {
        if (users[i].getID() == id)
        {
            cout << Utils::CYAN << Utils::BOLD;
            Utils::PrintMenuBorder();
            Utils::PrintMenuHeader("THAY DOI MAT KHAU NGUOI DUNG");
            Utils::PrintMenuBorder();
            cout << Utils::RESET;

            cout << Utils::YELLOW << "Dang thay doi mat khau cho nguoi dung ID: " << Utils::CYAN << id
                 << Utils::YELLOW << " (" << users[i].getName() << ")" << Utils::RESET << "\n\n";

            while (true)
            {
                cout << Utils::CYAN << "Nhap Mat Khau moi: " << Utils::RESET;
                cin.getline(new1, sizeof(new1));

                cout << Utils::CYAN << "Nhap lai Mat Khau moi: " << Utils::RESET;
                cin.getline(new2, sizeof(new2));

                if (strcmp(new1, new2) == 0)
                {
                    users[i].setPassword(new1);
                    SaveUsersToFile();
                    found = true;

                    cout << Utils::GREEN << Utils::BOLD << "THAY DOI MAT KHAU THANH CONG!";
                    cout << Utils::RESET << endl;
                    break;
                }
                else
                    cout << Utils::RED << "Mat khau moi khong khop! Vui long thu lai." << Utils::RESET << "\n\n";
            }
            break;
        }
    }

    if (!found)
        cout << Utils::RED << "Khong tim thay nguoi dung co ID " << id << Utils::RESET << endl;
}
