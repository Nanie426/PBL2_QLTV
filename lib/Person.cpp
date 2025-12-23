#include "Person.h"
#include "Utils.h"
#include <limits>

// Lấy họ (từ cuối)
string Person::getLastName() const {
    string fullName = this->getName();
    stringstream ss(fullName);
    string namePart, lastName = "";

    while (getline(ss, namePart, ' ')) {
        if (!namePart.empty())
            lastName = namePart;
    }
    return lastName;
}

void Person::InputUserDetails()
{
    char tempStr[100];

    do {
        cout << Utils::WHITE << "Ho ten: " << Utils::RESET;
        cin.getline(tempStr, sizeof(tempStr));
        if (strlen(tempStr) == 0) cout << Utils::RED << "Ten khong duoc de trong." << Utils::RESET << "\n";
    } while (strlen(tempStr) == 0);
    setName(tempStr);

    do {
        cout << Utils::WHITE << "Ngay sinh (DD/MM/YYYY): " << Utils::RESET;
        cin.getline(tempStr, sizeof(tempStr));
        if (strlen(tempStr) == 0) cout << Utils::RED << "Ngay sinh khong duoc de trong." << Utils::RESET << "\n";
    } while (strlen(tempStr) == 0);
    setDate(tempStr);

    do {
        cout << Utils::WHITE << "So Dien Thoai: " << Utils::RESET;
        cin.getline(tempStr, sizeof(tempStr));
        if (strlen(tempStr) == 0) cout << Utils::RED << "So dien thoai khong duoc de trong." << Utils::RESET << "\n";
    } while (strlen(tempStr) == 0);
    setPhone(tempStr);

    do {
        cout << Utils::WHITE << "Email: " << Utils::RESET;
        cin.getline(tempStr, sizeof(tempStr));
        if (strlen(tempStr) == 0) cout << Utils::RED << "Email khong duoc de trong." << Utils::RESET << "\n";
    } while (strlen(tempStr) == 0);
    setEmail(tempStr);

    cout << "\n";
}

void Person::CopyStringManual(char* dest, const char* src, size_t dest_size)
{
    size_t i = 0;
    while (i < dest_size - 1 && src[i] != '\0') {
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';
}

int Person::CompareStringManual(const char* s1, const char* s2) const
{
    while (*s1 && (*s1 == *s2)) {
        s1++; s2++;
    }
    return *(const unsigned char*)s1 - *(const unsigned char*)s2;
}

void Person::IntToStringManual(int num, char* buffer, size_t buffer_size)
{
    if (buffer_size == 0) return;
    if (num == 0) {
        if (buffer_size > 1) {
            buffer[0] = '0'; buffer[1] = '\0';
        } else buffer[0] = '\0';
        return;
    }

    bool neg = false;
    if (num < 0) { neg = true; num = -num; }

    int i = 0;
    while (num != 0 && i < buffer_size - 1) {
        buffer[i++] = (num % 10) + '0';
        num /= 10;
    }
    if (neg && i < buffer_size - 1)
        buffer[i++] = '-';

    buffer[i] = '\0';

    int start = 0, end = i - 1;
    while (start < end) {
        char t = buffer[start];
        buffer[start] = buffer[end];
        buffer[end] = t;
        start++; end--;
    }
}

// Constructor
Person::Person()
{
    ID = 0;
    name[0] = date[0] = phone[0] = email[0] = password[0] = '\0';
}

Person::Person(int id, const char n[], const char d[], const char p[], const char e[], const char pw[])
{
    ID = id;
    CopyStringManual(name, n, sizeof(name));
    CopyStringManual(date, d, sizeof(date));
    CopyStringManual(phone, p, sizeof(phone));
    CopyStringManual(email, e, sizeof(email));
    CopyStringManual(password, pw, sizeof(password));
}

Person::~Person() {}

void Person::setName(const char n[]) { CopyStringManual(name, n, sizeof(name)); }
void Person::setDate(const char d[]) { CopyStringManual(date, d, sizeof(date)); }
void Person::setPhone(const char p[]) { CopyStringManual(phone, p, sizeof(phone)); }
void Person::setEmail(const char e[]) { CopyStringManual(email, e, sizeof(email)); }
void Person::setID(int id) { ID = id; }
void Person::setPassword(const char p[]) { CopyStringManual(password, p, sizeof(password)); }

int Person::getID() const { return ID; }
const char* Person::getName() const { return name; }
const char* Person::getDate() const { return date; }
const char* Person::getPhone() const { return phone; }
const char* Person::getEmail() const { return email; }
const char* Person::getPassword() const { return password; }

void Person::InputAccountDetails()
{
    char default_pw[50];
    IntToStringManual(ID, default_pw, sizeof(default_pw));
    setPassword(default_pw);

    cout << "Mat Khau ban dau la: " << default_pw << "\n";

    char input[100];

    cout << "Nhap Ho ten: ";
    cin.getline(input, sizeof(input));
    setName(input);

    cout << "Nhap Ngay sinh (dd/mm/yyyy): ";
    cin.getline(input, sizeof(input));
    setDate(input);

    cout << "Nhap So dien thoai: ";
    cin.getline(input, sizeof(input));
    setPhone(input);

    cout << "Nhap Email: ";
    cin.getline(input, sizeof(input));
    setEmail(input);

    cout << "Tao tai khoan thanh cong!\n";
}

void Person::Show() const
{
    const int WIDTH = 59;
    cout << Utils::CYAN << Utils::BOLD;
    Utils::PrintMenuBorder(WIDTH);
    Utils::PrintMenuHeader("THONG TIN CA NHAN", WIDTH);
    Utils::PrintMenuBorder(WIDTH);
    cout << Utils::RESET;
    Utils::PrintInfoRow("ID", to_string(ID), WIDTH+2);
    Utils::PrintInfoRow("Ho Ten", name, WIDTH+2);
    Utils::PrintInfoRow("Ngay Sinh", date, WIDTH+2);
    Utils::PrintInfoRow("So Dien Thoai", phone, WIDTH+2);
    Utils::PrintInfoRow("Email", email, WIDTH+2);

    cout << Utils::CYAN << Utils::BOLD;
    Utils::PrintMenuBorder(WIDTH);
    cout << Utils::RESET;
}

ostream& operator<<(ostream& out, const Person& p)
{
    out << p.ID << "," << p.password << "," << p.name << "," 
        << p.date << "," << p.phone << "," << p.email;
    return out;
}

istream& operator>>(istream& in, Person& p)
{
    cout << "Nhap ID: ";
    while (!(in >> p.ID)) {
        cout << "ID khong hop le. Nhap lai ID: ";
        in.clear();
        in.ignore(100, '\n');
    }
    in.ignore();

    cout << "Nhap Mat Khau: ";
    in.getline(p.password, sizeof(p.password));

    cout << "Nhap Ho ten: ";
    in.getline(p.name, sizeof(p.name));

    cout << "Nhap Ngay sinh: ";
    in.getline(p.date, sizeof(p.date));

    cout << "Nhap So dien thoai: ";
    in.getline(p.phone, sizeof(p.phone));

    cout << "Nhap Email: ";
    in.getline(p.email, sizeof(p.email));

    return in;
}

bool Person::ChangePassword()
{
    char oldPw[50], new1[50], new2[50];
    const int WIDTH = 59;

    cout << Utils::CYAN << Utils::BOLD;
    Utils::PrintMenuBorder(WIDTH);
    Utils::PrintMenuHeader("THAY DOI MAT KHAU", WIDTH);
    Utils::PrintMenuBorder(WIDTH);
    cout << Utils::RESET;
    cout << Utils::YELLOW << "Nhap Mat khau cu: " << Utils::RESET;
    cin.getline(oldPw, sizeof(oldPw));

    if (CompareStringManual(oldPw, password) != 0) {
        cout << "Khong thay doi mat khau!\n";
        return false;
    }

    while (true)
    {

        cout << Utils::CYAN << "Nhap Mat khau moi: " << Utils::RESET;
        cin.getline(new1, sizeof(new1));

        cout << Utils::CYAN << "Nhap lai Mat khau moi: " << Utils::RESET;
        cin.getline(new2, sizeof(new2));

        if (CompareStringManual(new1, new2) == 0)
        {
            setPassword(new1);
            cout << Utils::GREEN << "Thay doi mat khau thanh cong!\n" << Utils::RESET;

            ifstream in("users.txt");
            ofstream out("temp.txt");

            if (!in.is_open() || !out.is_open()) return true;

            string line;
            while (getline(in, line)) {
                stringstream ss(line);
                string idS, pw, nameS, dateS, phoneS, emailS;

                getline(ss, idS, ',');
                getline(ss, pw, ',');
                getline(ss, nameS, ',');
                getline(ss, dateS, ',');
                getline(ss, phoneS, ',');
                getline(ss, emailS, ',');

                int fileID = stoi(idS);
                if (fileID == ID) {
                    out << ID << "," << password << "," << nameS << "," 
                        << dateS << "," << phoneS << "," << emailS << "\n";
                } else {
                    out << line << "\n";
                }
            }

            in.close();
            out.close();

            remove("users.txt");
            rename("temp.txt", "users.txt");

            return true;
        }
        else cout << Utils::RED << "Mat khau moi khong khop!\n" << Utils::RESET;
    }
}

void Person::Update()
{
    char input[100];
    char choice;

    cout << "\n--- CAP NHAT THONG TIN CA NHAN ---\n";

    cout << "Ban co muon thay doi mat khau? (Y/N): ";
    cin >> choice;
    cin.ignore(100, '\n'); // Clear the input buffer

    if (choice == 'Y' || choice == 'y') {
        if (!ChangePassword()) {
            cout << "Khong the thay doi mat khau. Tiep tuc cap nhat thong tin khac.\n";
        }
    } else {
        cout << "Khong thay doi mat khau!\n";
    }

    cout << "Ten hien tai: " << name << "\nNhap Ten moi: ";
    cin.getline(input, sizeof(input));
    if (input[0] != '\0') setName(input);

    cout << "Ngay sinh hien tai: " << date << "\nNhap Ngay sinh moi: ";
    cin.getline(input, sizeof(input));
    if (input[0] != '\0') setDate(input);

    cout << "SDT hien tai: " << phone << "\nNhap SDT moi: ";
    cin.getline(input, sizeof(input));
    if (input[0] != '\0') setPhone(input);

    cout << "Email hien tai: " << email << "\nNhap Email moi: ";
    cin.getline(input, sizeof(input));
    if (input[0] != '\0') setEmail(input);

    cout << "\nCap nhat thanh cong!\n";
    Show();
}

void Person::UpdateByUser()
{
    char input[100];
    const int WIDTH = 60;

    cout << Utils::CYAN << Utils::BOLD;
    Utils::PrintMenuBorder(WIDTH);
    Utils::PrintMenuHeader("CAP NHAT THONG TIN CA NHAN", WIDTH);
    Utils::PrintMenuBorder(WIDTH);
    cout << Utils::RESET;

    // ===== TEN =====
    cout << Utils::YELLOW << "Ten hien tai: " << name << Utils::RESET;
    cout << Utils::CYAN << "\nNhap ten moi (Enter de bo qua): " << Utils::RESET;
    
    //cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.getline(input, sizeof(input));
    if (input[0] != '\0')
        setName(input);


    // ===== NGAY SINH =====
    cout << Utils::YELLOW << "Ngay sinh hien tai: " << date << Utils::RESET<<"\n";
    cout << Utils::CYAN << "Nhap ngay sinh moi (DD/MM/YYYY): " << Utils::RESET;
    cin.getline(input, sizeof(input));
    if (input[0] != '\0')
        setDate(input);


    cout << Utils::YELLOW << "SDT hien tai: " << phone << Utils::RESET<<"\n";
    cout << Utils::CYAN << "Nhap SDT moi: " << Utils::RESET;
    cin.getline(input, sizeof(input));
    if (input[0] != '\0')
        setPhone(input);


    // ===== EMAIL =====
    cout << Utils::YELLOW << "Email hien tai: " << email << Utils::RESET<<"\n";
    cout << Utils::CYAN << "Nhap email moi: " << Utils::RESET;
    cin.getline(input, sizeof(input));
    if (input[0] != '\0')
        setEmail(input);


    cout << Utils::GREEN << Utils::BOLD
         << "CAP NHAT THONG TIN THANH CONG!"
         << Utils::RESET << "\n";
    cin.get();
    Show();
}
