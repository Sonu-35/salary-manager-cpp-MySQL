-- Active: 1752263647446@@127.0.0.1@3306@salary_manager_db
-- Sample Data for Salary Manager Database
-- This file contains sample users and transactions for testing

USE salary_manager_db;

-- Insert sample users (passwords are plain text for demo - in production, use hashed passwords)
INSERT IGNORE INTO users (username, password, first_name, last_name, email) VALUES
('john_doe', 'password123', 'John', 'Doe', 'john.doe@email.com'),
('jane_smith', 'password456', 'Jane', 'Smith', 'jane.smith@email.com'),
('bob_wilson', 'password789', 'Bob', 'Wilson', 'bob.wilson@email.com');

-- Get user IDs for sample data
SET @john_id = (SELECT user_id FROM users WHERE username = 'john_doe');
SET @jane_id = (SELECT user_id FROM users WHERE username = 'jane_smith');
SET @bob_id = (SELECT user_id FROM users WHERE username = 'bob_wilson');

-- Insert sample income transactions for John Doe
INSERT INTO transactions (user_id, transaction_type, category, amount, description, transaction_date) VALUES
(@john_id, 'INCOME', 'Salary', 5000.00, 'Monthly salary from ABC Corp', '2025-01-01'),
(@john_id, 'INCOME', 'Freelance', 1500.00, 'Web development project', '2025-01-05'),
(@john_id, 'INCOME', 'Investment', 200.00, 'Stock dividends', '2025-01-10'),
(@john_id, 'INCOME', 'Rental', 800.00, 'Rental income from property', '2025-01-01'),
(@john_id, 'INCOME', 'Bonus', 1000.00, 'Performance bonus', '2025-01-15');

-- Insert sample expense transactions for John Doe
INSERT INTO transactions (user_id, transaction_type, category, amount, description, transaction_date) VALUES
(@john_id, 'EXPENSE', 'Food & Dining', 300.00, 'Groceries and restaurants', '2025-01-02'),
(@john_id, 'EXPENSE', 'Transportation', 150.00, 'Gas and car maintenance', '2025-01-03'),
(@john_id, 'EXPENSE', 'Bills & Utilities', 200.00, 'Electricity and water', '2025-01-04'),
(@john_id, 'EXPENSE', 'Entertainment', 100.00, 'Movie tickets and games', '2025-01-06'),
(@john_id, 'EXPENSE', 'Healthcare', 80.00, 'Doctor visit copay', '2025-01-08'),
(@john_id, 'EXPENSE', 'Shopping', 250.00, 'Clothes and household items', '2025-01-12'),
(@john_id, 'EXPENSE', 'Education', 500.00, 'Online course subscription', '2025-01-14');

-- Insert sample income transactions for Jane Smith
INSERT INTO transactions (user_id, transaction_type, category, amount, description, transaction_date) VALUES
(@jane_id, 'INCOME', 'Salary', 4500.00, 'Monthly salary from XYZ Inc', '2025-01-01'),
(@jane_id, 'INCOME', 'Business', 2000.00, 'Consulting income', '2025-01-07'),
(@jane_id, 'INCOME', 'Investment', 350.00, 'Mutual fund returns', '2025-01-12');

-- Insert sample expense transactions for Jane Smith
INSERT INTO transactions (user_id, transaction_type, category, amount, description, transaction_date) VALUES
(@jane_id, 'EXPENSE', 'Food & Dining', 280.00, 'Weekly groceries', '2025-01-03'),
(@jane_id, 'EXPENSE', 'Transportation', 120.00, 'Public transport pass', '2025-01-05'),
(@jane_id, 'EXPENSE', 'Bills & Utilities', 180.00, 'Internet and phone bills', '2025-01-08'),
(@jane_id, 'EXPENSE', 'Travel', 600.00, 'Business trip expenses', '2025-01-10'),
(@jane_id, 'EXPENSE', 'Personal Care', 90.00, 'Salon and spa', '2025-01-13');

-- Insert sample income transactions for Bob Wilson
INSERT INTO transactions (user_id, transaction_type, category, amount, description, transaction_date) VALUES
(@bob_id, 'INCOME', 'Salary', 3800.00, 'Monthly salary', '2025-01-01'),
(@bob_id, 'INCOME', 'Gift', 500.00, 'Birthday gift money', '2025-01-09'),
(@bob_id, 'INCOME', 'Other Income', 200.00, 'Sold old electronics', '2025-01-16');

-- Insert sample expense transactions for Bob Wilson
INSERT INTO transactions (user_id, transaction_type, category, amount, description, transaction_date) VALUES
(@bob_id, 'EXPENSE', 'Food & Dining', 220.00, 'Food expenses', '2025-01-04'),
(@bob_id, 'EXPENSE', 'Home & Garden', 150.00, 'Home improvement supplies', '2025-01-07'),
(@bob_id, 'EXPENSE', 'Insurance', 300.00, 'Car insurance premium', '2025-01-11'),
(@bob_id, 'EXPENSE', 'Gifts & Charity', 100.00, 'Charity donation', '2025-01-15');

-- Display sample data summary
SELECT 
    'Sample Data Summary' as info,
    COUNT(*) as total_transactions,
    COUNT(DISTINCT user_id) as total_users,
    SUM(CASE WHEN transaction_type = 'INCOME' THEN amount ELSE 0 END) as total_income,
    SUM(CASE WHEN transaction_type = 'EXPENSE' THEN amount ELSE 0 END) as total_expenses
FROM transactions;

-- Display user balances
SELECT 
    u.username,
    u.first_name,
    u.last_name,
    COALESCE(SUM(CASE WHEN t.transaction_type = 'INCOME' THEN t.amount ELSE 0 END), 0) as total_income,
    COALESCE(SUM(CASE WHEN t.transaction_type = 'EXPENSE' THEN t.amount ELSE 0 END), 0) as total_expenses,
    COALESCE(SUM(CASE WHEN t.transaction_type = 'INCOME' THEN t.amount ELSE -t.amount END), 0) as balance
FROM users u
LEFT JOIN transactions t ON u.user_id = t.user_id
GROUP BY u.user_id, u.username, u.first_name, u.last_name
ORDER BY u.username;
