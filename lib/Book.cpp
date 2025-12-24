#include "Book.h"
#include "Utils.h"   
int Book::getYear() const
{
    return year;
}

// Book.h:57:6:
void Book::setYear(int y)
{
    year = y;
}

Book::Book()
{
    bookID = 0;
    title[0] = author[0] = category[0] = '\0';
    pubYear = 0;
    quantity = 0;
    isBorrowed = false;
}

Book::Book(int id, const char* t, const char* a, const char* c, int y, int q, bool b)
{
    bookID = id;

    int i = 0;
    while (t[i] != '\0' && i < 99) { title[i] = t[i]; i++; }
    title[i] = '\0';

    i = 0;
    while (a[i] != '\0' && i < 99) { author[i] = a[i]; i++; }
    author[i] = '\0';

    i = 0;
    while (c[i] != '\0' && i < 99) { category[i] = c[i]; i++; }
    category[i] = '\0';

    pubYear = y;
    quantity = q;
    isBorrowed = b;
    totalQuantity = q;
}

Book::~Book() {}

int Book::getID() const { return bookID; }
const char* Book::getTitle() const { return title; }
const char* Book::getAuthor() const { return author; }
const char* Book::getCategory() const { return category; }
int Book::getPubYear() const { return pubYear; }
int Book::getQuantity() const { return quantity; }
bool Book::getBorrowed() const { return isBorrowed; }

void Book::setTitle(const char* t) {
    int i = 0;
    while (t[i] != '\0' && i < 99) { title[i] = t[i]; i++; }
    title[i] = '\0';
}

void Book::setID(int id) { bookID = id;}
void Book::setAuthor(const char* a) {
    int i = 0;
    while (a[i] != '\0' && i < 99) { author[i] = a[i]; i++; }
    author[i] = '\0';
}

void Book::setCategory(const char* c) {
    int i = 0;
    while (c[i] != '\0' && i < 99) { category[i] = c[i]; i++; }
    category[i] = '\0';
}

void Book::setPubYear(int y) { pubYear = y; }
void Book::setQuantity(int q) { quantity = q; }
void Book::setBorrowed(bool b) { isBorrowed = b; }

void Book::Show() const 
{
    const int WIDTH = 59;
    cout << Utils::CYAN << Utils::BOLD;
    Utils::PrintMenuBorder(WIDTH);
    Utils::PrintMenuHeader("THONG TIN SACH", WIDTH);
    Utils::PrintMenuBorder(WIDTH);
    cout << Utils::RESET;

    Utils::PrintInfoRow("ID", to_string(bookID), WIDTH+2);
    Utils::PrintInfoRow("Ten Sach", title, WIDTH+2);
    Utils::PrintInfoRow("Tac Gia", author, WIDTH+2);
    Utils::PrintInfoRow("The Loai", category, WIDTH+2);
    Utils::PrintInfoRow("Nam Xuat Ban", to_string(pubYear), WIDTH+2);
    Utils::PrintInfoRow("So Luong", to_string(quantity), WIDTH+2);
    Utils::PrintInfoRow("Trang Thai", (isBorrowed ? "Da muon" : "Con trong"), WIDTH+2);

    cout << Utils::CYAN << Utils::BOLD;
    Utils::PrintMenuBorder(WIDTH);
    cout << Utils::RESET << endl;
}

void Book::InputBook() 
{
    cout << "Nhap ten sach: ";
    cin.getline(title, 100);

    cout << "Nhap ten tac gia: ";
    cin.getline(author, 100);

    cout << "Nhap the loai: ";
    cin.getline(category, 100);

    cout << "Nhap nam xuat ban: ";
    cin >> pubYear;

    cout << "Nhap so luong sach: ";
    cin >> quantity;

    isBorrowed = false;
    cout << Utils::GREEN << "Them sach thanh cong!\n" << Utils::RESET;
}

void Book::WriteToFile(ostream& out) const 
{
    out << bookID << ',' 
        << title << ',' 
        << author << ',' 
        << category << ',' 
        << pubYear << ',' 
        << quantity << ',' 
        << isBorrowed << '\n';
}

void Book::ReadToFile(istream& in) 
{
    char c;
    char temp[100];
    int idx = 0;
    int field = 0;

    while (in.get(c)) {
        if (c == ',' || c == '\n') {
            temp[idx] = '\0';
            switch (field) {
            case 0: bookID = atoi(temp); break;
            case 1: setTitle(temp); break;
            case 2: setAuthor(temp); break;
            case 3: setCategory(temp); break;
            case 4: pubYear = atoi(temp); break;
            case 5: quantity = atoi(temp); break;
            case 6: isBorrowed = (temp[0] == '1'); break;
            }
            idx = 0;
            field++;
            if (c == '\n') break;
        } else {
            if (idx < 99) temp[idx++] = c;
        }
    }
}
