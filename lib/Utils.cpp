#include "Utils.h"
#include <ctime>
#include <iostream>
#include <cstdio>
#include <string>
#include <iomanip>

using namespace std;

const int Utils::daysInMonth[13] = {
    0, 31, 28, 31, 30, 31, 30,
    31, 31, 30, 31, 30, 31
};

// ANSI Color Code Definitions
const string Utils::RESET = "\033[0m";
const string Utils::RED = "\033[31m";
const string Utils::GREEN = "\033[32m";
const string Utils::YELLOW = "\033[33m";
const string Utils::BLUE = "\033[34m";
const string Utils::MAGENTA = "\033[35m";
const string Utils::CYAN = "\033[36m";
const string Utils::WHITE = "\033[37m";
const string Utils::BLACK = "\033[30m";
const string Utils::BOLD = "\033[1m";

const int MAX_STREAM_SIZE_MANUAL = 100;
bool Utils::AskReturnToMenu()
{
    char choiceInput[10];
    cout << Utils::CYAN << "\nBan co muon quay ve menu? (Y/N): " << Utils::RESET;

    if (cin.getline(choiceInput, sizeof(choiceInput)).fail()) {
        cin.clear();
        cin.ignore(MAX_STREAM_SIZE_MANUAL, '\n');
        return false;
    }
    return (choiceInput[0] == 'Y' || choiceInput[0] == 'y');
}

int Utils::StringToIntManual(const char* str)
{
    while (*str == ' ' || *str == '\t') str++;

    int res = 0;
    int sign = 1;

    if (*str == '-') { sign = -1; str++; }

    if (!(*str >= '0' && *str <= '9')) return -1;

    while (*str != '\0') {
        if (*str >= '0' && *str <= '9') {
            res = res * 10 + (*str - '0');
        }
        else if (*str == '\n' || *str == '\r') break;
        else return -1;
        str++;
    }
    return res * sign;
}

int Utils::FindSubstringManual(const char* haystack, const char* needle)
{
    if (!*needle) return 0;

    while (*haystack) {
        const char* h = haystack;
        const char* n = needle;

        while (*n && *h == *n) { h++; n++; }

        if (!*n) return 1;

        haystack++;
    }
    return 0;
}

void Utils::CopyStringManual(char* dest, const char* src, size_t dest_size)
{
    size_t i = 0;
    while (i < dest_size - 1 && src[i] != '\0') {
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';
}

int Utils::SplitLineManual(char* line, char fields[][100], int maxFields, char delimiter)
{
    int fieldIndex = 0, charIndex = 0;

    while (*line != '\0' && fieldIndex < maxFields)
    {
        if (*line == delimiter) {
            fields[fieldIndex][charIndex] = '\0';
            fieldIndex++;
            charIndex = 0;
        }
        else {
            if (charIndex < 99) {
                fields[fieldIndex][charIndex] = *line;
                charIndex++;
            }
        }
        line++;
    }
    fields[fieldIndex][charIndex] = '\0';
    return fieldIndex + 1;
}

int Utils::CharArrayToIntManual(const char* str)
{
    int res = 0;
    int sign = 1;

    if (*str == '-') { sign = -1; str++; }

    while (*str != '\0') {
        if (*str >= '0' && *str <= '9') {
            res = res * 10 + (*str - '0');
        }
        else break;
        str++;
    }
    return res * sign;
}

void Utils::IntToStringManual(int num, char* buffer, size_t buffer_size)
{
    if (buffer_size == 0) return;

    if (num == 0) {
        if (buffer_size > 1) {
            buffer[0] = '0';
            buffer[1] = '\0';
        }
        else buffer[0] = '\0';
        return;
    }

    bool neg = false;
    if (num < 0) { neg = true; num = -num; }

    int i = 0;

    while (num && i < buffer_size - 1) {
        buffer[i++] = (num % 10) + '0';
        num /= 10;
    }

    if (neg && i < buffer_size - 1) buffer[i++] = '-';

    buffer[i] = '\0';

    int start = 0, end = i - 1;
    while (start < end) {
        char temp = buffer[start];
        buffer[start] = buffer[end];
        buffer[end] = temp;
        start++; end--;
    }
}

void Utils::GetCurrentDate(char* buffer, size_t buffer_size)
{
    if (buffer_size < 11) { buffer[0] = '\0'; return; }

    time_t now = time(0);
    tm* ltm = localtime(&now);

    sprintf(buffer, "%02d/%02d/%04d",
        ltm->tm_mday,
        ltm->tm_mon + 1,
        1900 + ltm->tm_year
    );
}

void Utils::GetCurrentDateManual(char* buffer, size_t buffer_size)
{
    CopyStringManual(buffer, "19/11/2025", buffer_size);
}

int Utils::CompareDates(const char* date1, const char* date2)
{
    int y1 = CharArrayToIntManual(date1 + 6);
    int y2 = CharArrayToIntManual(date2 + 6);
    if (y1 != y2) return y1 - y2;

    char m1[3] = {date1[3], date1[4], '\0'};
    char m2[3] = {date2[3], date2[4], '\0'};
    int month1 = CharArrayToIntManual(m1);
    int month2 = CharArrayToIntManual(m2);
    if (month1 != month2) return month1 - month2;

    char d1[3] = {date1[0], date1[1], '\0'};
    char d2[3] = {date2[0], date2[1], '\0'};
    int day1 = CharArrayToIntManual(d1);
    int day2 = CharArrayToIntManual(d2);
    return day1 - day2;
}

void Utils::ToUpperManual(char* str)
{
    while (*str) {
        if (*str >= 'a' && *str <= 'z') *str -= 32;
        str++;
    }
}

void Utils::ToLowerManual(char* str)
{
    while (*str) {
        if (*str >= 'A' && *str <= 'Z') *str += 32;
        str++;
    }
}

/* ------------------ HÀM XỬ LÝ NGÀY THÁNG (tm) ------------------ */

tm Utils::ParseDate(const string& dateStr)
{
    tm timeinfo = {};

#ifdef _WIN32
    int d, m, y;
    sscanf_s(dateStr.c_str(), "%d/%d/%d", &d, &m, &y);
    timeinfo.tm_mday = d;
    timeinfo.tm_mon = m - 1;
    timeinfo.tm_year = y - 1900;
#else
    strptime(dateStr.c_str(), "%d/%m/%Y", &timeinfo);
#endif

    mktime(&timeinfo);
    return timeinfo;
}

string Utils::FormatDate(const tm& timeinfo)
{
    char buffer[11];
    strftime(buffer, sizeof(buffer), "%d/%m/%Y", &timeinfo);
    return string(buffer);
}

tm Utils::AddDays(const tm& date, int days)
{
    tm newDate = date;
    newDate.tm_mday += days;
    mktime(&newDate);
    return newDate;
}

int Utils::CompareDates(const tm& date1, const tm& date2)
{
    time_t t1 = mktime(const_cast<tm*>(&date1));
    time_t t2 = mktime(const_cast<tm*>(&date2));

    if (t1 < t2) return -1;
    if (t1 > t2) return 1;
    return 0;
}

long long Utils::GetTotalDays(const char* date) {
    int d = CharArrayToIntManual(date);
    int m = CharArrayToIntManual(date + 3);
    int y = CharArrayToIntManual(date + 6);
    long long days = (long long)y * 365 + y / 4 - y / 100 + y / 400 + (m - 1) * 30 + d;
    return days;
}

string Utils::GeneratePassword(const string& lastName, const string& dob)
{
    string cleanDOB = "";
    for (char c : dob) if (c != '/') cleanDOB += c;

    return lastName + cleanDOB;
}

void Utils::PrintMenuBorder(int width)
{
    cout << "+";
    for (int i = 0; i < width; ++i) cout << "=";
    cout << "+" << endl;
}

void Utils::PrintMenuHeader(const string& title, int width)
{
    cout << "|";
    int padding = (width - title.length()) / 2;
    for (int i = 0; i < padding; ++i) cout << " ";
    cout << title;
    for (int i = 0; i < width - padding - title.length(); ++i) cout << " ";
    cout << "|" << endl;
}

void Utils::PrintMenuLine(const string& text, int width)
{
    cout << "| " << text;
    int spaces = width - 1 - text.length(); 
    for (int i = 0; i < spaces; ++i) cout << " ";
    cout << "|" << endl;
}

void Utils::PrintTableLine(const vector<int>& columnWidths)
{
    int totalWidth = 0;
    for (int width : columnWidths) {
        totalWidth += width + 1; 
    }
    totalWidth -= 1; 
    
    cout << CYAN << setfill('-') << setw(totalWidth) << "" << setfill(' ') << RESET << endl;
}

int Utils::GetVisibleLength(const string& str) {
    int length = 0;
    bool inEscape = false;
    for (char c : str) {
        if (c == '\033') { 
            inEscape = true;
        } else if (inEscape && c == 'm') { 
            inEscape = false;
        } else if (!inEscape) {
            length++;
        }
    }
    return length;
}

void Utils::PrintInfoRow(const string& label, const string& value, int totalWidth)
{
    cout << CYAN << "| " << YELLOW << label << ": " << RESET << value;
    int visibleUsedSpace = 1 + 1 + GetVisibleLength(label) + 1 + 1 + GetVisibleLength(value);
    int spacesNeeded = totalWidth - 1 - visibleUsedSpace;
    if (spacesNeeded > 0) {
        cout << string(spacesNeeded, ' ');
    }
    cout << CYAN << "|" << RESET << endl;
}
