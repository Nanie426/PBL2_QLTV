#include "UserManager.h"

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
        cout << "\n--- THEM NGUOI DUNG MOI ---\n";

        string newIDStr = GenerateNextUserID();
        int tempID = Utils::StringToIntManual(newIDStr.c_str());

        cout << "ID nguoi dung moi: " << newIDStr << "\n";

        users[userCount].setID(tempID);

        // Nhập thông tin cơ bản (Không nhập mật khẩu)
        users[userCount].InputUserDetails(); 

        // Tạo mật khẩu tự động
        string lastName = users[userCount].getLastName();
        string dob = users[userCount].getDate();
        string newPassword = Utils::GeneratePassword(lastName, dob);

        users[userCount].setPassword(newPassword.c_str());
        cout << "Them nguoi dung thanh cong!" << "\n";

        cout << "MAT KHAU: " << newPassword << "\n";
        users[userCount].Show();

        userCount++;
        SaveUsersToFile();

        cout << "Ban co muon them nguoi dung khac khong? (Y/N): ";
        cin >> choice;
        cin.ignore(100, '\n');

        choice = toupper(choice);

    } while (choice == 'Y');
}

void UserManager::ShowAllUsers() const
{
    if (userCount == 0)
    {
        cout << "Chua co nguoi dung nao!\n";
        return;
    }

    // Thiết lập độ rộng cột cho bảng
    const int ID_WIDTH = 10;
    const int NAME_WIDTH = 30;
    const int DATE_WIDTH = 12;
    const int PHONE_WIDTH = 12;
    const int EMAIL_WIDTH = 30;
    const int TOTAL_WIDTH = ID_WIDTH + NAME_WIDTH + DATE_WIDTH + PHONE_WIDTH + EMAIL_WIDTH;

    cout << "\n========== DANH SACH TAT CA NGUOI DUNG (" << userCount << " nguoi) ==========\n";

    // In tiêu đề bảng
    cout << setfill(' ') << left << setw(ID_WIDTH) << "ID" << " "
         << left << setw(NAME_WIDTH) << "Ten" << " "
         << left << setw(DATE_WIDTH) << "Ngay sinh" << " "
         << left << setw(PHONE_WIDTH) << "So dien thoai" << " "
         << left << setw(EMAIL_WIDTH) << "Email"
         << endl;

    // In dòng phân cách
    cout << setfill('-') << setw(TOTAL_WIDTH + 4) << "" << setfill(' ') << endl;  // +4 for the 4 spaces

    // Duyệt và in dữ liệu
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

    // In dòng kết thúc
    cout << setfill('-') << setw(TOTAL_WIDTH + 4) << "" << setfill(' ') << endl;
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
            cout << "Khong tim thay nguoi dung co ID " << id << endl;
        }
        else
        {
            cout << "\nBan co chac chan muon xoa nguoi dung sau khong?\n";
            users[pos].Show();
            cout << "Ban co chac chan muon xoa nguoi nay khong? (Y/N): ";

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
                cout << "Xoa thanh cong!\n";
            }
        }

        cout << "Ban co muon xoa nguoi dung khac khong? (Y/N): ";
        cin >> choice;
        cin.ignore(100, '\n');

        choice = toupper(choice);

        if (choice == 'Y')
        {
            cout << "Nhap ID nguoi dung can xoa: ";
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
                users[i].Update();
                SaveUsersToFile();
                found = true;
                break;
            }
        }

        if (!found)
            cout << "Khong tim thay nguoi dung co ID " << id << endl;

        cout << "Ban co muon sua nguoi dung khac khong? (Y/N): ";
        cin >> choice;
        cin.ignore(100, '\n');

        choice = toupper(choice);

        if (choice == 'Y')
        {
            cout << "Nhap ID nguoi dung can sua: ";
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

        // vòng lặp này thực ra vô nghĩa → bạn có thể xoá luôn
        // nhưng giữ nguyên theo code bạn gửi
    } while (choice == 'Y');
}
