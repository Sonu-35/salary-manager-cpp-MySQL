/*
 * SalaryManager.h - Main application class
 * Core application logic and user interface
 */

#ifndef SALARYMANAGER_H
#define SALARYMANAGER_H

#include <memory>
#include <vector>
#include "Database.h"
#include "User.h"
#include "Transaction.h"

using namespace std;

class SalaryManager {
private:
    unique_ptr<Database> database;
    unique_ptr<User> currentUser;
    
    // Menu methods
    void showMainMenu();
    void showUserMenu();
    void showTransactionMenu();
    void showReportsMenu();
    
    // User management methods
    void createNewUser();
    void selectUser();
    void viewAllUsers();
    void viewCurrentUser();
    
    // Transaction management methods
    void addIncomeTransaction();
    void addExpenseTransaction();
    void viewAllTransactions();
    
    // Report methods
    void viewBalanceSummary();
    void viewCategoryWiseReport();
    
    // Utility methods
    void displayUserBalance();
    void displayQuickStats();
    bool confirmAction(const string& message);
    void handleDatabaseError(const string& operation);
    string getTransactionCategory(TransactionType type);
    
    // Constants for categories
    static const vector<string> INCOME_CATEGORIES;
    static const vector<string> EXPENSE_CATEGORIES;

public:
    // Constructor & Destructor
    SalaryManager();
    ~SalaryManager();
    
    // Main application methods
    bool initialize();
    void run();
    void shutdown();
};

#endif // SALARYMANAGER_H
