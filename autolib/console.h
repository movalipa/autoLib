#pragma once
#include <Windows.h>

// color codes
#define BLACK        0
#define BLUE         1
#define GREEN        2
#define CYAN         3
#define RED          4
#define MAGENTA      5
#define BROWN        6
#define LIGHTGREY    7
#define DARKGREY     8
#define LIGHTBLUE    9
#define LIGHTGREEN   10
#define LIGHTCYAN    11
#define LIGHTRED     12
#define LIGHTMAGENTA 13
#define YELLOW       14
#define WHITE        15
#define BLINKERSTART "\033[5m"
#define BLINKEREND   "\033[0m"


int getConsoleRows() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    return csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
}

int getConsoleColumns() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    return csbi.srWindow.Right - csbi.srWindow.Left + 1;
}

void gotoxy(int x, int y)
{
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void clearConsole() {
    system("cls");
}

void setConsoleColor(int colorCode) {
    HANDLE cc;
    cc = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(cc, colorCode);
}

bool ensureConsoleSize() {
    bool check1 = false, check2 = false;
    if (getConsoleRows() >= 32)
        check1 = true;
    if (getConsoleColumns() >= 130)
        check2 = true;
    return check1 && check2;
}