#pragma once
#include "UserManager.h"
#include "BookManager.h"
#include "BorrowManager.h"
#include "Utils.h"

class StatisticsManager {
private:
    UserManager& userManager;
    BookManager& bookManager;
    BorrowManager& borrowManager;

public:
     StatisticsManager(UserManager& um, BookManager& bm, BorrowManager& bmgr);

    void ShowStatisticsMenu();

    void ThongKeSach();
    void ThongKeTongQuanSach();
    void ThongKeTheoTheLoai();
    void ThongKeTheoTacGia();
    void ThongKeTinhTrangMuon();
    void ThongKeSachMuonNhieu();

    void ThongKeDocGia();

    void ThongKeMuonTra();

    void BaoCaoTongHop();
};
