#include "BookManager.h"

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
        cout << "\n--- THEM SACH MOI ---\n";
        string newIDStr = GenerateNextBookID();
        int id = Utils::StringToIntManual(newIDStr.c_str());
        
        cout << "ID sach tu dong: " << newIDStr << "\n";
        
        // Bỏ qua ký tự '\n' còn sót lại (nếu cần thiết)
        
        // Đặt ID và nhận thông tin từ người dùng
        books[bookCount].setID(id);
        books[bookCount].InputBook();
        books[bookCount].setBorrowed(false); 
        bookCount++;

        SaveBooksToFile();

        cout << "Ban co muon them sach khac khong? (Y/N): ";
        cin >> choice;
        cin.ignore(100, '\n');
        if (choice >= 'a' && choice <= 'z')
            choice -= 32;

    } while (choice == 'Y');
}

// Hàm hiển thị tất cả sách (Đã giữ lại logic bảng của bạn)
void BookManager::ShowAllBooks() const
{
    if (bookCount == 0)
    {
        cout << "Chua co sach nao!\n";
        return;
    }

    // Thiết lập độ rộng cột cho bảng
    const int ID_WIDTH = 8;
    const int TITLE_WIDTH = 45;
    const int AUTHOR_WIDTH = 30;
    // const int PUBLISHER_WIDTH = 30; // Biến này không được dùng trong output
    const int YEAR_WIDTH = 10;
    const int QUANTITY_WIDTH = 10;
    // const int PRICE_WIDTH = 15; // Biến này không được dùng trong output
    const int TOTAL_WIDTH = ID_WIDTH + TITLE_WIDTH + AUTHOR_WIDTH + YEAR_WIDTH + QUANTITY_WIDTH;

    cout << "\n========== DANH SACH TAT CA SACH (" << bookCount << " cuon) ==========\n";

    // --- 1. In Tiêu đề Bảng ---
    cout << setfill(' '); 

    cout << left << setw(ID_WIDTH) << "ID"
         << left << setw(TITLE_WIDTH) << "Ten sach"
         << left << setw(AUTHOR_WIDTH) << "Tac gia"
         << left << setw(YEAR_WIDTH) << "Nam XB"
         << left << setw(QUANTITY_WIDTH) << "So luong"
         << endl;

    // --- 2. In Dòng Phân cách ---
    cout << setfill('-') << setw(TOTAL_WIDTH) << "" << setfill(' ') << endl;

    // --- 3. Duyệt và In Dữ liệu ---
    for (int i = 0; i < bookCount; ++i)
    {
        const Book& book = books[i];

        // In từng cột (Giả định class Book có các hàm getter tương ứng)
        cout << left << setw(ID_WIDTH) << book.getID()
             << left << setw(TITLE_WIDTH) << book.getTitle()
             << left << setw(AUTHOR_WIDTH) << book.getAuthor()
             << left << setw(YEAR_WIDTH) << book.getPubYear() // Giả định getPubYear() là đúng
             << left << setw(QUANTITY_WIDTH) << book.getQuantity()
             << endl;
    }

    // --- 4. In Dòng Kết thúc ---
    cout << setfill('-') << setw(TOTAL_WIDTH) << "" << setfill(' ') << endl;
}

// Hàm tìm sách theo ID (Non-const - Dùng để Update)
Book* BookManager::GetBookByID(int id)
{
    for (int i = 0; i < bookCount; ++i)
    {
        if (books[i].getID() == id)
            return &books[i]; 
    }
    return nullptr;
}

// Hàm tìm sách theo ID (Const - Dùng để Show/Read Only)
const Book* BookManager::GetBookByID(int id) const
{
    for (int i = 0; i < bookCount; ++i)
    {
        if (books[i].getID() == id)
            return &books[i];
    }
    return nullptr;
}


// CÁC HÀM KHÁC (Được giữ nguyên)

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
    
    cout << "\n========== BAO CAO THONG KE SACH ==========\n";
    cout << "Tong so dau sach: " << bookCount << "\n";
    cout << "Tong so ban sao (copies): " << totalCopies << "\n";
    cout << "   - So ban sao CON LAI: " << availableCopies << "\n";
    cout << "   - So ban sao DANG MUON: " << borrowedCopies << "\n";
    cout << "===========================================\n";
}

void BookManager::SearchBookByAuthor(const char* author) const
{
    cout << "\n--- KET QUA TIM KIEM TAC GIA: " << author << " ---\n";
    int foundCount = 0;
    
    // Thiết lập độ rộng cột cho bảng
    const int ID_WIDTH = 8;
    const int TITLE_WIDTH = 45;
    const int AUTHOR_WIDTH = 30;
    const int CATEGORY_WIDTH = 20;
    const int YEAR_WIDTH = 10;
    const int QUANTITY_WIDTH = 10;
    const int TOTAL_WIDTH = ID_WIDTH + TITLE_WIDTH + AUTHOR_WIDTH + CATEGORY_WIDTH + YEAR_WIDTH + QUANTITY_WIDTH;

    // In tiêu đề bảng
    cout << setfill(' ') << left << setw(ID_WIDTH) << "ID"
         << left << setw(TITLE_WIDTH) << "Ten sach"
         << left << setw(AUTHOR_WIDTH) << "Tac gia"
         << left << setw(CATEGORY_WIDTH) << "The loai"
         << left << setw(YEAR_WIDTH) << "Nam XB"
         << left << setw(QUANTITY_WIDTH) << "So luong"
         << endl;

    // In dòng phân cách
    cout << setfill('-') << setw(TOTAL_WIDTH) << "" << setfill(' ') << endl;

    for (int i = 0; i < bookCount; ++i) {
        
        if (Utils::FindSubstringManual(books[i].getAuthor(), author)) 
        {
            const Book& book = books[i];
            cout << left << setw(ID_WIDTH) << book.getID()
                 << left << setw(TITLE_WIDTH) << book.getTitle()
                 << left << setw(AUTHOR_WIDTH) << book.getAuthor()
                 << left << setw(CATEGORY_WIDTH) << book.getCategory()
                 << left << setw(YEAR_WIDTH) << book.getPubYear()
                 << left << setw(QUANTITY_WIDTH) << book.getQuantity()
                 << endl;
            foundCount++;
        }
    }
    
    // In dòng kết thúc
    cout << setfill('-') << setw(TOTAL_WIDTH) << "" << setfill(' ') << endl;
    
    if (foundCount == 0) {
        cout << "Khong tim thay sach nao cua tac gia '" << author << "'.\n";
    } else {
        cout << "Tim thay " << foundCount << " cuon sach.\n";
    }
}

void BookManager::SearchBookByCategory(const char* category) const
{
    cout << "\n--- KET QUA TIM KIEM THE LOAI: " << category << " ---\n";
    int foundCount = 0;
    
    // Thiết lập độ rộng cột cho bảng
    const int ID_WIDTH = 8;
    const int TITLE_WIDTH = 45;
    const int AUTHOR_WIDTH = 30;
    const int CATEGORY_WIDTH = 20;
    const int YEAR_WIDTH = 10;
    const int QUANTITY_WIDTH = 10;
    const int TOTAL_WIDTH = ID_WIDTH + TITLE_WIDTH + AUTHOR_WIDTH + CATEGORY_WIDTH + YEAR_WIDTH + QUANTITY_WIDTH;

    // In tiêu đề bảng
    cout << setfill(' ') << left << setw(ID_WIDTH) << "ID"
         << left << setw(TITLE_WIDTH) << "Ten sach"
         << left << setw(AUTHOR_WIDTH) << "Tac gia"
         << left << setw(CATEGORY_WIDTH) << "The loai"
         << left << setw(YEAR_WIDTH) << "Nam XB"
         << left << setw(QUANTITY_WIDTH) << "So luong"
         << endl;

    // In dòng phân cách
    cout << setfill('-') << setw(TOTAL_WIDTH) << "" << setfill(' ') << endl;

    for (int i = 0; i < bookCount; ++i) {
        
        if (containsIgnoreCase(books[i].getCategory(), category)) 
        {
            const Book& book = books[i];
            cout << left << setw(ID_WIDTH) << book.getID()
                 << left << setw(TITLE_WIDTH) << book.getTitle()
                 << left << setw(AUTHOR_WIDTH) << book.getAuthor()
                 << left << setw(CATEGORY_WIDTH) << book.getCategory()
                 << left << setw(YEAR_WIDTH) << book.getPubYear()
                 << left << setw(QUANTITY_WIDTH) << book.getQuantity()
                 << endl;
            foundCount++;
        }
    }
    
    // In dòng kết thúc
    cout << setfill('-') << setw(TOTAL_WIDTH) << "" << setfill(' ') << endl;
    
    if (foundCount == 0) {
        cout << "Khong tim thay sach nao cua the loai '" << category << "'.\n";
    } else {
        cout << "Tim thay " << foundCount << " cuon sach.\n";
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
    cout << "Nhap ten sach can tim: ";
    cin.ignore();
    cin.getline(keyword, 100);
    trim(keyword);

    cout << "\n--- KET QUA TIM KIEM TEN SACH: " << keyword << " ---\n";
    int foundCount = 0;

    // Thiết lập độ rộng cột cho bảng
    const int ID_WIDTH = 8;
    const int TITLE_WIDTH = 45;
    const int AUTHOR_WIDTH = 30;
    const int CATEGORY_WIDTH = 20;
    const int YEAR_WIDTH = 10;
    const int QUANTITY_WIDTH = 10;
    const int TOTAL_WIDTH = ID_WIDTH + TITLE_WIDTH + AUTHOR_WIDTH + CATEGORY_WIDTH + YEAR_WIDTH + QUANTITY_WIDTH;

    // In tiêu đề bảng
    cout << setfill(' ') << left << setw(ID_WIDTH) << "ID"
         << left << setw(TITLE_WIDTH) << "Ten sach"
         << left << setw(AUTHOR_WIDTH) << "Tac gia"
         << left << setw(CATEGORY_WIDTH) << "The loai"
         << left << setw(YEAR_WIDTH) << "Nam XB"
         << left << setw(QUANTITY_WIDTH) << "So luong"
         << endl;

    // In dòng phân cách
    cout << setfill('-') << setw(TOTAL_WIDTH) << "" << setfill(' ') << endl;

    for (int i = 0; i < bookCount; ++i) {
        const char* title = books[i].getTitle();
        if (containsIgnoreCase(title, keyword)) {
            const Book& book = books[i];
            cout << left << setw(ID_WIDTH) << book.getID()
                 << left << setw(TITLE_WIDTH) << book.getTitle()
                 << left << setw(AUTHOR_WIDTH) << book.getAuthor()
                 << left << setw(CATEGORY_WIDTH) << book.getCategory()
                 << left << setw(YEAR_WIDTH) << book.getPubYear()
                 << left << setw(QUANTITY_WIDTH) << book.getQuantity()
                 << endl;
            foundCount++;
        }
    }

    // In dòng kết thúc
    cout << setfill('-') << setw(TOTAL_WIDTH) << "" << setfill(' ') << endl;

    if (!foundCount) {
        cout << "Khong tim thay sach phu hop.\n";
    } else {
        cout << "Tim thay " << foundCount << " cuon sach.\n";
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
            cout << "Nhap ID sach can xoa: ";
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
        cout << "Thu vien chua co sach nao!\n";
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
    // Sử dụng hàm non-const GetBookByID để lấy con trỏ có thể thay đổi
    Book* book = GetBookByID(id); 
    if (!book) {
        cout << "Khong tim thay sach co ID: " << id << "\n";
        return;
    }

    cout << "\n--- CAP NHAT SACH ID: " << id << " ---\n";
    cout << "--- THONG TIN CU ---\n";
    book->Show(); // Hiển thị thông tin cũ
    cout << "--------------------\n";

    char input[200];
    
    // 1. Cập nhật Tên sách (Title)
    cout << "Nhap Ten sach moi: ";
    cin.ignore(); // Xử lý ký tự newline còn sót
    cin.getline(input, sizeof(input));
    if (input[0] != '\0') {
        book->setTitle(input);
    }
    
    // 2. Cập nhật Tác giả (Author)
    cout << "Nhap Tac gia moi: ";
    cin.getline(input, sizeof(input));
    if (input[0] != '\0') {
        book->setAuthor(input);
    }
    
    // 3. Cập nhật Năm xuất bản
    cout << "Nhap Nam xuat ban moi: ";
    cin.getline(input, sizeof(input));
    if (input[0] != '\0') {
        int newYear = Utils::CharArrayToIntManual(input);
        if (newYear > 0) { 
            book->setPubYear(newYear); // Sửa: Giả định hàm setPubYear() có sẵn
        } else {
            cout << "Canh bao: Nam nhap vao khong hop le hoac <= 0. Giu nguyen gia tri cu.\n";
        }
    }
    
    // 4. Cập nhật Thể loại (Category)
    cout << "Nhap The loai moi: ";
    cin.getline(input, sizeof(input));
    if (input[0] != '\0') {
        book->setCategory(input);
    }

    // (Bạn có thể cần thêm cập nhật Quantity/TotalQuantity)
    
    cout << "\nCap nhat thong tin sach thanh cong!\n";
    SaveBooksToFile(); 
}