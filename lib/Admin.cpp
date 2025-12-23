#include "Admin.h"
#include "Utils.h"

Admin::Admin() 
    : statsManager(userManager, bookManager, borrowManager) 
{
}

void Admin::StatisticsMenu()
{
    statsManager.ShowStatisticsMenu();
}

void Admin::UserMenu()
{
    int choice;
    char input[10];

    do
    {
        cout << Utils::GREEN << Utils::BOLD;
        Utils::PrintMenuBorder();
        Utils::PrintMenuHeader("QUAN LY NGUOI DUNG");
        Utils::PrintMenuBorder();
        Utils::PrintMenuLine("");
        Utils::PrintMenuLine("1. Them nguoi dung moi");
        Utils::PrintMenuLine("2. Xem tat ca nguoi dung");
        Utils::PrintMenuLine("3. Chinh sua thong tin nguoi dung");
        Utils::PrintMenuLine("4. Thai doi mat khau nguoi dung");
        Utils::PrintMenuLine("5. Xoa nguoi dung");
        Utils::PrintMenuLine("0. Quay lai menu chinh");
        Utils::PrintMenuLine("");
        Utils::PrintMenuBorder();
        cout << Utils::RESET;
        cout << Utils::CYAN << "Chon: " << Utils::RESET;

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
            cout << Utils::CYAN << "Nhap ID nguoi dung can chinh sua: " << Utils::RESET;
            cin >> id;
            cin.ignore(100, '\n');
            userManager.UpdateUserByID(id);
            Utils::AskReturnToMenu();
        }
        else if (choice == 4)
        {
            int id;
            cout << Utils::CYAN << "Nhap ID nguoi dung de thay doi mat khau: " << Utils::RESET;
            cin >> id;
            cin.ignore(100, '\n');
            userManager.ChangeUserPassword(id);
            Utils::AskReturnToMenu();
        }
        else if (choice == 5)
        {
            int id;
            cout << Utils::CYAN << "Nhap ID nguoi dung can xoa: " << Utils::RESET;
            cin >> id;
            cin.ignore(100, '\n');
            userManager.DeleteUserByID(id);
            Utils::AskReturnToMenu();
        }
        else if (choice == 0)
            ; // exit loop
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
        cout << Utils::YELLOW << Utils::BOLD;
        Utils::PrintMenuBorder();
        Utils::PrintMenuHeader("QUAN LY SACH");
        Utils::PrintMenuBorder();
        Utils::PrintMenuLine("");
        Utils::PrintMenuLine("1. Them sach moi");
        Utils::PrintMenuLine("2. Xem tat ca sach");
        Utils::PrintMenuLine("3. Tim kiem sach");
        Utils::PrintMenuLine("4. Chinh sua thong tin sach");
        Utils::PrintMenuLine("5. Xoa sach");
        Utils::PrintMenuLine("0. Quay lai menu chinh");
        Utils::PrintMenuLine("");
        Utils::PrintMenuBorder();
        cout << Utils::RESET;
        cout << Utils::CYAN << "Chon: " << Utils::RESET;

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
                    bookManager.SearchBookByTitle();
                else if (subChoice == 2)
                {
                    char author[100];
                    cout << Utils::CYAN << "Nhap ten tac gia: " << Utils::RESET;
                    cin.getline(author, sizeof(author));
                    bookManager.SearchBookByAuthor(author);
                }
                else if (subChoice == 3)
                {
                    char category[100];
                    cout << Utils::CYAN << "Nhap the loai: " << Utils::RESET;
                    cin.getline(category, sizeof(category));
                    bookManager.SearchBookByCategory(category);
                }
                else if (subChoice == 4)
                {
                    char idStr[20];
                    int bookID;
                    cout << Utils::CYAN << "Nhap ID cuon sach: " << Utils::RESET;
                    cin.getline(idStr, sizeof(idStr));
                    bookID = Utils::CharArrayToIntManual(idStr);
                    const Book *bookPtr = bookManager.GetBookByID(bookID);
                    if (bookPtr)
                    {
                        // Thiết lập độ rộng cột cho bảng
                        const int ID_WIDTH = 8;
                        const int TITLE_WIDTH = 45;
                        const int AUTHOR_WIDTH = 30;
                        const int CATEGORY_WIDTH = 20;
                        const int YEAR_WIDTH = 10;
                        const int QUANTITY_WIDTH = 10;
                        const vector<int> columnWidths = {ID_WIDTH, TITLE_WIDTH, AUTHOR_WIDTH, CATEGORY_WIDTH, YEAR_WIDTH, QUANTITY_WIDTH};

                        cout << Utils::CYAN << Utils::BOLD;
                        Utils::PrintMenuBorder();
                        Utils::PrintMenuHeader("SEARCH RESULT - BOOK ID: " + to_string(bookID));
                        Utils::PrintMenuBorder();
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

                        // In dữ liệu
                        cout << left << setw(ID_WIDTH) << bookPtr->getID() << " "
                             << left << setw(TITLE_WIDTH) << bookPtr->getTitle() << " "
                             << left << setw(AUTHOR_WIDTH) << bookPtr->getAuthor() << " "
                             << left << setw(CATEGORY_WIDTH) << bookPtr->getCategory() << " "
                             << left << setw(YEAR_WIDTH) << bookPtr->getPubYear() << " "
                             << left << setw(QUANTITY_WIDTH) << bookPtr->getQuantity()
                             << endl;

                        // Bottom separator
                        Utils::PrintTableLine(columnWidths);

                        cout << Utils::GREEN << "Tim thay 1 cuon sach." << Utils::RESET << "\n";
                    }
                    else
                        cout << Utils::RED << "Khong tim thay sach co ID " << bookID << Utils::RESET << "\n";
                }
                else if (subChoice == 0)
                    break;
                else
                    cout << "Lua chon khong hop le!\n";

            } while (subChoice != 0);

            break;
        }
        }
        else if (choice == 4)
        {
            int id;
            cout << Utils::CYAN << "Nhap ID sach can sua: " << Utils::RESET;
            cin >> id;
            cin.ignore(100, '\n');
            bookManager.UpdateBookByID(id);
            Utils::AskReturnToMenu();
        }
        else if (choice == 5)
        {
            int id;
            cout << Utils::CYAN << "Nhap ID sach can xoa: " << Utils::RESET;
            cin >> id;
            cin.ignore(100, '\n');
            bookManager.DeleteBookByID(id);
            Utils::AskReturnToMenu();
        }
        else if (choice == 0)
            ; // exit loop
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
        cout << Utils::CYAN << Utils::BOLD;
        Utils::PrintMenuBorder();
        Utils::PrintMenuHeader("QUAN LY MUON/TRA SACH");
        Utils::PrintMenuBorder();
        Utils::PrintMenuLine("");
        Utils::PrintMenuLine("1. Muon sach");
        Utils::PrintMenuLine("2. Tra sach");
        Utils::PrintMenuLine("3. Xem sach da muon");
        Utils::PrintMenuLine("4. Xem lich su giao dich");
        Utils::PrintMenuLine("0. Quay ve menu chinh");
        Utils::PrintMenuLine("");
        Utils::PrintMenuBorder();
        cout << Utils::RESET;
        cout << Utils::CYAN << "Chon: " << Utils::RESET;

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
                    cout << Utils::CYAN << "\nNhap ID doc gia de xem chi tiet (nhap 0 de thoat): " << Utils::RESET;
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
            ; // exit loop
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
        cout << Utils::MAGENTA << Utils::BOLD;
        Utils::PrintMenuBorder();
        Utils::PrintMenuHeader("HE THONG QUAN LY THU VIEN - ADMIN");
        Utils::PrintMenuBorder();
        Utils::PrintMenuLine("");
        Utils::PrintMenuLine("1. Quan ly nguoi dung");
        Utils::PrintMenuLine("2. Quan ly sach");
        Utils::PrintMenuLine("3. Quan ly muon/tra");
        Utils::PrintMenuLine("4. Thong ke & Bao cao");
        Utils::PrintMenuLine("0. Dang xuat");
        Utils::PrintMenuLine("");
        Utils::PrintMenuBorder();
        cout << Utils::RESET;
        cout << Utils::CYAN << "Chon: " << Utils::RESET;

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
            cout << Utils::YELLOW << "Dang xuat khoi Admin Panel...\n" << Utils::RESET;
        else
            cout << Utils::RED << "Lua chon khong hop le! Vui long chon mot lua chon hop le.\n" << Utils::RESET;

    } while (choice != 0);
}