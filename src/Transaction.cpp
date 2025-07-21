#include "Transaction.h"
#include <iostream>
#include <iomanip>

using namespace std;

Transaction::Transaction()
    : transactionId(-1), userId(-1), amount(0.0), type(TransactionType::EXPENSE) {}

Transaction::Transaction(int userId, TransactionType type, double amount,
                         const string& category, const string& description)
    : userId(userId), type(type), amount(amount),
      category(category), description(description) {}

Transaction::Transaction(int transactionId, int userId, TransactionType type, double amount,
                         const string& category, const string& description,
                         const string& transactionDate)
    : transactionId(transactionId), userId(userId), type(type), amount(amount),
      category(category), description(description), transactionDate(transactionDate) {}

Transaction::~Transaction() {}

int Transaction::getTransactionId() const { return transactionId; }
int Transaction::getUserId() const { return userId; }
TransactionType Transaction::getType() const { return type; }
double Transaction::getAmount() const { return amount; }
string Transaction::getCategory() const { return category; }
string Transaction::getDescription() const { return description; }
string Transaction::getTransactionDate() const { return transactionDate; }

string Transaction::getTypeString() const {
    return type == TransactionType::INCOME ? "INCOME" : "EXPENSE";
}

string Transaction::typeToString(TransactionType type) {
    return type == TransactionType::INCOME ? "INCOME" : "EXPENSE";
}

TransactionType Transaction::stringToType(const string& str) {
    return (str == "INCOME") ? TransactionType::INCOME : TransactionType::EXPENSE;
}

void Transaction::displayTransaction() const {
    cout << "[" << transactionId << "] " << getTypeString()
         << " | ₹" << fixed << setprecision(2) << amount
         << " | Category: " << category
         << " | Date: " << transactionDate
         << " | " << description << endl;
}

void Transaction::displayTransactionSummary() const {
    cout << transactionDate << " - "
         << getTypeString() << ": ₹" << amount
         << " (" << category << ")" << endl;
}

bool Transaction::validate() const {
    return amount > 0 && !category.empty();
}

string Transaction::getValidationErrors() const {
    string err;
    if (amount <= 0) err += "Amount must be greater than zero.\n";
    if (category.empty()) err += "Category is required.\n";
    return err;
}

bool Transaction::operator==(const Transaction& other) const {
    return this->transactionId == other.transactionId;
}

bool Transaction::operator<(const Transaction& other) const {
    return this->transactionDate < other.transactionDate;
}
