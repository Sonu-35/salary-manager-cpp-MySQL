/*
* Transaction.h - Transaction class declaration
* Handles income and expense transaction operations
*/

#ifndef TRANSACTION_H
#define TRANSACTION_H

#include<string>
using namespace std;

enum class TransactionType{
    INCOME = 1,
    EXPENSE = 2
};

class Transaction{
    private:
        int transactionId;
        int userId;
        TransactionType type;
        double amount;
        string category;
        string description;
        string transactionDate;

    public:
        //constructors
        Transaction();
        Transaction(int userId, TransactionType type, double amount, const string& category, const string& description);
        Transaction(int transactionId, int userId, TransactionType type, double amount, const string& category, const string& description, const string& transactionDate);

        //Destructors
        ~Transaction();

        //Getters
        int getTransactionId() const;
        int getUserId() const;
        TransactionType getType() const;
        double getAmount() const;
        string getCategory() const;
        string getDescription() const;
        string getTransactionDate() const;
        string getTypeString() const;

        //utility methods
        void displayTransaction() const;
        void displayTransactionSummary() const;
        bool validate() const;
        string getValidationErrors() const;

        //static methods
        static string typeToString(TransactionType type);
        static TransactionType stringToType(const string& typeStr);

        //operators
        bool operator == (const Transaction& other) const;
        bool operator < (const Transaction& other) const;
};

#endif