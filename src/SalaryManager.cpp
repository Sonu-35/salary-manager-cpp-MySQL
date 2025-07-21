#include "SalaryManager.h"
#include "Utils.h"
#include <iostream>
#include <limits>

using namespace std;

const vector<string> SalaryManager::INCOME_CATEGORIES = {"Salary", "Bonus", "Business", "Other"};
const vector<string> SalaryManager::EXPENSE_CATEGORIES = {"Food", "Transport", "Bills", "Entertainment", "Other"};

SalaryManager::SalaryManager() {
    database = make_unique<Database>();
}

SalaryManager::~SalaryManager() {
    shutdown();
}

bool SalaryManager::initialize() {
    return database->connect();
}

void SalaryManager::run() {
    if (!initialize()) {
        cout << "Failed to connect to database. Exiting...\n";
        return;
    }

    int choice = 0;
    do {
        showMainMenu();
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore(); // flush newline

        switch (choice) {
            case 1: createNewUser(); break;
            case 2: selectUser(); break;
            case 3: viewAllUsers(); break;
            case 0: shutdown(); break;
            default: cout << "Invalid option.\n";
        }

        Utils::pressEnterToContinue();
    } while (choice != 0);
}

void SalaryManager::showMainMenu() {
    Utils::clearScreen();
    cout << "==== MAIN MENU ====\n";
    cout << "1. Create New User\n";
    cout << "2. Select Existing User\n";
    cout << "3. View All Users\n";
    cout << "0. Exit\n";
}

void SalaryManager::showUserMenu() {
    int option;
    do {
        Utils::clearScreen();
        cout << "==== USER DASHBOARD (" << currentUser->getFullName() << ") ====\n";
        cout << "1. View User Details\n";
        cout << "2. View Balance Summary\n";
        cout << "3. Add Income\n";
        cout << "4. Add Expense\n";
        cout << "5. View Transactions\n";
        cout << "6. Category-wise Report\n";
        cout << "0. Logout\n";

        cout << "\nEnter choice: ";
        cin >> option;
        cin.ignore();

        switch (option) {
            case 1: viewCurrentUser(); break;
            case 2: viewBalanceSummary(); break;
            case 3: addIncomeTransaction(); break;
            case 4: addExpenseTransaction(); break;
            case 5: viewAllTransactions(); break;
            case 6: viewCategoryWiseReport(); break;
            case 0: currentUser = nullptr; break;
            default: cout << "Invalid option.\n";
        }

        Utils::pressEnterToContinue();
    } while (option != 0);
}

void SalaryManager::createNewUser() {
    Utils::clearScreen();
    string username, firstName, lastName, email, phone;
    
    cout << "Enter Username: ";
    getline(cin, username);
    cout << "First Name: ";
    getline(cin, firstName);
    cout << "Last Name: ";
    getline(cin, lastName);
    cout << "Email: ";
    getline(cin, email);
    cout << "Phone Number: ";
    getline(cin, phone);

    User user(username, firstName, lastName, email, phone);

    if (!user.validate()) {
        cout << "Error:\n" << user.getValidationErrors();
        return;
    }

    if (database->userExists(username)) {
        cout << "Error: Username already exists!\n";
        return;
    }

    if (database->createUser(user)) {
        cout << "User created successfully!\n";
    } else {
        handleDatabaseError("creating user");
    }
}

void SalaryManager::selectUser() {
    Utils::clearScreen();
    string username;
    cout << "Enter username to login: ";
    getline(cin, username);

    unique_ptr<User> user = database->getUserByUsername(username);
    if (!user) {
        cout << "User not found.\n";
        return;
    }

    currentUser = move(user);
    showUserMenu();
}

void SalaryManager::viewAllUsers() {
    Utils::clearScreen();
    cout << "All Registered Users:\n";

    vector<unique_ptr<User>> users = database->getAllUsers();
    for (const auto& user : users) {
        user->displayUserSummary();
    }
}

void SalaryManager::viewCurrentUser() {
    Utils::clearScreen();
    if (currentUser) currentUser->displayUserInfo();
    else cout << "No user selected.\n";
}

void SalaryManager::addIncomeTransaction() {
    Utils::clearScreen();
    double amount;
    string category, description;

    cout << "Enter amount: ";
    cin >> amount;
    cin.ignore();

    category = getTransactionCategory(TransactionType::INCOME);
    cout << "Description (optional): ";
    getline(cin, description);

    Transaction txn(currentUser->getUserId(), TransactionType::INCOME, amount, category, description);

    if (!txn.validate()) {
        cout << "Transaction invalid: " << txn.getValidationErrors();
        return;
    }

    if (database->addTransaction(txn)) {
        cout << "Income added successfully.\n";
    } else {
        handleDatabaseError("adding income");
    }
}

void SalaryManager::addExpenseTransaction() {
    Utils::clearScreen();
    double amount;
    string category, description;

    cout << "Enter amount: ";
    cin >> amount;
    cin.ignore();

    category = getTransactionCategory(TransactionType::EXPENSE);
    cout << "Description (optional): ";
    getline(cin, description);

    Transaction txn(currentUser->getUserId(), TransactionType::EXPENSE, amount, category, description);

    if (!txn.validate()) {
        cout << "Transaction invalid: " << txn.getValidationErrors();
        return;
    }

    if (database->addTransaction(txn)) {
        cout << "Expense added successfully.\n";
    } else {
        handleDatabaseError("adding expense");
    }
}

void SalaryManager::viewAllTransactions() {
    Utils::clearScreen();
    vector<unique_ptr<Transaction>> txns = database->getTransactionsByUserId(currentUser->getUserId());

    if (txns.empty()) {
        cout << "No transactions found.\n";
        return;
    }

    for (const auto& txn : txns) {
        txn->displayTransaction();
    }
}

void SalaryManager::viewBalanceSummary() {
    double income = database->calculateUserIncome(currentUser->getUserId());
    double expense = database->calculateUserExpenses(currentUser->getUserId());
    double balance = income - expense;

    Utils::clearScreen();
    cout << "--- BALANCE REPORT ---\n";
    cout << "Total Income: ₹" << income << endl;
    cout << "Total Expenses: ₹" << expense << endl;
    cout << "Current Balance: ₹" << balance << endl;
}

void SalaryManager::viewCategoryWiseReport() {
    Utils::clearScreen();
    auto results = database->getCategoryWiseExpenses(currentUser->getUserId());
    if (results.empty()) {
        cout << "No data available.\n";
        return;
    }

    cout << "--- EXPENSES BY CATEGORY ---\n";
    for (const auto& row : results) {
        cout << row.first << ": ₹" << row.second << endl;
    }
}

void SalaryManager::shutdown() {
    if (database) {
        database->disconnect();
    }
    cout << "\nGoodbye!\n";
}

bool SalaryManager::confirmAction(const string& message) {
    char response;
    cout << message << " (Y/N)? ";
    cin >> response;
    cin.ignore();
    return (response == 'Y' || response == 'y');
}

void SalaryManager::handleDatabaseError(const string& operation) {
    cout << "Database error while " << operation << ": "
         << database->getLastError() << endl;
}

string SalaryManager::getTransactionCategory(TransactionType type) {
    const vector<string>& categories = (type == TransactionType::INCOME) ? INCOME_CATEGORIES : EXPENSE_CATEGORIES;
    int choice;

    cout << "\nSelect Category:\n";
    for (int i = 0; i < categories.size(); ++i) {
        cout << i + 1 << ". " << categories[i] << endl;
    }
    cout << "Enter choice: ";
    cin >> choice;
    cin.ignore();

    if (choice >= 1 && choice <= categories.size())
        return categories[choice - 1];
    return "Other";
}
