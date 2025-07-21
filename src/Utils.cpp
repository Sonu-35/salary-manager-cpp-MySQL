#include "Utils.h"
#include <iostream>
#include <cstdlib>
#include <chrono>
#include <ctime>

using namespace std;

void Utils::clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void Utils::pressEnterToContinue() {
    cout << "\nPress Enter to continue...";
    cin.ignore();
    cin.get();
}

string Utils::getCurrentDateTime() {
    auto now = chrono::system_clock::now();
    time_t tt = chrono::system_clock::to_time_t(now);
    char buf[100];
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", localtime(&tt));
    return string(buf);
}

void Utils::displayWelcomeMessage() {
    cout << "=============================\n";
    cout << "  SALARY MANAGER (C++ + SQL)\n";
    cout << "=============================\n";
}
