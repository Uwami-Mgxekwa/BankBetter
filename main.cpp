#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
using namespace std;

struct Account {
    int accountNumber;
    string name;
    double balance;
};

void logTransaction(int accNo, string type, double amount, double newBalance) {
    ofstream log("transaction_history.txt", ios::app);
    if (log.is_open()) {
        log << "Account: " << accNo
            << " | " << type
            << " | Amount: " << amount
            << " | New Balance: " << newBalance
            << endl;
        log.close();
    }
}

void createAccount() {
    Account acc;
    cout << "Enter Account Number: ";
    cin >> acc.accountNumber;
    cout << "Enter Account Holder Name: ";
    cin.ignore();
    getline(cin, acc.name);
    cout << "Enter Initial Balance: ";
    cin >> acc.balance;

    ofstream file("bank_database.txt", ios::app);
    if (file.is_open()) {
        file << acc.accountNumber << " | " << acc.name << " | " << acc.balance << endl;
        file.close();
        cout << "Account created successfully!\n";
        logTransaction(acc.accountNumber, "Account Created", acc.balance, acc.balance);
    } else {
        cout << "Error opening file.\n";
    }
}

void displayAccounts() {
    ifstream file("bank_database.txt");
    string line;
    cout << "\n--- Account Records ---\n";
    while (getline(file, line)) {
        cout << line << endl;
    }
    file.close();
}

void depositMoney(int accNo, double amount) {
    ifstream file("bank_database.txt");
    ofstream temp("temp.txt");
    Account acc;
    bool found = false;

    while (file >> acc.accountNumber) {
        file.ignore(3, '|');
        getline(file, acc.name, '|');
        file >> acc.balance;

        if (acc.accountNumber == accNo) {
            acc.balance += amount;
            found = true;
            cout << "Deposited " << amount << " successfully!\n";
            logTransaction(acc.accountNumber, "Deposit", amount, acc.balance);
        }
        temp << acc.accountNumber << " | " << acc.name << " | " << acc.balance << endl;
    }

    file.close();
    temp.close();
    remove("bank_database.txt");
    rename("temp.txt", "bank_database.txt");

    if (!found) cout << "Account not found!\n";
}

void withdrawMoney(int accNo, double amount) {
    ifstream file("bank_database.txt");
    ofstream temp("temp.txt");
    Account acc;
    bool found = false;

    while (file >> acc.accountNumber) {
        file.ignore(3, '|');
        getline(file, acc.name, '|');
        file >> acc.balance;

        if (acc.accountNumber == accNo) {
            if (acc.balance >= amount) {
                acc.balance -= amount;
                cout << "Withdrawn " << amount << " successfully!\n";
                logTransaction(acc.accountNumber, "Withdrawal", amount, acc.balance);
            } else {
                cout << "Insufficient balance!\n";
            }
            found = true;
        }
        temp << acc.accountNumber << " | " << acc.name << " | " << acc.balance << endl;
    }

    file.close();
    temp.close();
    remove("bank_database.txt");
    rename("temp.txt", "bank_database.txt");

    if (!found) cout << "Account not found!\n";
}

void viewHistory() {
    ifstream log("transaction_history.txt");
    string line;
    cout << "\n--- Transaction History ---\n";
    while (getline(log, line)) {
        cout << line << endl;
    }
    log.close();
}

int main() {
    int choice;
    do {
        cout << "\n--- Bank Management System ---\n";
        cout << "1. Create Account\n";
        cout << "2. Display Accounts\n";
        cout << "3. Deposit Money\n";
        cout << "4. Withdraw Money\n";
        cout << "5. View Transaction History\n";
        cout << "6. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
            case 1: createAccount(); break;
            case 2: displayAccounts(); break;
            case 3: {
                int accNo; double amount;
                cout << "Enter Account Number: "; cin >> accNo;
                cout << "Enter Amount to Deposit: "; cin >> amount;
                depositMoney(accNo, amount);
                break;
            }
            case 4: {
                int accNo; double amount;
                cout << "Enter Account Number: "; cin >> accNo;
                cout << "Enter Amount to Withdraw: "; cin >> amount;
                withdrawMoney(accNo, amount);
                break;
            }
            case 5: viewHistory(); break;
            case 6: cout << "Exiting...\n"; break;
            default: cout << "Invalid choice!\n";
        }
    } while (choice != 6);

    return 0;
}
