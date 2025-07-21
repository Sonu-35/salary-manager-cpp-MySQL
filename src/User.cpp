#include "User.h"
#include <iostream>
#include <regex>

using namespace std;

User::User() : userId(-1), currentBalance(0.0) {}

User::User(const string& username, const string& firstName, const string& lastName,
           const string& email, const string& phoneNumber)
    : username(username), firstName(firstName), lastName(lastName),
      email(email), phoneNumber(phoneNumber), currentBalance(0.0) {}

User::User(int userId, const string& username, const string& firstName,
           const string& lastName, const string& email, const string& phoneNumber,
           double currentBalance, const string& createdDate)
    : userId(userId), username(username), firstName(firstName), lastName(lastName),
      email(email), phoneNumber(phoneNumber), currentBalance(currentBalance), createdDate(createdDate) {}

User::~User() {}

int User::getUserId() const { return userId; }
string User::getUsername() const { return username; }
string User::getFirstName() const { return firstName; }
string User::getLastName() const { return lastName; }
string User::getEmail() const { return email; }
string User::getPhoneNumber() const { return phoneNumber; }
double User::getCurrentBalance() const { return currentBalance; }
string User::getCreatedDate() const { return createdDate; }
string User::getFullName() const { return firstName + " " + lastName; }

void User::setUserId(int id) { userId = id; }

void User::displayUserInfo() const {
    cout << "\n--- User Details ---\n";
    cout << "User ID: " << userId << endl;
    cout << "Username: " << username << endl;
    cout << "Name: " << getFullName() << endl;
    cout << "Email: " << email << endl;
    cout << "Phone: " << phoneNumber << endl;
    cout << "Balance: ₹" << currentBalance << endl;
    cout << "Created: " << createdDate << endl;
}

void User::displayUserSummary() const {
    cout << "[" << userId << "] " << getFullName()
         << " | Balance: ₹" << currentBalance << endl;
}

bool User::validate() const {
    return !username.empty() && !email.empty() && !phoneNumber.empty();
}

string User::getValidationErrors() const {
    string errors;
    if (username.empty()) errors += "Username is required.\n";
    if (email.find('@') == string::npos) errors += "Email must contain '@'.\n";
    if (phoneNumber.length() < 10) errors += "Invalid phone number length.\n";
    return errors;
}

bool User::operator==(const User& other) const {
    return this->userId == other.userId;
}

bool User::operator!=(const User& other) const {
    return !(*this == other);
}
