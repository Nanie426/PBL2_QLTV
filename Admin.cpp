#include "Admin.h"

// THÊM CONSTRUCTOR
Admin::Admin() 
    : statsManager(userManager, bookManager, borrowManager) 
{
}

// THÊM HÀM MENU THỐNG KÊ
void Admin::StatisticsMenu()
{
    statsManager.ShowStatisticsMenu();
}

// bool Admin::AskReturnToMenu()
// {
//     char choice;
//     cout << "\nBan co muon quay lai MENU khong? (Y/N): ";
//     choice = cin.get(); 
//     if (choice == '\n') {
//         return true; 
//     }
//     cin.ignore(100, '\n'); 

//     if (choice == 'Y' || choice == 'y')
//         return true;
//     return false;
// }

void Admin::UserMenu()
{
    int choice;
    char input[10];

    do
    {
        cout << "\n========= QUAN LY NGUOI DUNG =========\n";
        cout << "1. Them nguoi dung\n";
        cout << "2. Xem danh sach nguoi dung\n";
        cout << "3. Sua thong tin nguoi dung\n";
        cout << "4. Xoa nguoi dung\n";
        cout << "0. Quay lai MENU chinh\n";
        cout << "Chon: ";

        cin.getline(input, 10);
        choice = Utils::CharArrayToIntManual(input);

        if (choice == 1)
            userManager.AddUser();
        else if (choice == 2)
        {
            userManager.ShowAllUsers();
            Utils::AskReturnToMenu();
        }
        else if (choice == 3)
        {
            int id;
            cout << "Nhap ID nguoi dung can sua: ";
            cin >> id;
            cin.ignore(100, '\n');
            userManager.UpdateUserByID(id);
            Utils::AskReturnToMenu();
        }
        else if (choice == 4)
        {
            int id;
            cout << "Nhap ID nguoi dung can xoa: ";
            cin >> id;
            cin.ignore(100, '\n');
            userManager.DeleteUserByID(id);
            Utils::AskReturnToMenu();
        }
        else if (choice == 0)
            break;
        else
            cout << "Lua chon khong hop le!\n";

    } while (choice != 0);
}

void Admin::BookMenu()
{
    int choice;
    char input[10];

    do
    {
        cout << "\n========= QUAN LY SACH =========\n";
        cout << "1. Them sach\n";
        cout << "2. Xem danh sach sach\n";
        cout << "3. Tim sach\n";
        cout << "4. Sua thong tin sach\n";
        cout << "5. Xoa sach\n";
        cout << "0. Quay lai MENU chinh\n";
        cout << "Chon: ";

        cin.getline(input, 10);
        choice = Utils::CharArrayToIntManual(input);

        if (choice == 1)
            bookManager.AddBook();
        else if (choice == 2)
        {
                bookManager.ShowAllBooks();
                Utils::AskReturnToMenu();
        }
        else if (choice == 3)
        {
            {
            int subChoice;
            char subInput[10];
            do
            {
                cout << "\n--- MENU TIM KIEM SACH ---\n";
                cout << "1. Theo ten\n";
                cout << "2. Theo tac gia\n";
                cout << "3. Theo the loai\n";
                cout << "4. Theo ID\n";
                cout << "0. Quay lai\n";
                cout << "Chon: ";
                cin.getline(subInput, sizeof(subInput));
                subChoice = Utils::StringToIntManual(subInput);

                if (subChoice == 1)
                    bookManager.SearchBookByTitle();
                else if (subChoice == 2)
                {
                    char author[100];
                    cout << "Nhap ten tac gia: ";
                    cin.getline(author, sizeof(author));
                    bookManager.SearchBookByAuthor(author);
                }
                else if (subChoice == 3)
                {
                    char category[100];
                    cout << "Nhap the loai: ";
                    cin.getline(category, sizeof(category));
                    bookManager.SearchBookByCategory(category);
                }
                else if (subChoice == 4)
                {
                    char idStr[20];
                    int bookID;
                    cout << "Nhap ID sach: ";
                    cin.getline(idStr, sizeof(idStr));
                    bookID = Utils::CharArrayToIntManual(idStr);
                    const Book *bookPtr = bookManager.GetBookByID(bookID);
                    if (bookPtr)
                    {
                        cout << "\n--- KET QUA TIM KIEM ID: " << bookID << " ---\n";
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

                        // In dữ liệu
                        cout << left << setw(ID_WIDTH) << bookPtr->getID()
                             << left << setw(TITLE_WIDTH) << bookPtr->getTitle()
                             << left << setw(AUTHOR_WIDTH) << bookPtr->getAuthor()
                             << left << setw(CATEGORY_WIDTH) << bookPtr->getCategory()
                             << left << setw(YEAR_WIDTH) << bookPtr->getPubYear()
                             << left << setw(QUANTITY_WIDTH) << bookPtr->getQuantity()
                             << endl;

                        // In dòng kết thúc
                        cout << setfill('-') << setw(TOTAL_WIDTH) << "" << setfill(' ') << endl;
                        cout << "Tim thay 1 cuon sach.\n";
                    }
                    else
                        cout << "Khong tim thay sach co ID " << bookID << "\n";
                }
                else if (subChoice == 0)
                    break;
                else
                    cout << "Lua chon khong hop le!\n";

            } while (subChoice != 0);

            Utils::AskReturnToMenu();
            break;
        }
        }
        else if (choice == 4)
        {
            int id;
            cout << "Nhap ID sach can sua: ";
            cin >> id;
            cin.ignore(100, '\n');
            bookManager.UpdateBookByID(id);
            Utils::AskReturnToMenu();
        }
        else if (choice == 5)
        {
            int id;
            cout << "Nhap ID sach can xoa: ";
            cin >> id;
            cin.ignore(100, '\n');
            bookManager.DeleteBookByID(id);
            Utils::AskReturnToMenu();
        }
        else if (choice == 0)
            break;
        else
            cout << "Lua chon khong hop le!\n";

    } while (choice != 0);
}

void Admin::BorrowBookMenu()
{
    int choice;
    char input[10];

    do
    {
        cout << "\n========= QUAN LY MUON/TRA SACH =========\n";
        cout << "1. Them thong tin muon sach\n";
        cout << "2. Ghi nhan tra sach\n";
        cout << "3. Xem sach nguoi dung dang muon\n";
        cout << "4. Xem lich su muon sach\n";
        cout << "0. Quay lai MENU chinh\n";
        cout << "Chon: ";

        cin.getline(input, 10);
        choice = Utils::CharArrayToIntManual(input);

        if (choice == 1)
        {
            borrowManager.HandleBorrowBook(userManager, bookManager);
            Utils::AskReturnToMenu();
        }
        else if (choice == 2)
        {
            borrowManager.HandleReturnBook(userManager, bookManager);
            Utils::AskReturnToMenu();
        }
        else if (choice == 3)
        {
            
            borrowManager.ShowActiveAndOverdueBorrows(userManager, bookManager);
            Utils::AskReturnToMenu();
        }
        else if (choice == 4)
        {
            borrowManager.ShowAllUsersTransactionHistory(userManager, bookManager);
            
            char viewDetailChoice;
            cout << "\nBan co muon xem chi tiet lich su cua tung doc gia? (y/n): ";
            cin >> viewDetailChoice;
            cin.ignore();
            
            if (viewDetailChoice == 'y' || viewDetailChoice == 'Y') {
                do {
                    int userID;
                    cout << "\nNhap ID doc gia can xem chi tiet (nhap 0 de thoat): ";
                    if (!(cin >> userID)) {
                        cout << "ID khong hop le.\n";
                        cin.clear();
                        cin.ignore(100, '\n');
                        continue;
                    }
                    cin.ignore(100, '\n');
                    
                    if (userID == 0) {
                        break;
                    }
                    
                    User user;
                    user.LoadUserByID(to_string(userID));
                    
                    if (user.getID() == 0) {
                        cout << "Khong tim thay doc gia voi ID: " << userID << "\n";
                    } else {
                        cout << "\n=== LICH SU CHI TIET DOC GIA: " << user.getName() << " (ID: " << userID << ") ===\n";
                        user.ShowTransactionHistory(bookManager);
                    }
                    
                } while (true);
            }
            
            Utils::AskReturnToMenu();
        }
        else if (choice == 0)
            break;
        else
            cout << "Lua chon khong hop le!\n";

    } while (choice != 0);
}

void Admin::Menu()
{
    int choice;
    char input[10];

    do
    {
        cout << "\n========= HE THONG QUAN LY ADMIN =========\n";
        cout << "1. Quan ly nguoi dung\n";
        cout << "2. Quan ly sach\n";
        cout << "3. Quan ly muon/tra sach\n";
        cout << "4. Thong ke\n"; // SỬA: GỌI MENU THỐNG KÊ
        cout << "0. Thoat\n";
        cout << "Chon: ";

        cin.getline(input, 10);
        choice = Utils::CharArrayToIntManual(input);

        if (choice == 1)
            UserMenu();
        else if (choice == 2)
            BookMenu();
        else if (choice == 3)
            BorrowBookMenu();
        else if (choice == 4)
        {
            StatisticsMenu();
        }
        else if (choice == 0)
            cout << "Thoat chuong trinh.\n";
        else
            cout << "Lua chon khong hop le!\n";

    } while (choice != 0);
}