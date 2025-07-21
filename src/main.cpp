/*
* Salary Manager - C++ MySQL Project
* Main Entry Point
* Author: Sonu Sharma
* Date: July 2025
*/

#include<iostream>
#include<memory>
#include<exception>
#include "SalaryManager.h"
#include "Utils.h"

using namespace std;

int main(){
    try{
        //clear screen and show welcome message
        Utils::clearScreen();
        Utils::displayWelcomeMessage();

        //Initialize the Salary Manager System
        unique_ptr<SalaryManager> manager = make_unique<SalaryManager>();

        //start the main application loop
        manager -> run();

        cout << "\n Thank you for using Salary Manager!" << endl;

    } catch(const exception& e){
        cout << "Fatal error" << e.what() << endl;
        return 1;
    }
    return 0;
}