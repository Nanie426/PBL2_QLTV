#include "BookManager.h"
#include "Utils.h"
#include <algorithm>
using namespace std;


using namespace std;

// Constructor: Gọi hàm load để cập nhật nextAvailableID
BookManager::BookManager() 
{
    // Giả định: books là mảng cố định (fixed size array)
    bookCount = 0;
    // Giả định: nextAvailableID được khởi tạo trong BookManager.h (ví dụ: nextAvailableID = 1;)
    Utils::CopyStringManual(userFilePath, "book.txt", sizeof(userFilePath));
    LoadBooksFromFile(); // ⬅️ Tải dữ liệu và cập nhật ID lớn nhất
}

// Hàm tải sách từ file và cập nhật ID lớn nhất
void BookManager::LoadBooksFromFile()
{
    ifstream inFile(userFilePath);
    if (!inFile.is_open())
    {
        // Nếu file không tồn tại, reset bookCount và bắt đầu ID từ 1
        bookCount = 0;
        this->nextAvailableID = 1; 
        return;
    }

    bookCount = 0;
    int maxID = 0; // Biến tìm ID lớn nhất
    char line[512];
    
    // Bỏ qua dòng tiêu đề nếu cần thiết
    // inFile.getline(line, sizeof(line));

    while (inFile.getline(line, sizeof(line)))
    {
        if (line[0] == '\0')
            continue;

        char fields[7][200];
        int fld = 0, pos = 0;
        
        // Logic phân tích chuỗi thủ công (Manual Split)
        for (int i = 0; line[i] != '\0'; ++i)
        {
            if (line[i] == ',')
            {
                fields[fld][pos] = '\0';
                fld++;
                pos = 0;
            }
            else
            {
                if (pos < 199)
                    fields[fld][pos++] = line[i];
            }
        }
        fields[fld][pos] = '\0'; 

        if (fld < 6) // Cần ít nhất 7 trường (ID, Title, Author, Category, Year, Qty, Borrowed)
            continue; 

        // Chuyển đổi từ chuỗi sang số
        int id = atoi(fields[0]);
        int pubYear = atoi(fields[4]);
        int quantity = atoi(fields[5]);
        bool borrowed = (fields[6][0] == '1');

        // Tạo đối tượng Book và thêm vào mảng
        books[bookCount] = Book(id, fields[1], fields[2], fields[3], pubYear, quantity, borrowed);
        
        // CẬP NHẬT ID LỚN NHẤT
        if (id > maxID) {
            maxID = id;
        }

        bookCount++;
        if (bookCount >= 100) // Giả định kích thước mảng tối đa là 100
            break;
    }

    inFile.close();
    
    // 🌟 KHẮC PHỤC LỖI ID TIẾP THEO 🌟
    this->nextAvailableID = maxID + 1; // Đảm bảo ID tiếp theo là maxID + 1 (ví dụ: 140)
}

// Hàm lưu sách vào file
void BookManager::SaveBooksToFile() const
{
    ofstream outFile(userFilePath);
    if (!outFile.is_open()) return;
    
    // Ghi tiêu đề nếu cần
    // outFile << "ID,Title,Author,Category,PubYear,Quantity,Borrowed\n";
    
    for (int i = 0; i < bookCount; ++i)
    {
        outFile << books[i].getID() << ","
                << books[i].getTitle() << ","
                << books[i].getAuthor() << ","
                << books[i].getCategory() << ","
                << books[i].getPubYear() << ","
                << books[i].getQuantity() << ","
                << (books[i].getBorrowed() ? 1 : 0);
        if (i < bookCount - 1)
            outFile << "\n";
    }
    outFile.close();
}

// Hàm tạo ID sách tiếp theo
string BookManager::GenerateNextBookID()
{
    // 1. Lấy ID hiện tại (ví dụ: 140)
    int newID = nextAvailableID;
    
    // 2. Tăng ID lên cho lần gọi tiếp theo (141)
    nextAvailableID++;
    
    // 3. Trả về ID dưới dạng string
    return to_string(newID);
} 

// Hàm thêm sách
void BookManager::AddBook()
{
    char choice;
    do
    {
        cout << Utils::GREEN << Utils::BOLD;
        Utils::PrintMenuBorder();
        Utils::PrintMenuHeader("THEM SACH MOI");
        Utils::PrintMenuBorder();
        cout << Utils::RESET;

        string newIDStr = GenerateNextBookID();
        int id = Utils::StringToIntManual(newIDStr.c_str());

        cout << Utils::CYAN << "ID sach: " << Utils::YELLOW << newIDStr << Utils::RESET << "\n\n";

        books[bookCount].setID(id);
        books[bookCount].InputBook();
        books[bookCount].setBorrowed(false);
        bookCount++;

        SaveBooksToFile();

        cout << Utils::CYAN << "\nBan co muon them sach khac khong? (Y/N): " << Utils::RESET;
        cin >> choice;
        cin.ignore(100, '\n');
        if (choice >= 'a' && choice <= 'z')
            choice -= 32;

    } while (choice == 'Y');
}

void BookManager::ShowAllBooks() const
{
    if (bookCount == 0)
    {
        cout << Utils::YELLOW << "Khong tim thay sach!" << Utils::RESET << "\n";
        return;
    }

    const int ID_WIDTH = 8;
    const int TITLE_WIDTH = 30;
    const int AUTHOR_WIDTH = 20;
    const int CATEGORY_WIDTH = 15;
    const int YEAR_WIDTH = 8;
    const int QUANTITY_WIDTH = 8;
    const vector<int> columnWidths = {ID_WIDTH, TITLE_WIDTH, AUTHOR_WIDTH, CATEGORY_WIDTH, YEAR_WIDTH, QUANTITY_WIDTH};

    int tableWidth = 0;
    for (int width : columnWidths) {
        tableWidth += width + 1;
    }
    tableWidth -= 1;

    cout << Utils::CYAN << Utils::BOLD;
    Utils::PrintMenuBorder(tableWidth);
    Utils::PrintMenuHeader("DANH SACH TAT CA CAC SACH (" + to_string(bookCount) + " quyen)", tableWidth);
    Utils::PrintMenuBorder(tableWidth);
    cout << Utils::RESET;

    cout << Utils::BOLD << Utils::YELLOW
         << left << setw(ID_WIDTH) << "ID" << " "
         << left << setw(TITLE_WIDTH) << "Ten sach" << " "
         << left << setw(AUTHOR_WIDTH) << "Tac gia" << " "
         << left << setw(CATEGORY_WIDTH) << "The loai" << " "
         << left << setw(YEAR_WIDTH) << "Nam XB" << " "
         << left << setw(QUANTITY_WIDTH) << "So luong"
         << Utils::RESET << endl;

    Utils::PrintTableLine(columnWidths);

    for (int i = 0; i < bookCount; ++i)
    {
        const Book& book = books[i];

        cout << left << setw(ID_WIDTH) << book.getID() << " "
             << left << setw(TITLE_WIDTH) << book.getTitle() << " "
             << left << setw(AUTHOR_WIDTH) << book.getAuthor() << " "
             << left << setw(CATEGORY_WIDTH) << book.getCategory() << " "
             << left << setw(YEAR_WIDTH) << book.getPubYear() << " "
             << left << setw(QUANTITY_WIDTH) << book.getQuantity()
             << endl;
    }

    Utils::PrintTableLine(columnWidths);
}

Book* BookManager::GetBookByID(int id)
{
    for (int i = 0; i < bookCount; ++i)
    {
        if (books[i].getID() == id)
            return &books[i]; 
    }
    return nullptr;
}

const Book* BookManager::GetBookByID(int id) const
{
    for (int i = 0; i < bookCount; ++i)
    {
        if (books[i].getID() == id)
            return &books[i];
    }
    return nullptr;
}


void BookManager::ShowStockReport() const
{
    int totalCopies = 0;
    int availableCopies = 0;
    int borrowedCopies = 0; 
    
    for (int i = 0; i < bookCount; ++i) {
        totalCopies += books[i].getTotalQuantity(); 
        availableCopies += books[i].getQuantity();
    }
    
    borrowedCopies = totalCopies - availableCopies;
    
    const int WIDTH = 59;
    cout << Utils::CYAN << Utils::BOLD;
    Utils::PrintMenuBorder(WIDTH);
    Utils::PrintMenuHeader("BAO CAO TON KHO SACH", WIDTH);
    Utils::PrintMenuBorder(WIDTH);
    cout << Utils::RESET;

    Utils::PrintInfoRow("Tong So Dau Sach", to_string(bookCount), WIDTH+2);
    Utils::PrintInfoRow("Tong So Ban Sao", to_string(totalCopies), WIDTH+2);
    Utils::PrintInfoRow("Ban Sao Con Lai", to_string(availableCopies), WIDTH+2);
    Utils::PrintInfoRow("Ban Sao Dang Muon", to_string(borrowedCopies), WIDTH+2);

    cout << Utils::CYAN << Utils::BOLD;
    Utils::PrintMenuBorder(WIDTH);
    cout << Utils::RESET;
}

void BookManager::SearchBookByAuthor(const char* author) const
{
    int foundCount = 0;
    
    const int ID_WIDTH = 8;
    const int TITLE_WIDTH = 45;
    const int AUTHOR_WIDTH = 30;
    const int CATEGORY_WIDTH = 20;
    const int YEAR_WIDTH = 10;
    const int QUANTITY_WIDTH = 10;
    const vector<int> columnWidths = {ID_WIDTH, TITLE_WIDTH, AUTHOR_WIDTH, CATEGORY_WIDTH, YEAR_WIDTH, QUANTITY_WIDTH};

    int tableWidth = 0;
    for (int width : columnWidths) {
        tableWidth += width + 1;
    }
    tableWidth -= 1;

    cout << Utils::CYAN << Utils::BOLD;
    Utils::PrintMenuBorder(tableWidth);
    Utils::PrintMenuHeader("KET QUA TIM KIEM - TAC GIA: " + string(author), tableWidth);
    Utils::PrintMenuBorder(tableWidth);
    cout << Utils::RESET;

    cout << Utils::BOLD << Utils::YELLOW
         << left << setw(ID_WIDTH) << "ID" << " "
         << left << setw(TITLE_WIDTH) << "Ten sach" << " "
         << left << setw(AUTHOR_WIDTH) << "Tac gia" << " "
         << left << setw(CATEGORY_WIDTH) << "The loai" << " "
         << left << setw(YEAR_WIDTH) << "Nam XB" << " "
         << left << setw(QUANTITY_WIDTH) << "So luong"
         << Utils::RESET << endl;

    Utils::PrintTableLine(columnWidths);

    for (int i = 0; i < bookCount; ++i) {
        
        if (Utils::FindSubstringManual(books[i].getAuthor(), author)) 
        {
            const Book& book = books[i];
            cout << left << setw(ID_WIDTH) << book.getID() << " "
                 << left << setw(TITLE_WIDTH) << book.getTitle() << " "
                 << left << setw(AUTHOR_WIDTH) << book.getAuthor() << " "
                 << left << setw(CATEGORY_WIDTH) << book.getCategory() << " "
                 << left << setw(YEAR_WIDTH) << book.getPubYear() << " "
                 << left << setw(QUANTITY_WIDTH) << book.getQuantity()
                 << endl;
            foundCount++;
        }
    }
    
    // Bottom separator
    Utils::PrintTableLine(columnWidths);
    
    if (foundCount == 0) {
        cout << Utils::YELLOW << "Khong tim thay sach nao cua tac gia '" << author << "'." << Utils::RESET << "\n";
    } else {
        cout << Utils::GREEN << "Tim thay " << foundCount << " cuon sach." << Utils::RESET << "\n";
    }
}

void BookManager::SearchBookByCategory(const char* category) const
{
    int foundCount = 0;
    
    const int ID_WIDTH = 8;
    const int TITLE_WIDTH = 45;
    const int AUTHOR_WIDTH = 30;
    const int CATEGORY_WIDTH = 20;
    const int YEAR_WIDTH = 10;
    const int QUANTITY_WIDTH = 10;
    const vector<int> columnWidths = {ID_WIDTH, TITLE_WIDTH, AUTHOR_WIDTH, CATEGORY_WIDTH, YEAR_WIDTH, QUANTITY_WIDTH};

    int tableWidth = 0;
    for (int width : columnWidths) {
        tableWidth += width + 1;
    }
    tableWidth -= 1;

    cout << Utils::CYAN << Utils::BOLD;
    Utils::PrintMenuBorder(tableWidth);
    Utils::PrintMenuHeader("KET QUA TIM KIEM - THE LOAI: " + string(category), tableWidth);
    Utils::PrintMenuBorder(tableWidth);
    cout << Utils::RESET;

    // Table headers
    cout << Utils::BOLD << Utils::YELLOW
         << left << setw(ID_WIDTH) << "ID" << " "
         << left << setw(TITLE_WIDTH) << "Ten sach" << " "
         << left << setw(AUTHOR_WIDTH) << "Tac gia" << " "
         << left << setw(CATEGORY_WIDTH) << "The loai" << " "
         << left << setw(YEAR_WIDTH) << "Nam XB" << " "
         << left << setw(QUANTITY_WIDTH) << "So luong"
         << Utils::RESET << endl;

    // Separator line
    Utils::PrintTableLine(columnWidths);

    for (int i = 0; i < bookCount; ++i) {
        
        if (containsIgnoreCase(books[i].getCategory(), category)) 
        {
            const Book& book = books[i];
            cout << left << setw(ID_WIDTH) << book.getID() << " "
                 << left << setw(TITLE_WIDTH) << book.getTitle() << " "
                 << left << setw(AUTHOR_WIDTH) << book.getAuthor() << " "
                 << left << setw(CATEGORY_WIDTH) << book.getCategory() << " "
                 << left << setw(YEAR_WIDTH) << book.getPubYear() << " "
                 << left << setw(QUANTITY_WIDTH) << book.getQuantity()
                 << endl;
            foundCount++;
        }
    }
    
    // Bottom separator
    Utils::PrintTableLine(columnWidths);
    
    if (foundCount == 0) {
        cout << Utils::YELLOW << "Khong tim thay sach nao cua the loai '" << category << "'." << Utils::RESET << "\n";
    } else {
        cout << Utils::GREEN << "Tim thay " << foundCount << " cuon sach." << Utils::RESET << "\n";
    }
}

bool BookManager::IsBookIDExist(int id)
{
    for (int i = 0; i < bookCount; ++i)
        if (books[i].getID() == id)
            return true;
    return false;
}

char BookManager::toLower(char c) const {
    if (c >= 'A' && c <= 'Z') return c + 32;
    return c;
}

void BookManager::trim(char* str) {
    int start = 0;
    while (str[start] == ' ' || str[start] == '\t') start++;

    int end = 0;
    while (str[end] != '\0') end++;
    end--; 
    while (end >= start && (str[end] == ' ' || str[end] == '\t')) end--;

    int j = 0;
    for (int i = start; i <= end; ++i) {
        str[j++] = str[i];
    }
    str[j] = '\0';
}

bool BookManager::containsIgnoreCase(const char* text, const char* keyword) const {
    for (int i = 0; text[i] != '\0'; ++i) {
        int j = 0;
        while (keyword[j] != '\0' && text[i + j] != '\0' &&
               toLower(text[i + j]) == toLower(keyword[j])) {
            j++;
        }
        if (keyword[j] == '\0') return true; 
    }
    return false;
}

void BookManager::SearchBookByTitle() {
    char keyword[100];
    cout << Utils::CYAN << "Nhap ten sach can tim: " << Utils::RESET;
    cin.ignore();
    cin.getline(keyword, 100);
    trim(keyword);

    int foundCount = 0;
    const int ID_WIDTH = 8;
    const int TITLE_WIDTH = 45;
    const int AUTHOR_WIDTH = 30;
    const int CATEGORY_WIDTH = 20;
    const int YEAR_WIDTH = 10;
    const int QUANTITY_WIDTH = 10;
    const vector<int> columnWidths = {ID_WIDTH, TITLE_WIDTH, AUTHOR_WIDTH, CATEGORY_WIDTH, YEAR_WIDTH, QUANTITY_WIDTH};

    int tableWidth = 0;
    for (int width : columnWidths) {
        tableWidth += width + 1;
    }
    tableWidth -= 1;

    cout << Utils::CYAN << Utils::BOLD;
    Utils::PrintMenuBorder(tableWidth);
    Utils::PrintMenuHeader("KET QUA TIM KIEM - TEN SACH: " + string(keyword), tableWidth);
    Utils::PrintMenuBorder(tableWidth);
    cout << Utils::RESET;

    // Table headers
    cout << Utils::BOLD << Utils::YELLOW
         << left << setw(ID_WIDTH) << "ID" << " "
         << left << setw(TITLE_WIDTH) << "Ten sach" << " "
         << left << setw(AUTHOR_WIDTH) << "Tac gia" << " "
         << left << setw(CATEGORY_WIDTH) << "The loai" << " "
         << left << setw(YEAR_WIDTH) << "Nam XB" << " "
         << left << setw(QUANTITY_WIDTH) << "So luong"
         << Utils::RESET << endl;

    // Separator line
    Utils::PrintTableLine(columnWidths);

    for (int i = 0; i < bookCount; ++i) {
        const char* title = books[i].getTitle();
        if (containsIgnoreCase(title, keyword)) {
            const Book& book = books[i];
            cout << left << setw(ID_WIDTH) << book.getID() << " "
                 << left << setw(TITLE_WIDTH) << book.getTitle() << " "
                 << left << setw(AUTHOR_WIDTH) << book.getAuthor() << " "
                 << left << setw(CATEGORY_WIDTH) << book.getCategory() << " "
                 << left << setw(YEAR_WIDTH) << book.getPubYear() << " "
                 << left << setw(QUANTITY_WIDTH) << book.getQuantity()
                 << endl;
            foundCount++;
        }
    }

    // Bottom separator
    Utils::PrintTableLine(columnWidths);

    if (!foundCount) {
        cout << Utils::YELLOW << "Khong tim thay sach phu hop." << Utils::RESET << "\n";
    } else {
        cout << Utils::GREEN << "Tim thay " << foundCount << " cuon sach." << Utils::RESET << "\n";
    }
}

void BookManager::DeleteBookByID(int id)
{
    char choice;
    do
    {
        int pos = -1;
        for (int i = 0; i < bookCount; ++i)
            if (books[i].getID() == id)
            {
                pos = i;
                break;
            }

        if (pos == -1)
        {
            cout << "Khong tim thay sach co ID " << id << endl;
        }
        else
        {
            cout << "\nBan co chac chan muon xoa sach sau khong?\n";
            books[pos].Show();
            cout << "Ban co chac chan muon xoa sach nay khong? (Y/N): ";
            char confirm;
            cin >> confirm;
            cin.ignore(100, '\n');
            if (confirm >= 'a' && confirm <= 'z')
                confirm -= 32;

            if (confirm == 'Y')
            {
                for (int i = pos; i < bookCount - 1; ++i)
                    books[i] = books[i + 1];
                bookCount--;
                SaveBooksToFile();
                cout << "Xoa sach thanh cong!\n";
            }
            else
            {
                cout << "Da huy thao tac xoa.\n";
            }
        }

        cout << "\nBan co muon xoa sach khac khong? (Y/N): ";
        cin >> choice;
        cin.ignore(100, '\n');
        if (choice >= 'a' && choice <= 'z')
            choice -= 32;

        if (choice == 'Y')
        {
            cout << Utils::CYAN << "Enter Book ID to delete: " << Utils::RESET;
            if (!(cin >> id))
            {
                cin.clear();
                cin.ignore(100, '\n');
                break;
            }
        }

    } while (choice == 'Y');
}
void BookManager::ShowStockReportPerBook() const
{
    if (bookCount == 0) {
        cout << Utils::YELLOW << "No books in the library yet!" << Utils::RESET << "\n";
        return;
    }

    cout << "\n===== BAO CAO THEO TUNG SACH =====\n";
    cout << "ID\tTen sach\t\tTong ban\tCon lai\tDang muon\n";
    cout << "-------------------------------------------------------------\n";

    for (int i = 0; i < bookCount; ++i)
    {
        int totalCopies = books[i].getTotalQuantity();
        int available = books[i].getQuantity();
        int borrowed = totalCopies - available;

        cout << books[i].getID() << "\t"
             << books[i].getTitle() << "\t\t"
             << totalCopies << "\t\t"
             << available << "\t"
             << borrowed << "\n";
    }

    cout << "=============================================================\n";
}

void BookManager::UpdateBookByID(int id)
{
    Book* book = GetBookByID(id);
    if (!book)
    {
        cout << Utils::RED
             << "Khong tim thay sach co ID: " << id
             << Utils::RESET << "\n";
        return;
    }

    const int WIDTH = 70;
    char input[200];

    // ===== THONG TIN HIEN TAI =====

    cout << Utils::CYAN << Utils::BOLD;
    Utils::PrintMenuBorder(WIDTH);
    Utils::PrintMenuHeader("THONG TIN HIEN TAI", WIDTH);
    Utils::PrintMenuBorder(WIDTH);
    cout << Utils::RESET;

    Utils::PrintInfoRow("ID sach", to_string(book->getID()), WIDTH+2);
    Utils::PrintInfoRow("Ten sach", book->getTitle(), WIDTH+2);
    Utils::PrintInfoRow("Tac gia", book->getAuthor(), WIDTH+2);
    Utils::PrintInfoRow("Nam xuat ban", to_string(book->getPubYear()), WIDTH+2);
    Utils::PrintInfoRow("The loai", book->getCategory(), WIDTH+2);
    Utils::PrintInfoRow("So luong", to_string(book->getQuantity()), WIDTH+2);
    cout << Utils::CYAN << Utils::BOLD;
    Utils::PrintMenuBorder(WIDTH);
    cout << Utils::RESET << "\n";
    // ===== CAP NHAT TEN SACH =====
    cout << Utils::YELLOW << Utils::BOLD;
    Utils::PrintMenuBorder(WIDTH);
    Utils::PrintMenuHeader("CAP NHAT THONG TIN SACH", WIDTH);
    Utils::PrintMenuBorder(WIDTH);
    cout << Utils::RESET;
    cout << Utils::CYAN << Utils::BOLD;
    cout << "Nhap ten sach moi (Enter de bo qua): ";
    cin.ignore(1024, '\n');
    cin.getline(input, sizeof(input));
    if (input[0] != '\0')
        book->setTitle(input);


    // ===== CAP NHAT TAC GIA =====
    cout << "Nhap ten tac gia moi (Enter de bo qua): ";
    cin.getline(input, sizeof(input));
    if (input[0] != '\0')
        book->setAuthor(input);



    // ===== CAP NHAT NAM XUAT BAN =====
    cout << "Nhap nam xuat ban moi (Enter de bo qua): ";
    cin.getline(input, sizeof(input));
    if (input[0] != '\0')
    {
        int newYear = Utils::CharArrayToIntManual(input);
        if (newYear > 0)
            book->setPubYear(newYear);
        else
            cout << Utils::YELLOW
                 << "Canh bao: Nam xuat ban khong hop le!"
                 << Utils::RESET << "\n";
    }


    // ===== CAP NHAT THE LOAI =====
    cout << "Nhap the loai moi (Enter de bo qua): ";
    cin.getline(input, sizeof(input));
    if (input[0] != '\0')
        book->setCategory(input);

   
    // ===== HOAN TAT =====
    SaveBooksToFile();

    cout << Utils::GREEN << Utils::BOLD
         << "CAP NHAT THONG TIN SACH THANH CONG!"
         << Utils::RESET << "\n";

    cout << Utils::CYAN
         << "Nhan Enter de quay lai..."
         << Utils::RESET;
    cin.get();
 }
void BookManager::SortBooksByID(bool ascending)
{
    sort(books, books + bookCount,
        [ascending](const Book& a, const Book& b) {
            return ascending ? a.getID() < b.getID()
                             : a.getID() > b.getID();
        });
}

void BookManager::SortBooksByTitle(bool ascending)
{
    sort(books, books + bookCount,
        [ascending](const Book& a, const Book& b) {
            return ascending ? strcmp(a.getTitle(), b.getTitle()) < 0
                             : strcmp(a.getTitle(), b.getTitle()) > 0;
        });
}

void BookManager::SortBooksByQuantity(bool ascending)
{
    sort(books, books + bookCount,
        [ascending](const Book& a, const Book& b) {
            return ascending ? a.getQuantity() < b.getQuantity()
                             : a.getQuantity() > b.getQuantity();
        });
}
void BookManager::SortBooksByBorrowCount(bool descending)
{
    sort(books, books + bookCount,
        [descending](const Book& a, const Book& b)
        {
            int borrowedA = a.getTotalQuantity() - a.getQuantity();
            int borrowedB = b.getTotalQuantity() - b.getQuantity();

            return descending ? borrowedA > borrowedB
                              : borrowedA < borrowedB;
        });
}

void BookManager::SortBooksMenu()
{
    int choice;
    bool ascending;

    do {
        cout << Utils::CYAN << Utils::BOLD;
        Utils::PrintMenuBorder();
        Utils::PrintMenuHeader("SAP XEP SACH", 59);
        Utils::PrintMenuBorder();
        cout << Utils::RESET;

        cout << "1. Sap xep theo ID\n";
        cout << "2. Sap xep theo Ten sach\n";
        cout << "3. Sap xep theo So luong\n";
        cout << "4. Thoat\n";
        cout << Utils::CYAN << "Chon phuong thuc sap xep (1-4): " << Utils::RESET;

        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(1000, '\n');
            choice = -1;
        }

        switch (choice) {
        case 1:
            ascending = ChooseAscending();
            SortBooksByID(ascending);
            ShowAllBooks();
            break;

        case 2:
            ascending = ChooseAscending();
            SortBooksByTitle(ascending);
            ShowAllBooks();
            break;

        case 3:
            ascending = ChooseAscending();
            SortBooksByQuantity(ascending);
            ShowAllBooks();
            break;

        case 4:
            cout << "Thoat khoi menu sap xep.\n";
            break;

        default:
            cout << Utils::RED
                 << "Lua chon khong hop le. Vui long chon lai."
                 << Utils::RESET << "\n";
        }

    } while (choice != 4);
}


bool BookManager::ChooseAscending()
{
    int opt;
    cout << "   1. Tang dan\n";
    cout << "   2. Giam dan\n";
    cout << "   Chon: ";
    cin >> opt;
    return (opt == 1);
}
