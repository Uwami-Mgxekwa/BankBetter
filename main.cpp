#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>
#include <ctime>
#include <sstream>
#include <algorithm>

using namespace std;

class Account {
public:
    int accountNumber;
    string name;
    string password;
    string accountType;
    double balance;
    bool locked;
    int failedAttempts;

    Account() {
        accountNumber = 0;
        balance = 0;
        locked = false;
        failedAttempts = 0;
    }
};

class BankSystem {
private:
    vector<Account> accounts;

public:
    void loadAccounts() {
        accounts.clear();

        ifstream file("bank_database.txt");
        if (!file.is_open()) return;

        string line;

        while (getline(file, line)) {
            stringstream ss(line);
            Account acc;
            string temp;

            getline(ss, temp, '|');
            acc.accountNumber = stoi(temp);

            getline(ss, acc.name, '|');
            getline(ss, acc.password, '|');
            getline(ss, acc.accountType, '|');

            getline(ss, temp, '|');
            acc.balance = stod(temp);

            getline(ss, temp, '|');
            acc.locked = stoi(temp);

            getline(ss, temp, '|');
            acc.failedAttempts = stoi(temp);

            accounts.push_back(acc);
        }

        file.close();
    }

    void saveAccounts() {
        ofstream file("bank_database.txt");

        for (auto &acc : accounts) {
            file << acc.accountNumber << "|"
                 << acc.name << "|"
                 << acc.password << "|"
                 << acc.accountType << "|"
                 << acc.balance << "|"
                 << acc.locked << "|"
                 << acc.failedAttempts << endl;
        }

        file.close();
    }

    string currentDateTime() {
        time_t now = time(0);
        char* dt = ctime(&now);
        string result(dt);
        result.pop_back();
        return result;
    }

    void logTransaction(string message) {
        ofstream log("transaction_history.txt", ios::app);
        log << "[" << currentDateTime() << "] " << message << endl;
        log.close();
    }

    bool accountExists(int accNo) {
        for (auto &acc : accounts) {
            if (acc.accountNumber == accNo)
                return true;
        }
        return false;
    }

    Account* findAccount(int accNo) {
        for (auto &acc : accounts) {
            if (acc.accountNumber == accNo)
                return &acc;
        }
        return nullptr;
    }

    void createAccount() {
        Account acc;

        cout << "\n=====================================\n";
        cout << "        CREATE NEW ACCOUNT\n";
        cout << "=====================================\n";

        cout << "Enter Account Number: ";
        cin >> acc.accountNumber;

        if (accountExists(acc.accountNumber)) {
            cout << "Account already exists!\n";
            return;
        }

        cin.ignore();

        cout << "Enter Full Name: ";
        getline(cin, acc.name);

        cout << "Create Password/PIN: ";
        getline(cin, acc.password);

        cout << "Select Account Type\n";
        cout << "1. Savings\n";
        cout << "2. Current\n";
        cout << "3. Student\n";
        cout << "Choice: ";

        int typeChoice;
        cin >> typeChoice;

        switch (typeChoice) {
            case 1:
                acc.accountType = "Savings";
                break;
            case 2:
                acc.accountType = "Current";
                break;
            case 3:
                acc.accountType = "Student";
                break;
            default:
                acc.accountType = "Savings";
        }

        cout << "Enter Initial Deposit: R";
        cin >> acc.balance;

        accounts.push_back(acc);
        saveAccounts();

        logTransaction("Account Created -> Account No: " + to_string(acc.accountNumber));

        cout << "\nAccount created successfully!\n";
    }

    bool login(int accNo, string password) {
        Account* acc = findAccount(accNo);

        if (acc == nullptr) {
            cout << "Account not found!\n";
            return false;
        }

        if (acc->locked) {
            cout << "Account is locked due to multiple failed attempts.\n";
            return false;
        }

        if (acc->password == password) {
            acc->failedAttempts = 0;
            saveAccounts();
            return true;
        }
        else {
            acc->failedAttempts++;

            if (acc->failedAttempts >= 3) {
                acc->locked = true;
                cout << "Account locked! Too many failed attempts.\n";
            }
            else {
                cout << "Incorrect password!\n";
            }

            saveAccounts();
            return false;
        }
    }

    void displayAccounts() {
        cout << "\n===============================================================\n";
        cout << left
             << setw(15) << "Account No"
             << setw(25) << "Name"
             << setw(15) << "Type"
             << setw(15) << "Balance"
             << endl;
        cout << "===============================================================\n";

        for (auto &acc : accounts) {
            cout << left
                 << setw(15) << acc.accountNumber
                 << setw(25) << acc.name
                 << setw(15) << acc.accountType
                 << "R" << fixed << setprecision(2) << acc.balance
                 << endl;
        }
    }

    void depositMoney() {
        int accNo;
        double amount;

        cout << "Enter Account Number: ";
        cin >> accNo;

        Account* acc = findAccount(accNo);

        if (acc == nullptr) {
            cout << "Account not found!\n";
            return;
        }

        cout << "Enter Amount to Deposit: R";
        cin >> amount;

        if (amount <= 0) {
            cout << "Invalid amount!\n";
            return;
        }

        acc->balance += amount;

        saveAccounts();

        logTransaction("Deposit -> Account No: " + to_string(accNo) +
                       " Amount: R" + to_string(amount));

        cout << "Deposit successful!\n";
    }

    void withdrawMoney() {
        int accNo;
        double amount;
        string password;

        cout << "Enter Account Number: ";
        cin >> accNo;

        cin.ignore();

        cout << "Enter Password/PIN: ";
        getline(cin, password);

        if (!login(accNo, password)) return;

        Account* acc = findAccount(accNo);

        cout << "Enter Amount to Withdraw: R";
        cin >> amount;

        if (amount <= 0) {
            cout << "Invalid amount!\n";
            return;
        }

        if (acc->balance < amount) {
            cout << "Insufficient balance!\n";
            return;
        }

        acc->balance -= amount;

        saveAccounts();

        logTransaction("Withdrawal -> Account No: " + to_string(accNo) +
                       " Amount: R" + to_string(amount));

        cout << "Withdrawal successful!\n";
    }

    void transferMoney() {
        int senderNo, receiverNo;
        double amount;
        string password;

        cout << "\n=========== MONEY TRANSFER ===========\n";

        cout << "Enter Sender Account Number: ";
        cin >> senderNo;

        cin.ignore();

        cout << "Enter Password/PIN: ";
        getline(cin, password);

        if (!login(senderNo, password)) return;

        cout << "Enter Receiver Account Number: ";
        cin >> receiverNo;

        cout << "Enter Amount to Transfer: R";
        cin >> amount;

        Account* sender = findAccount(senderNo);
        Account* receiver = findAccount(receiverNo);

        if (receiver == nullptr) {
            cout << "Receiver account not found!\n";
            return;
        }

        if (amount <= 0) {
            cout << "Invalid amount!\n";
            return;
        }

        if (sender->balance < amount) {
            cout << "Insufficient balance!\n";
            return;
        }

        sender->balance -= amount;
        receiver->balance += amount;

        saveAccounts();

        logTransaction("Transfer -> From: " + to_string(senderNo) +
                       " To: " + to_string(receiverNo) +
                       " Amount: R" + to_string(amount));

        cout << "Transfer successful!\n";
    }

    void searchAccount() {
        int accNo;

        cout << "Enter Account Number to Search: ";
        cin >> accNo;

        Account* acc = findAccount(accNo);

        if (acc == nullptr) {
            cout << "Account not found!\n";
            return;
        }

        cout << "\n=========== ACCOUNT DETAILS ===========\n";
        cout << "Account Number : " << acc->accountNumber << endl;
        cout << "Name           : " << acc->name << endl;
        cout << "Account Type   : " << acc->accountType << endl;
        cout << "Balance        : R" << fixed << setprecision(2) << acc->balance << endl;
        cout << "Status         : " << (acc->locked ? "Locked" : "Active") << endl;
    }

    void deleteAccount() {
        int accNo;

        cout << "Enter Account Number to Delete: ";
        cin >> accNo;

        auto it = remove_if(accounts.begin(), accounts.end(),
            [accNo](Account &acc) {
                return acc.accountNumber == accNo;
            });

        if (it != accounts.end()) {
            accounts.erase(it, accounts.end());
            saveAccounts();

            logTransaction("Account Deleted -> Account No: " + to_string(accNo));

            cout << "Account deleted successfully!\n";
        }
        else {
            cout << "Account not found!\n";
        }
    }

    void applyInterest() {
        cout << "Applying interest to savings accounts...\n";

        for (auto &acc : accounts) {
            if (acc.accountType == "Savings") {
                double interest = acc.balance * 0.05;
                acc.balance += interest;

                logTransaction("Interest Applied -> Account No: " +
                               to_string(acc.accountNumber) +
                               " Interest: R" + to_string(interest));
            }
        }

        saveAccounts();

        cout << "Interest applied successfully!\n";
    }

    void viewTransactionHistory() {
        ifstream file("transaction_history.txt");

        if (!file.is_open()) {
            cout << "No transaction history found!\n";
            return;
        }

        string line;

        cout << "\n=============== TRANSACTION HISTORY ===============\n";

        while (getline(file, line)) {
            cout << line << endl;
        }

        file.close();
    }

    void bankStatistics() {
        double totalBalance = 0;
        double highestBalance = 0;
        string richestCustomer;

        for (auto &acc : accounts) {
            totalBalance += acc.balance;

            if (acc.balance > highestBalance) {
                highestBalance = acc.balance;
                richestCustomer = acc.name;
            }
        }

        cout << "\n=============== BANK STATISTICS ===============\n";
        cout << "Total Accounts      : " << accounts.size() << endl;
        cout << "Total Bank Balance  : R" << fixed << setprecision(2)
             << totalBalance << endl;
        cout << "Highest Balance     : R" << highestBalance << endl;
        cout << "Top Customer        : " << richestCustomer << endl;
    }

    void exportReport() {
        ofstream report("bank_report.csv");

        report << "AccountNumber,Name,AccountType,Balance\n";

        for (auto &acc : accounts) {
            report << acc.accountNumber << ","
                   << acc.name << ","
                   << acc.accountType << ","
                   << acc.balance << endl;
        }

        report.close();

        cout << "Report exported successfully to bank_report.csv\n";
    }
};

void showMenu() {
    cout << "\n====================================================\n";
    cout << "         ADVANCED BANK MANAGEMENT SYSTEM\n";
    cout << "====================================================\n";
    cout << "1. Create Account\n";
    cout << "2. Display Accounts\n";
    cout << "3. Deposit Money\n";
    cout << "4. Withdraw Money\n";
    cout << "5. Transfer Money\n";
    cout << "6. Search Account\n";
    cout << "7. Delete Account\n";
    cout << "8. Apply Interest\n";
    cout << "9. View Transaction History\n";
    cout << "10. Bank Statistics\n";
    cout << "11. Export CSV Report\n";
    cout << "12. Exit\n";
    cout << "====================================================\n";
    cout << "Enter your choice: ";
}

int main() {
    BankSystem bank;
    bank.loadAccounts();

    int choice;

    do {
        showMenu();
        cin >> choice;

        switch (choice) {
            case 1:
                bank.createAccount();
                break;

            case 2:
                bank.displayAccounts();
                break;

            case 3:
                bank.depositMoney();
                break;

            case 4:
                bank.withdrawMoney();
                break;

            case 5:
                bank.transferMoney();
                break;

            case 6:
                bank.searchAccount();
                break;

            case 7:
                bank.deleteAccount();
                break;

            case 8:
                bank.applyInterest();
                break;

            case 9:
                bank.viewTransactionHistory();
                break;

            case 10:
                bank.bankStatistics();
                break;

            case 11:
                bank.exportReport();
                break;

            case 12:
                cout << "Exiting system...\n";
                break;

            default:
                cout << "Invalid option!\n";
        }

    } while (choice != 12);

    return 0;
}
