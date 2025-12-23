#include "login.h"
#include "Utils.h"



string Login::Trim(const string& str) {
    size_t first = str.find_first_not_of(" \t\n\r");
    if (string::npos == first) return str;
    size_t last = str.find_last_not_of(" \t\n\r");
    return str.substr(first, (last - first + 1));
}

int Login::CompareString(const char* s1, const char* s2) const
{
    int i = 0;
    while (s1[i] != '\0' && s2[i] != '\0') {
        if (s1[i] != s2[i]) return s1[i] - s2[i];
        i++;
    }
    return s1[i] - s2[i];
}

bool Login::DangNhap(const string& inputID, const string& inputPassword, const string& roleChoice, char* roleOutput)
{
    string filename;
    if (roleChoice == "admin") filename = "admin.txt";
    else if (roleChoice == "reader") filename = "users.txt";
    else return false;

    ifstream file(filename);
    if (!file.is_open()) return false;

    string line;
    if (filename == "admin.txt") getline(file, line);  // Skip header for admin.txt

    while (getline(file, line))
    {        
        if (line.size() >= 3 && (unsigned char)line[0] == 0xEF && (unsigned char)line[1] == 0xBB && (unsigned char)line[2] == 0xBF) {
            line = line.substr(3);
        }
        stringstream ss(line);
        string segment;
        string fields[2];
        int count = 0;
        while (getline(ss, segment, ',') && count < 2)
        {
            fields[count++] = Trim(segment);
        }
        if (fields[0] == inputID && fields[1] == inputPassword)
        {
            strncpy(roleOutput, roleChoice.c_str(), 10);
            roleOutput[9] = '\0';
            file.close();
            return true;
        }
    }
    file.close();
    roleOutput[0] = '\0';
    return false;
}

void Login::ShowStartMenu()
{
    int choice;
    string id, password;
    char role[10];
    string roleChoice;

    do {
        cout << Utils::CYAN << Utils::BOLD;
        Utils::PrintMenuBorder();
        Utils::PrintMenuHeader("HE THONG QUAN LY THU VIEN");
        Utils::PrintMenuBorder();
        Utils::PrintMenuLine("");
        Utils::PrintMenuLine("1. DANG NHAP VOI VAI TRO ADMIN");
        Utils::PrintMenuLine("2. DANG NHAP VOI VAI TRO READER");
        Utils::PrintMenuLine("0. Thoat");
        Utils::PrintMenuLine("");
        Utils::PrintMenuBorder();
        cout << Utils::RESET;
        cout << Utils::CYAN << "Chon: " << Utils::RESET;

        if (!(cin >> choice)) {
            cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n'); choice = -1;
        } else {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        if (choice == 1 || choice == 2)
        {
            roleChoice = (choice == 1) ? "admin" : "reader";
            bool loggedIn = false;

            while (!loggedIn) {
                cout << Utils::CYAN << Utils::BOLD;
                Utils::PrintMenuBorder();
                Utils::PrintMenuHeader("DANG NHAP VOI VAI TRO " + string(choice == 1 ? "ADMIN" : "READER"));
                Utils::PrintMenuBorder();
                cout << Utils::RESET;
                cout << Utils::CYAN << "ID: " << Utils::RESET; getline(cin, id);
                if (id == "0" || id == "Thoat") break;
                cout << Utils::CYAN << "Password: " << Utils::RESET; getline(cin, password);
                if (password == "0" || password == "Thoat") break;

                id = Trim(id);
                password = Trim(password);

                if (DangNhap(id, password, roleChoice, role))
                {
                    cout << Utils::GREEN << Utils::BOLD << "\nDang nhap thanh cong! Vai tro: " << role << "\n" << Utils::RESET;
                    loggedIn = true;

                    if (CompareString(role, "admin") == 0)
                    {
                        Admin adminManager;
                        adminManager.Menu();
                    }
                    else if (CompareString(role, "reader") == 0)
                    {
                        User userTask;
                        userTask.LoadUserByID(id);
                        userTask.Menu(this->userManager, this->bookManager);
                    }
                }
                else cout << Utils::RED << "ID hoac mat khau khong dung! Vui long thu lai.\n" << Utils::RESET;
            }
        }
        else if (choice == 0)
        {
            cout << Utils::YELLOW << "Tam biet! Cam on ban da su dung he thong quan ly thu vien.\n" << Utils::RESET;
        }
        else cout << Utils::RED << "Lua chon khong hop le! Vui long chon mot lua chon hop le.\n" << Utils::RESET;

    } while (choice != 0);
}
