-- Active: 1752263647446@@127.0.0.1@3306@salary_manager_db
-- Salary Manager Database Schema
-- C++ MySQL Project

-- Create database
CREATE DATABASE IF NOT EXISTS salary_manager_db 
CHARACTER SET utf8mb4 
COLLATE utf8mb4_unicode_ci;

USE salary_manager_db;

-- Create Users table
CREATE TABLE IF NOT EXISTS users (
    user_id INT AUTO_INCREMENT PRIMARY KEY,
    username VARCHAR(50) UNIQUE NOT NULL,
    password VARCHAR(255) NOT NULL,
    first_name VARCHAR(100) NOT NULL,
    last_name VARCHAR(100) NOT NULL,
    email VARCHAR(255) UNIQUE NOT NULL,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    updated_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP
) ENGINE=InnoDB;

-- Create Transactions table
CREATE TABLE IF NOT EXISTS transactions (
    transaction_id INT AUTO_INCREMENT PRIMARY KEY,
    user_id INT NOT NULL,
    transaction_type ENUM('INCOME', 'EXPENSE') NOT NULL,
    category VARCHAR(100) NOT NULL,
    amount DECIMAL(10, 2) NOT NULL CHECK (amount > 0),
    description TEXT,
    transaction_date DATE NOT NULL,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    updated_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
    
    FOREIGN KEY (user_id) REFERENCES users(user_id) ON DELETE CASCADE,
    INDEX idx_user_id (user_id),
    INDEX idx_transaction_type (transaction_type),
    INDEX idx_transaction_date (transaction_date),
    INDEX idx_user_type (user_id, transaction_type),
    INDEX idx_user_date (user_id, transaction_date)
) ENGINE=InnoDB;

-- Create Categories table (optional for predefined categories)
CREATE TABLE IF NOT EXISTS categories (
    category_id INT AUTO_INCREMENT PRIMARY KEY,
    category_name VARCHAR(100) UNIQUE NOT NULL,
    category_type ENUM('INCOME', 'EXPENSE', 'BOTH') NOT NULL,
    is_active BOOLEAN DEFAULT TRUE,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
) ENGINE=InnoDB;

-- Insert default income categories
INSERT IGNORE INTO categories (category_name, category_type) VALUES
('Salary', 'INCOME'),
('Freelance', 'INCOME'),
('Business', 'INCOME'),
('Investment', 'INCOME'),
('Rental', 'INCOME'),
('Bonus', 'INCOME'),
('Gift', 'INCOME'),
('Other Income', 'INCOME');

-- Insert default expense categories
INSERT IGNORE INTO categories (category_name, category_type) VALUES
('Food & Dining', 'EXPENSE'),
('Transportation', 'EXPENSE'),
('Shopping', 'EXPENSE'),
('Entertainment', 'EXPENSE'),
('Bills & Utilities', 'EXPENSE'),
('Healthcare', 'EXPENSE'),
('Education', 'EXPENSE'),
('Travel', 'EXPENSE'),
('Insurance', 'EXPENSE'),
('Personal Care', 'EXPENSE'),
('Home & Garden', 'EXPENSE'),
('Gifts & Charity', 'EXPENSE'),
('Business Expenses', 'EXPENSE'),
('Other Expenses', 'EXPENSE');

-- Create Views for easy reporting
CREATE OR REPLACE VIEW user_balance_summary AS
SELECT 
    u.user_id,
    u.username,
    u.first_name,
    u.last_name,
    COALESCE(income.total_income, 0) as total_income,
    COALESCE(expenses.total_expenses, 0) as total_expenses,
    COALESCE(income.total_income, 0) - COALESCE(expenses.total_expenses, 0) as balance
FROM users u
LEFT JOIN (
    SELECT user_id, SUM(amount) as total_income
    FROM transactions 
    WHERE transaction_type = 'INCOME'
    GROUP BY user_id
) income ON u.user_id = income.user_id
LEFT JOIN (
    SELECT user_id, SUM(amount) as total_expenses
    FROM transactions 
    WHERE transaction_type = 'EXPENSE'
    GROUP BY user_id
) expenses ON u.user_id = expenses.user_id;

-- Create View for monthly summaries
CREATE OR REPLACE VIEW monthly_summary AS
SELECT 
    user_id,
    YEAR(transaction_date) as year,
    MONTH(transaction_date) as month,
    transaction_type,
    SUM(amount) as total_amount,
    COUNT(*) as transaction_count
FROM transactions
GROUP BY user_id, YEAR(transaction_date), MONTH(transaction_date), transaction_type
ORDER BY user_id, year DESC, month DESC;

-- Create stored procedure for user registration
DELIMITER //
CREATE OR REPLACE PROCEDURE RegisterUser(
    IN p_username VARCHAR(50),
    IN p_password VARCHAR(255),
    IN p_first_name VARCHAR(100),
    IN p_last_name VARCHAR(100),
    IN p_email VARCHAR(255)
)
BEGIN
    DECLARE user_exists INT DEFAULT 0;
    DECLARE email_exists INT DEFAULT 0;
    DECLARE EXIT HANDLER FOR SQLEXCEPTION
    BEGIN
        ROLLBACK;
        RESIGNAL;
    END;

    -- Check if username already exists
    SELECT COUNT(*) INTO user_exists FROM users WHERE username = p_username;
    
    -- Check if email already exists
    SELECT COUNT(*) INTO email_exists FROM users WHERE email = p_email;
    
    IF user_exists > 0 THEN
        SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = 'Username already exists';
    ELSEIF email_exists > 0 THEN
        SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = 'Email already exists';
    ELSE
        START TRANSACTION;
        INSERT INTO users (username, password, first_name, last_name, email)
        VALUES (p_username, p_password, p_first_name, p_last_name, p_email);
        COMMIT;
    END IF;
END //
DELIMITER ;

-- Create function to get user balance
DELIMITER //
CREATE OR REPLACE FUNCTION GetUserBalance(p_user_id INT)
RETURNS DECIMAL(10,2)
READS SQL DATA
DETERMINISTIC
BEGIN
    DECLARE v_balance DECIMAL(10,2) DEFAULT 0;
    
    SELECT COALESCE(SUM(CASE WHEN transaction_type = 'INCOME' THEN amount ELSE -amount END), 0)
    INTO v_balance
    FROM transactions
    WHERE user_id = p_user_id;
    
    RETURN v_balance;
END //
DELIMITER ;
