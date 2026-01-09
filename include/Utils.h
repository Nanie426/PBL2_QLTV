#pragma once
#include <string>
#include <ctime>
#include <vector>

using namespace std;

class Utils {
public:
    static const int MAX_STREAM_SIZE_MANUAL = 100;
    
    static const string RESET;
    static const string RED;
    static const string GREEN;
    static const string YELLOW;
    static const string BLUE;
    static const string MAGENTA;
    static const string CYAN;
    static const string WHITE;
    static const string BLACK;
    static const string BOLD;
    
    static int StringToIntManual(const char* str);
    static int FindSubstringManual(const char* haystack, const char* needle);
    static void CopyStringManual(char* dest, const char* src, size_t dest_size);
    static int SplitLineManual(char* line, char fields[][100], int maxFields, char delimiter);
    static int CharArrayToIntManual(const char* str);
    static void IntToStringManual(int num, char* buffer, size_t buffer_size);
    static void GetCurrentDate(char* buffer, size_t buffer_size);
    static int CompareDates(const char* date1, const char* date2);
    static void GetCurrentDateManual(char* buffer, size_t buffer_size);

    static void ToUpperManual(char* str);
    static void ToLowerManual(char* str);

    static void PrintMenuLine(const string& text, int width = 59);
    static void PrintMenuHeader(const string& title, int width = 59);
    static void PrintMenuBorder(int width = 59);
    static void PrintTableLine(const vector<int>& columnWidths);
    static void PrintInfoRow(const string& label, const string& value, int totalWidth = 61);
    static int GetVisibleLength(const string& str);

    static const int daysInMonth[13];
    static bool AskReturnToMenu();

    static tm ParseDate(const  string& dateStr);
    static  string FormatDate(const tm& timeinfo);
    static tm AddDays(const tm& date, int days);
    static int CompareDates(const tm& date1, const tm& date2);

    static long long GetTotalDays(const char* date);

    static  string GeneratePassword(const  string& lastName, const  string& dob);
};
