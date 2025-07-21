#include "DatabaseConnection.h"
#include "Utils.h"

#include <iostream>
#include <stdexcept>

using namespace std;

DatabaseConnection::DatabaseConnection(const string& host, const string& username,
                                       const string& password, const string& database)
    : host(host), username(username), password(password), database(database), 
      driver(nullptr), connection(nullptr) {
}

DatabaseConnection::~DatabaseConnection() {
    disconnect();
}

bool DatabaseConnection::connect() {
    try {
        driver = sql::mysql::get_mysql_driver_instance();

        string connectionString = "tcp://" + host + ":3306";
        connection.reset(driver->connect(connectionString, username, password));

        if (connection) {
            string createDBQuery = "CREATE DATABASE IF NOT EXISTS " + database;
            executeQuery(createDBQuery);

            connection->setSchema(database);
            cout << "Successfully connected to MySQL database: " << database << endl;
            return true;
        }
    } catch (sql::SQLException& e) {
        cerr << "Database connection error: " << e.what() << endl;
        cerr << "MySQL error code: " << e.getErrorCode() << endl;
        cerr << "SQLState: " << e.getSQLState() << endl;
        return false;
    } catch (exception& e) {
        cerr << "General error: " << e.what() << endl;
        return false;
    }

    return false;
}

bool DatabaseConnection::disconnect() {
    try {
        if (connection) {
            connection->close();
            connection.reset();
            cout << "Database connection closed." << endl;
            return true;
        }
    } catch (sql::SQLException& e) {
        cerr << "Error closing database connection: " << e.what() << endl;
        return false;
    }

    return true;
}

bool DatabaseConnection::isConnected() const {
    return connection && !connection->isClosed();
}

sql::Connection* DatabaseConnection::getConnection() const {
    return connection.get();
}

bool DatabaseConnection::executeQuery(const string& query) {
    try {
        if (!isConnected()) {
            cerr << "Database not connected!" << endl;
            return false;
        }

        unique_ptr<sql::Statement> stmt(connection->createStatement());
        stmt->execute(query);
        return true;
    } catch (sql::SQLException& e) {
        cerr << "Query execution error: " << e.what() << endl;
        cerr << "Query: " << query << endl;
        return false;
    }
}

unique_ptr<sql::ResultSet> DatabaseConnection::executeSelectQuery(const string& query) {
    try {
        if (!isConnected()) {
            cerr << "Database not connected!" << endl;
            return nullptr;
        }

        unique_ptr<sql::Statement> stmt(connection->createStatement());
        return unique_ptr<sql::ResultSet>(stmt->executeQuery(query));
    } catch (sql::SQLException& e) {
        cerr << "Select query execution error: " << e.what() << endl;
        cerr << "Query: " << query << endl;
        return nullptr;
    }
}

unique_ptr<sql::PreparedStatement> DatabaseConnection::prepareStatement(const string& query) {
    try {
        if (!isConnected()) {
            cerr << "Database not connected!" << endl;
            return nullptr;
        }

        return unique_ptr<sql::PreparedStatement>(connection->prepareStatement(query));
    } catch (sql::SQLException& e) {
        cerr << "Prepared statement creation error: " << e.what() << endl;
        return nullptr;
    }
}

bool DatabaseConnection::createTables() {
    try {
        string createUsersTable = R"(
            CREATE TABLE IF NOT EXISTS users (
                user_id INT AUTO_INCREMENT PRIMARY KEY,
                username VARCHAR(50) UNIQUE NOT NULL,
                password VARCHAR(255) NOT NULL,
                first_name VARCHAR(100) NOT NULL,
                last_name VARCHAR(100) NOT NULL,
                email VARCHAR(255) UNIQUE NOT NULL,
                created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
            )
        )";

        if (!executeQuery(createUsersTable)) {
            cerr << "Failed to create users table" << endl;
            return false;
        }

        string createTransactionsTable = R"(
            CREATE TABLE IF NOT EXISTS transactions (
                transaction_id INT AUTO_INCREMENT PRIMARY KEY,
                user_id INT NOT NULL,
                transaction_type ENUM('INCOME', 'EXPENSE') NOT NULL,
                category VARCHAR(100) NOT NULL,
                amount DECIMAL(10, 2) NOT NULL,
                description TEXT,
                transaction_date DATE NOT NULL,
                created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
                FOREIGN KEY (user_id) REFERENCES users(user_id) ON DELETE CASCADE
            )
        )";

        if (!executeQuery(createTransactionsTable)) {
            cerr << "Failed to create transactions table" << endl;
            return false;
        }

        cout << "Database tables created successfully!" << endl;
        return true;
    } catch (sql::SQLException& e) {
        cerr << "Error creating tables: " << e.what() << endl;
        return false;
    }
}

void DatabaseConnection::testConnection() {
    try {
        if (isConnected()) {
            auto result = executeSelectQuery("SELECT 'Database connection successful!' as message");
            if (result && result->next()) {
                cout << "Test result: " << result->getString("message") << endl;
            }
        } else {
            cout << "Database connection test failed - not connected" << endl;
        }
    } catch (sql::SQLException& e) {
        cerr << "Connection test error: " << e.what() << endl;
    }
}
