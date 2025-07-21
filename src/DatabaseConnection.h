#ifndef DATABASE_CONNECTION_H
#define DATABASE_CONNECTION_H

#include <mysql_connection.h>
#include <mysql_driver.h>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>

#include <string>
#include <memory>

using namespace std;

class DatabaseConnection {
private:
    sql::mysql::MySQL_Driver* driver;
    unique_ptr<sql::Connection> connection;
    string host;
    string username; 
    string password;
    string database;
    
public:
    DatabaseConnection(const string& host, const string& username, 
                      const string& password, const string& database);
    ~DatabaseConnection();
    
    bool connect();
    bool disconnect();
    bool isConnected() const;
    
    sql::Connection* getConnection() const;
    bool executeQuery(const string& query);
    unique_ptr<sql::ResultSet> executeSelectQuery(const string& query);
    unique_ptr<sql::PreparedStatement> prepareStatement(const string& query);
    
    bool createTables();
    void testConnection();
};

#endif // DATABASE_CONNECTION_H
