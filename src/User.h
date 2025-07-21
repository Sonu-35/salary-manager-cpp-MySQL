/*
* User.h - User Class declaration
* Handles user account information and operations 
*/

#ifndef USER_H
#define USER_H

#include <string>
#include <vector>

using namespace std;

class User{
    private: 
        int userId;
        string username;
        string firstName;
        string lastName;
        string email;
        string phoneNumber;
        double currentBalance;
        string createdDate;
    
    public:
        //constructor
        User();
         User(const string& username, const string& firstName, const string& lastName, 
         const string& email, const string& phoneNumber);
        User(int userId, const string& username, const string& firstName, 
         const string& lastName, const string& email, const string& phoneNumber,
         double currentBalance, const string& createdDate);
        
        //destructor
        ~User();

        //Getters
        int getUserId() const;
        string getUsername() const;
        string getFirstName() const;
        string getLastName() const;
        string getEmail() const;
        string getPhoneNumber() const;
        double getCurrentBalance() const;
        string getCreatedDate() const;
        string getFullName() const;

        //setters and utility methods
        void setUserId(int userId);
        void displayUserInfo() const;
        void displayUserSummary() const;
        bool validate() const;
        string getValidationErrors() const;

        //operators
        bool operator == (const User& other) const;
        bool operator != (const User& other) const;
};

#endif