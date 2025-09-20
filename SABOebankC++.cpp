#include <iostream>
#include <fstream>
#include <sstream>
#include <random> 
#include <chrono>
#include <iomanip>
#include <ctime>
#include <limits>
#include <algorithm>
#include <vector>
using namespace std;

const string Accounts_FILE = "Accounts.txt";
const string ADMIN_FEEDBACK_FILE = "Admin_Feedbacks.txt";
const int MAX_Accounts = 500;

const float USD_EGP = 48.22;   // دولار أمريكي
const float EUR_EGP = 56.57;   // يورو
const float GBP_EGP = 65.37;   // جنيه إسترليني
const float AED_EGP = 13.13;   // درهم إماراتي
const float SAR_EGP = 12.85;   // ريال سعودي

struct infobalance {
    long double withdraw;
    long double deposit;
    long double transfer;
    int withdrawcount;
    int depositcount;
    int transfercount;
};

struct loan {
    bool hasloan = false;
    double loanamount = 0;
    double totaltopay = 0;
    int installments = 0;
    int paidinstallments = 0;
    double installmentvalue = 0;
    long long lastpaymentdate;
    double paidamount;
};


struct Accounts {
    string Name;
    string type_of_account;
    string ID;
    string PIN;
    string Phone;
    string Email;
    string Age;
    string Job;
    float Salary;
    string Gender;
    string Address;
    string CreatedAt;
    string TransactionsFile;
    string MessagesFile;
    bool isFrozen = false;
    float Balance = 0.0f;
    infobalance money;
    time_t lastInterestTime = time(0);
    loan haram;
    vector<string> notifications;
    float lastInterestAdded = 0.0f;
};


void SaveAccountsFromFile();
void LoadAccountsFromFile();
string generateRandomID();
void Transfer(int accountIndex);
void AccountMenu(int accountIndex);
void Deposit(int accountIndex);
void Withdraw(int accountIndex);
void CreateANewAccount();
int Login();
void Welcome();
void DeleteAccount();
void AdminMenu();
void PrintDepositReceipt(int accountIndex, double amount);
void PrintWithdrawReceipt(int accountIndex, double amount);
void PrintTransferReceipt(int accountIndex, int recipientIndex, double amount);
void LogTransaction(int accountIndex, const string& receiptContent);
void ViewTransactions(int accountIndex);
void AdminViewAccountTransactions();
void ViewAllAccounts();
void SearchAndManageAccount();
void EditAccountByAdmin(int accountIndex);
void UnFreezeAccount();
void FreezeAccount();
void ViewBalance(int accountIndex);
void EditInformation(int accountIndex);
void EditName(int accountIndex);
void EditPhoneNumber(int accountIndex);
void EditPIN(int accountIndex);
void ViewAccountsByBalance();
void ViewAccountDetails(int accountIndex);
void AccountMenu_Pension(int accountIndex);
void AccountMenu_VIP(int accountIndex);
void AccountMenu_Savings(int accountIndex);
void AccountMenu_Student_Current(int accountIndex);
void ForeignCurrencies(int accountIndex);
float getRate(string currency);
void ShowInbox(int accountIndex);
void AddInboxEntry(int accountIndex, const string& entry);
string GetTimestamp();
string trim(const string& s);
void Notify(int accountIndex, const string& message);
void LoadNotifications(int accountIndex);
void ShowNotifications(int accountIndex);
void SendFeedback(int accountIndex);
string escapeForFile(const string& s);
string unescapeFromFile(const string& s);
void AdminViewFeedbacksAndReply();
void PredictBalance(int accountIndex);
void ApplyWeeklyInterest(int accountIndex);
void ApplyWeeklyInterestForAll();
void ShowLastInterest(int accountIndex);
void SavingsGoals(int accountIndex);
void LoanMenu(int accountIndex);
void NewLoan(int accountIndex);
void CurrentLoanDetails(int accountIndex);
void PayAnInstallment(int accountIndex);
bool checkFileExists(const string& filename);
void createTransactionFileIfNotExists(int accountIndex);
void createMessageFileIfNotExists(int accountIndex);

Accounts users[MAX_Accounts];
int AccountCount = 0;

//-----------------------------------------
void SaveAccountsFromFile() {
    ofstream file(Accounts_FILE);
    if (!file.is_open()) {
        cout << "Error opening file to save accounts.\n";
        return;
    }

    for (int i = 0; i < AccountCount; i++) {
        file << users[i].Name << "-"
            << users[i].type_of_account << "-"
            << users[i].ID << "-"
            << users[i].PIN << "-"
            << users[i].Phone << "-"
            << users[i].Email << "-"
            << users[i].Age << "-"
            << users[i].Job << "-"
            << users[i].Salary << "-"
            << users[i].Gender << "-"
            << users[i].Address << "-"
            << users[i].CreatedAt << "-"
            << users[i].TransactionsFile << "-"
            << users[i].MessagesFile << "-"
            << (users[i].isFrozen ? "1" : "0") << "-"
            << users[i].Balance << "-"
            << users[i].money.withdraw << "-"
            << users[i].money.deposit << "-"
            << users[i].money.transfer << "-"
            << users[i].money.withdrawcount << "-"
            << users[i].money.depositcount << "-"
            << users[i].money.transfercount << "-"
            << users[i].lastInterestTime << "-"
            << users[i].lastInterestAdded << "-"
            << (users[i].haram.hasloan ? "1" : "0") << "-"
            << users[i].haram.loanamount << "-"
            << users[i].haram.totaltopay << "-"
            << users[i].haram.installments << "-"
            << users[i].haram.paidinstallments << "-"
            << users[i].haram.installmentvalue << "-"
            << users[i].haram.lastpaymentdate << "-"
            << users[i].haram.paidamount
            << endl;
    }

    file.close();
}
//-----------------------------------------
void LoadAccountsFromFile() {
    ifstream file(Accounts_FILE);
    if (!file.is_open()) {
        cout << "No existing accounts file. A new one will be created.\n";
        return;
    }

    AccountCount = 0;
    string line;

    while (getline(file, line)) {
        if (line.empty()) continue;

        stringstream ss(line);
        string token;

        getline(ss, users[AccountCount].Name, '-');
        getline(ss, users[AccountCount].type_of_account, '-');
        getline(ss, users[AccountCount].ID, '-');
        getline(ss, users[AccountCount].PIN, '-');
        getline(ss, users[AccountCount].Phone, '-');
        getline(ss, users[AccountCount].Email, '-');
        getline(ss, users[AccountCount].Age, '-');
        getline(ss, users[AccountCount].Job, '-');

        getline(ss, token, '-');
        try { users[AccountCount].Salary = stof(token); }
        catch (...) { users[AccountCount].Salary = 0.0f; }

        getline(ss, users[AccountCount].Gender, '-');
        getline(ss, users[AccountCount].Address, '-');
        getline(ss, users[AccountCount].CreatedAt, '-');
        getline(ss, users[AccountCount].TransactionsFile, '-');
        getline(ss, users[AccountCount].MessagesFile, '-');

        string frozenStr;
        getline(ss, frozenStr, '-');
        users[AccountCount].isFrozen = (frozenStr == "1" || frozenStr == "true");

        getline(ss, token, '-');
        try { users[AccountCount].Balance = stof(token); }
        catch (...) { users[AccountCount].Balance = 0.0f; }

        try {
            getline(ss, token, '-'); users[AccountCount].money.withdraw = stold(token);
            getline(ss, token, '-'); users[AccountCount].money.deposit = stold(token);
            getline(ss, token, '-'); users[AccountCount].money.transfer = stold(token);
            getline(ss, token, '-'); users[AccountCount].money.withdrawcount = stoi(token);
            getline(ss, token, '-'); users[AccountCount].money.depositcount = stoi(token);
            getline(ss, token, '-'); users[AccountCount].money.transfercount = stoi(token);
        }
        catch (...) {
            users[AccountCount].money = { 0,0,0,0,0,0 };
        }

        try {
            getline(ss, token, '-'); users[AccountCount].lastInterestTime = stoll(token);
            getline(ss, token, '-'); users[AccountCount].lastInterestAdded = stof(token);   // ⭐ تحميل آخر فايدة

            getline(ss, token, '-'); users[AccountCount].haram.hasloan = (token == "1");
            getline(ss, token, '-'); users[AccountCount].haram.loanamount = stod(token);
            getline(ss, token, '-'); users[AccountCount].haram.totaltopay = stod(token);
            getline(ss, token, '-'); users[AccountCount].haram.installments = stoi(token);
            getline(ss, token, '-'); users[AccountCount].haram.paidinstallments = stoi(token);
            getline(ss, token, '-'); users[AccountCount].haram.installmentvalue = stod(token);
            getline(ss, token, '-'); users[AccountCount].haram.lastpaymentdate = stoll(token);
            getline(ss, token, '-'); users[AccountCount].haram.paidamount = stod(token);
        }
        catch (...) {
            users[AccountCount].lastInterestTime = time(0);
            users[AccountCount].lastInterestAdded = 0.0f;
            users[AccountCount].haram = loan();
        }

        AccountCount++;
        if (AccountCount >= MAX_Accounts) break;
    }

    file.close();
}
//-----------------------------------------
string generateRandomID() {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> distrib(0, 9);

    stringstream ss;
    for (int i = 0; i < 8; ++i) {
        ss << distrib(gen);
    }

    return ss.str();
}
//-----------------------------------------
void Deposit(int accountIndex)
{
    cout << "Please Enter An Amount In Multiples Of 50 EGP."
        << endl << "(The Maximum Amount Is \"30,000\" EGP & The Minimum Is \"50 EGP\")  ";
    int x;

    while (true)
    {
        cin >> x;
        if (cin.fail()) {
            cout << "Invalid input. Please enter a number. Try Again! ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }
        if (x > 30000) { cout << "The Amount Cannot Exceed 30,000 EGP. Try Again! "; continue; }
        if (x == 0) { cout << " The Amount Cannot Be Zero. Try Again! "; continue; }
        if (x < 50) { cout << "The Amount Must Be At Least 50 EGP. Try Again! "; continue; }
        if (x % 50 != 0) { cout << "The Amount Must Be In Multiples Of 50 EGP. Try Again!  "; continue; }
        break;
    }

    users[accountIndex].Balance += x;
    SaveAccountsFromFile();

    PrintDepositReceipt(accountIndex, x);

    stringstream receiptStream;
    auto now = chrono::system_clock::now();
    time_t time_now = chrono::system_clock::to_time_t(now);
    tm localTime;
    localtime_s(&localTime, &time_now);

    receiptStream << "========== Deposit Receipt ==========\n"
        << " Amount Deposited : " << fixed << setprecision(2) << (double)x << " EGP\n"
        << "-------------------------------------\n"
        << " Account Name     : " << users[accountIndex].Name << "\n"
        << " Account ID       : " << users[accountIndex].ID << "\n"
        << " New Balance      : " << fixed << setprecision(2) << users[accountIndex].Balance << " EGP\n"
        << "-------------------------------------\n"
        << " Date             : " << put_time(&localTime, "%Y-%m-%d") << "\n"
        << " Time             : " << put_time(&localTime, "%H:%M:%S") << "\n"
        << "=====================================\n";
    users[accountIndex].money.withdrawcount++;
    users[accountIndex].money.withdraw = +x;

    LogTransaction(accountIndex, receiptStream.str());
    Notify(accountIndex, "Deposit of " + to_string(x) + " EGP successful. New Balance: " + to_string(users[accountIndex].Balance));

}
//-----------------------------------------
void Withdraw(int accountIndex)
{
    cout << "Please Enter An Amount In Multiples Of 50 EGP."
        << endl << "(The Maximum Amount Is \"30,000\" EGP & The Minimum Is 50 EGP) ";
    int x;

    while (true)
    {
        cin >> x;
        if (cin.fail()) {
            cout << "Invalid input. Please enter a number. Try Again! ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }
        if (x == 0) { cout << " The Amount Cannot Be Zero. Try Again!  "; continue; }
        if (users[accountIndex].Balance < x) { cout << "Insufficient Funds.!  "; continue; }
        if (x > 30000) { cout << "The Amount Cannot Exceed 30,000 EGP. Try Again!  "; continue; }
        if (x < 50) { cout << "The Amount Must Be At Least 50 EGP. Try Again!  "; continue; }
        if (x % 50 != 0) { cout << "The Amount Must Be In Multiples Of 50 EGP. Try Again!  "; continue; }
        break;
    }

    users[accountIndex].Balance -= x;
    SaveAccountsFromFile();

    PrintWithdrawReceipt(accountIndex, x);

    stringstream receiptStream;
    auto now = chrono::system_clock::now();
    time_t time_now = chrono::system_clock::to_time_t(now);
    tm localTime;
    localtime_s(&localTime, &time_now);

    receiptStream << "========= Withdrawal Receipt =========\n"
        << " Amount Withdrawn : " << fixed << setprecision(2) << (double)x << " EGP\n"
        << "-------------------------------------\n"
        << " Account Name     : " << users[accountIndex].Name << "\n"
        << " Account ID       : " << users[accountIndex].ID << "\n"
        << " New Balance      : " << fixed << setprecision(2) << users[accountIndex].Balance << " EGP\n"
        << "-------------------------------------\n"
        << " Date             : " << put_time(&localTime, "%Y-%m-%d") << "\n"
        << " Time             : " << put_time(&localTime, "%H:%M:%S") << "\n"
        << "=====================================\n";
    users[accountIndex].money.depositcount++;
    users[accountIndex].money.deposit += x;
    LogTransaction(accountIndex, receiptStream.str());
    Notify(accountIndex, "Withdrawal of " + to_string(x) + " EGP successful. New Balance: " + to_string(users[accountIndex].Balance));

}
//-----------------------------------------
void AdminMenu()
{
    cout << "------------------------------" << endl;
    cout << "    Welcome To Admin Menu" << endl;
    cout << "------------------------------" << endl;

    cout << "1. Delete An Account " << endl;
    cout << "2. Freeze An Account " << endl;
    cout << "3. UnFreeze An Account " << endl;
    cout << "4. View Account Transactions " << endl;
    cout << "5. Search for an account " << endl;
    cout << "6. View All Accounts " << endl;
    cout << "7. View Accounts By Balance " << endl;
    cout << "8. View Feedbacks & Reply" << endl;
    cout << "9. Logout " << endl;
    cout << "0. Exit " << endl;
    cout << "-----------------------------" << endl;
    cout << "Enter Choice: " << endl;
    int choice;
    cin >> choice;
    switch (choice) {
    case 1:
        DeleteAccount();
        break;
    case 2:
        FreezeAccount();
        break;
    case 3:
        UnFreezeAccount();
        break;
    case 4:
        AdminViewAccountTransactions();
        break;
    case 5:
        SearchAndManageAccount();
        break;
    case 6:
        ViewAllAccounts();
        break;
    case 7:
        ViewAccountsByBalance();
        break;
    case 8:
        AdminViewFeedbacksAndReply();
        break;
    case 9:
        return;
        break;
    case 0:
        exit(0);
        break;
    default:
        cout << "Invalid input" << endl;
        break;
    }
}
//-----------------------------------------
void DeleteAccount()
{
    string input;
    int index = -1;
    int choice;

    cout << "\n===== Delete Account =====\n";
    cout << "1. Delete by ID\n";
    cout << "2. Delete by Phone\n";
    cout << "3. Delete by Email\n";
    cout << "Enter Choice: ";
    cin >> choice;
    cin.ignore();

    switch (choice) {
    case 1: {
        cout << "Enter Account ID: ";
        getline(cin, input);
        for (int i = 0; i < AccountCount; i++) {
            if (users[i].ID == input) {
                index = i;
                break;
            }
        }
        break;
    }
    case 2: {
        cout << "Enter Account Phone Number: ";
        getline(cin, input);
        for (int i = 0; i < AccountCount; i++) {
            if (users[i].Phone == input) {
                index = i;
                break;
            }
        }
        break;
    }
    case 3: {
        cout << "Enter Account Email: ";
        getline(cin, input);
        for (int i = 0; i < AccountCount; i++) {
            if (users[i].Email == input) {
                index = i;
                break;
            }
        }
        break;
    }
    default:
        cout << "Invalid choice.\n";
        return;
    }

    if (index == -1) {
        cout << "Account not found.\n";
        return;
    }

    for (int i = index; i < AccountCount - 1; i++) {
        users[i] = users[i + 1];
    }
    AccountCount--;

    SaveAccountsFromFile();

    cout << "Account deleted successfully.\n";
}
//-----------------------------------------
void Transfer(int accountIndex) {
    string id, phone, email;
    int choice, recipientIndex = -1;
    double amount;
    bool find = false;

    cout << "\n===== Transfer Menu =====\n";
    cout << "1. Transfer by Phone\n";
    cout << "2. Transfer by ID\n";
    cout << "3. Transfer by Email\n";
    cout << "4. Back to Account Menu\n";
    cout << "5. Exit Program\n";
    cout << "Enter Choice: ";
    cin >> choice;

    if (choice == 4) { return; }
    if (choice == 5) {
        cout << "Thank you for using SABO eBank. Goodbye!\n";
        SaveAccountsFromFile();
        exit(0);
    }

    if (choice == 1 || choice == 2 || choice == 3) {
        while (true) {
            find = false;
            if (choice == 1) {
                cout << "Enter the Phone Number: "; cin >> phone;
                for (int i = 0; i < AccountCount; i++) { if (phone == users[i].Phone) { find = true; recipientIndex = i; break; } }
                if (!find) { cout << "The Number not found. Please try again.\n"; continue; }
            }
            else if (choice == 2) {
                cout << "Enter the ID: "; cin >> id;
                for (int i = 0; i < AccountCount; i++) { if (id == users[i].ID) { find = true; recipientIndex = i; break; } }
                if (!find) { cout << "The ID not found. Please try again.\n"; continue; }
            }
            else if (choice == 3) {
                cout << "Enter the Email: "; cin >> email;
                for (int i = 0; i < AccountCount; i++) { if (email == users[i].Email) { find = true; recipientIndex = i; break; } }
                if (!find) { cout << "The Email not found. Please try again.\n"; continue; }
            }
            if (recipientIndex == accountIndex) { cout << "You cannot transfer money to yourself. Transaction cancelled.\n"; return; }
            if (users[recipientIndex].isFrozen) { cout << "The recipient account is frozen. Transaction cancelled.\n"; return; }
            break;
        }

        cout << "Enter the Amount to transfer: ";
        cin >> amount;
        if (cin.fail() || amount <= 0) {
            cout << "Invalid amount. Please enter a positive number.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return;
        }

        double fee = 0.0;
        if (amount < 1000) {
            fee = 0.5;
        }
        else {
            fee = amount / 1000.0;
        }
        double totalAmount = amount + fee;
        cout << "Transfer Fee: " << fixed << setprecision(2) << fee << " EGP\n";
        cout << "Total to be deducted: " << fixed << setprecision(2) << totalAmount << " EGP\n";

        if (totalAmount > users[accountIndex].Balance) {
            cout << "Your balance is not enough to cover the amount and the transfer fee.\n";
            return;
        }

        string pas;
        int maxTries = 3;
        bool pinOK = false;

        while (maxTries > 0) {
            cout << "Enter your PIN to confirm transaction: ";
            cin >> pas;

            if (pas == users[accountIndex].PIN) {
                pinOK = true;
                break;
            }
            else {
                maxTries--;
                if (maxTries > 0) {
                    cout << "Wrong PIN. You have " << maxTries << " tries left.\n";
                }
                else {
                    cout << "\nToo many wrong attempts. Your account has been FROZEN for security reasons!\n";
                    cout << "Please contact customer support to reactivate it.\n";
                    users[accountIndex].isFrozen = true;
                    SaveAccountsFromFile();
                    Notify(accountIndex, "Your account has been FROZEN due to multiple wrong PIN attempts.");

                }
            }
        }

        if (!pinOK) {
            return;
        }

        cout << "\nProcessing transaction..." << endl;
        users[accountIndex].Balance -= totalAmount;
        users[recipientIndex].Balance += amount;
        SaveAccountsFromFile();

        auto now = chrono::system_clock::now();
        time_t time_now = chrono::system_clock::to_time_t(now);
        tm localTime;
        localtime_s(&localTime, &time_now);

        cout << "\n========== Transfer Receipt ==========\n";
        cout << " Amount Transferred : " << fixed << setprecision(2) << amount << " EGP\n";
        cout << " Transfer Fee       : " << fixed << setprecision(2) << fee << " EGP\n";
        cout << " Total Deducted     : " << fixed << setprecision(2) << totalAmount << " EGP\n";
        cout << "--------------------------------------\n";
        cout << " To (Name)          : " << users[recipientIndex].Name << "\n";
        cout << " To (ID)            : " << users[recipientIndex].ID << "\n";
        cout << " To (Email)         : " << users[recipientIndex].Email << "\n";
        cout << "--------------------------------------\n";
        cout << " From (Name)        : " << users[accountIndex].Name << "\n";
        cout << " From (ID)          : " << users[accountIndex].ID << "\n";
        cout << " From (Email)       : " << users[accountIndex].Email << "\n";
        cout << " Your New Balance   : " << fixed << setprecision(2) << users[accountIndex].Balance << " EGP\n";
        cout << "--------------------------------------\n";
        cout << " Date               : " << put_time(&localTime, "%Y-%m-%d") << "\n";
        cout << " Time               : " << put_time(&localTime, "%H:%M:%S") << "\n";
        cout << "======================================\n";

        users[accountIndex].money.transfer += amount;
        users[accountIndex].money.transfercount++;

        users[recipientIndex].money.deposit += amount;
        users[recipientIndex].money.depositcount++;

        stringstream senderReceipt;
        senderReceipt << "========== Transfer Receipt (Sent) ==========\n"
            << " Amount Transferred : " << fixed << setprecision(2) << amount << " EGP\n"
            << " Transfer Fee       : " << fixed << setprecision(2) << fee << " EGP\n"
            << " To (Name)          : " << users[recipientIndex].Name << "\n"
            << " Your New Balance   : " << fixed << setprecision(2) << users[accountIndex].Balance << " EGP\n"
            << "-------------------------------------------\n"
            << " Date & Time        : " << put_time(&localTime, "%Y-%m-%d %H:%M:%S") << "\n"
            << "===========================================\n";
        LogTransaction(accountIndex, senderReceipt.str());
        Notify(accountIndex, senderReceipt.str());

        stringstream recipientReceipt;
        recipientReceipt << "========== Transfer Receipt (Received) ==========\n"
            << " Amount Received    : " << fixed << setprecision(2) << amount << " EGP\n"
            << " From (Name)        : " << users[accountIndex].Name << "\n"
            << " Your New Balance   : " << fixed << setprecision(2) << users[recipientIndex].Balance << " EGP\n"
            << "----------------------------------------------\n"
            << " Date & Time        : " << put_time(&localTime, "%Y-%m-%d %H:%M:%S") << "\n"
            << "==============================================\n";
        LogTransaction(recipientIndex, recipientReceipt.str());
        Notify(recipientIndex, recipientReceipt.str());

    }
    else {
        cout << "Invalid choice.\n";
    }
}
//-----------------------------------------
void AccountMenu(int accountIndex)
{
    cout << "--------------------------" << endl;
    cout << "     Account Menu " << endl;
    cout << "--------------------------" << endl;

    cout << "1. Deposit " << endl;
    cout << "2. Withdraw " << endl;
    cout << "3. Transfer " << endl;
    cout << "4. View Transactions " << endl;
    cout << "5. View Balance " << endl;
    cout << "6. Edit Information " << endl;
    cout << "7. View Information " << endl;
    cout << "8. Logout " << endl;
    cout << "9. Exit " << endl;
    cout << "--------------------------" << endl;
    cout << "Enter Choice: " << endl;
    int choice;
    cin >> choice;
    switch (choice) {
    case 1:
        Deposit(accountIndex);
        break;
    case 2:
        Withdraw(accountIndex);
        break;
    case 3:
        Transfer(accountIndex);
        break;
    case 4:
        ViewTransactions(accountIndex);
        break;
    case 5:
        ViewBalance(accountIndex);
        break;
    case 6:
        EditInformation(accountIndex);
        break;
    case 7:
        ViewAccountDetails(accountIndex);
        break;
    case 8:
        cout << "Logging out..." << endl;
        return;
    case 9:
        exit(0);
    default:
        cout << "Invalid input" << endl;
        break;
    }
}
//-----------------------------------------
void createMessageFileIfNotExists(int accountIndex) {
    if (accountIndex < 0 || accountIndex >= AccountCount) return;

    string filename = users[accountIndex].MessagesFile;
    if (!checkFileExists(filename)) {
        ofstream file(filename);
        if (file.is_open()) {
            file << "";
            file.close();
        }
    }
}
//-----------------------------------------
void createTransactionFileIfNotExists(int accountIndex) {
    if (accountIndex < 0 || accountIndex >= AccountCount) return;

    string filename = users[accountIndex].TransactionsFile;
    if (!checkFileExists(filename)) {
        ofstream file(filename);
        if (file.is_open()) {
            file << "";
            file.close();
        }
    }
}
//-----------------------------------------
void CreateANewAccount() {
    if (AccountCount >= MAX_Accounts) {
        cout << "Sorry, the bank cannot accept new accounts at this time.\n";
        return;
    }

    cout << "--------------------------" << endl;
    cout << "  Create A New Account " << endl;
    cout << "--------------------------" << endl;

    cout << "Enter Your Full Name: ";
    cin.ignore();
    getline(cin, users[AccountCount].Name);

    int age;
    cout << "Enter Your Age: ";
    cin >> age;
    users[AccountCount].Age = to_string(age);

    int accTypeChoice;
    while (true) {
        cout << "Select Account Type:\n";
        cout << "1. Student Account\n";
        cout << "2. Current Account\n";
        cout << "3. Savings Account\n";
        cout << "4. VIP Account\n";
        cout << "5. Pension Account (For Seniors)\n";
        cout << "Enter Choice (1-5): ";
        cin >> accTypeChoice;

        if (accTypeChoice >= 1 && accTypeChoice <= 5) {
            if (accTypeChoice == 1 && age > 21) {
                cout << "Student Account is only for people under 21 years old.\n";
                return;
            }
            if (accTypeChoice != 1 && age < 21) {
                cout << "You must select Student Account because you are under 21.\n";
                return;
            }
            break;
        }
        cout << "Invalid choice! Please select again.\n";
    }

    switch (accTypeChoice) {
    case 1: users[AccountCount].type_of_account = "Student Account"; break;
    case 2: users[AccountCount].type_of_account = "Current Account"; break;
    case 3: users[AccountCount].type_of_account = "Savings Account"; break;
    case 4: users[AccountCount].type_of_account = "VIP Account"; break;
    case 5: users[AccountCount].type_of_account = "Pension Account"; break;
    }

    string emailInput;
    while (true) {
        cout << "Enter Your Email (without @sabo.com, only lowercase letters/numbers): ";
        cin >> emailInput;

        for (char& c : emailInput) {
            c = tolower(c);
        }

        if (emailInput.find(' ') != string::npos ||
            emailInput.find(',') != string::npos ||
            emailInput.find('@') != string::npos) {
            cout << "Invalid email! Just enter the name part only.\n";
            continue;
        }

        string email = emailInput + "@sabo.com";

        bool exists = false;
        for (int i = 0; i < AccountCount; i++) {
            if (users[i].Email == email) {
                exists = true;
                break;
            }
        }

        if (exists) {
            cout << "This email already exists! Try another one.\n";
            continue;
        }

        users[AccountCount].Email = email;
        cout << "Your new Email is: " << users[AccountCount].Email << endl;
        break;
    }

    string x;
    while (true) {
        cout << "Enter Your PIN (at least 6 digits): ";
        cin >> x;

        if (x.length() < 6) {
            cout << "PIN is too short! Try again.\n";
            continue;
        }

        bool allSame = true;
        for (int i = 1; i < (int)x.length(); i++) {
            if (x[i] != x[0]) {
                allSame = false;
                break;
            }
        }

        if (allSame) {
            cout << "PIN Is Very Weak! Try Again.\n";
            continue;
        }
        break;
    }
    users[AccountCount].PIN = x;

    string newID = generateRandomID();
    users[AccountCount].ID = newID;
    cout << "Your new ID is: " << users[AccountCount].ID << "  Please remember it." << endl;
    users[AccountCount].TransactionsFile = "Transactions_" + newID + ".txt";
    users[AccountCount].MessagesFile = "Messages_" + newID + ".txt";

    ofstream transactionFile(users[AccountCount].TransactionsFile);
    if (transactionFile.is_open()) {
        transactionFile << "";
        transactionFile.close();
    }
    else {
        cout << "Warning: Could not create transaction file.\n";
    }

    ofstream messageFile(users[AccountCount].MessagesFile);
    if (messageFile.is_open()) {
        messageFile << "";
        messageFile.close();
    }
    else {
        cout << "Warning: Could not create message file.\n";
    }

    string y;
    while (true) {
        cout << "Enter Your Phone Number : ";
        cin >> y;

        if (y.length() != 11 || y[0] != '0' || y[1] != '1' ||
            (y[2] != '0' && y[2] != '1' && y[2] != '2' && y[2] != '5')) {
            cout << "This Number Is Wrong! Please Try Again.\n";
            continue;
        }
        break;
    }
    users[AccountCount].Phone = y;

    int jobChoice;
    while (true) {
        cout << "Select Your Job:\n";
        cout << "1. Doctor\n";
        cout << "2. Engineer\n";
        cout << "3. Teacher\n";
        cout << "4. Nurse\n";
        cout << "5. Lawyer\n";
        cout << "6. Accountant\n";
        cout << "7. Police Officer\n";
        cout << "8. Software Developer\n";
        cout << "9. Driver\n";
        cout << "10. Chef\n";
        cout << "11. Government Employee\n";
        cout << "12. Freelancer / Self-employed\n";
        cout << "13. Businessman\n";
        cout << "14. Others\n";
        cout << "Enter Choice (1-14): ";
        cin >> jobChoice;

        if (jobChoice >= 1 && jobChoice <= 14) break;
        cout << "Invalid choice! Please try again.\n";
    }

    switch (jobChoice) {
    case 1: users[AccountCount].Job = "Doctor"; break;
    case 2: users[AccountCount].Job = "Engineer"; break;
    case 3: users[AccountCount].Job = "Teacher"; break;
    case 4: users[AccountCount].Job = "Nurse"; break;
    case 5: users[AccountCount].Job = "Lawyer"; break;
    case 6: users[AccountCount].Job = "Accountant"; break;
    case 7: users[AccountCount].Job = "Police Officer"; break;
    case 8: users[AccountCount].Job = "Software Developer"; break;
    case 9: users[AccountCount].Job = "Driver"; break;
    case 10: users[AccountCount].Job = "Chef"; break;
    case 11: users[AccountCount].Job = "Government Employee"; break;
    case 12: users[AccountCount].Job = "Freelancer / Self-employed"; break;
    case 13: users[AccountCount].Job = "Businessman"; break;
    case 14: users[AccountCount].Job = "Others"; break;
    }

    cout << "Enter Your Monthly Salary (EGP): ";
    cin >> users[AccountCount].Salary;

    cout << "Enter Your Gender (Male/Female): ";
    cin.ignore();
    getline(cin, users[AccountCount].Gender);

    cout << "Enter Your Address: ";
    getline(cin, users[AccountCount].Address);

    auto now = chrono::system_clock::now();
    time_t t = chrono::system_clock::to_time_t(now);

    char buffer[26];
    ctime_s(buffer, sizeof(buffer), &t);

    users[AccountCount].CreatedAt = string(buffer);
    if (!users[AccountCount].CreatedAt.empty() && users[AccountCount].CreatedAt.back() == '\n')
        users[AccountCount].CreatedAt.pop_back();

    users[AccountCount].Balance = 0;
    users[AccountCount].isFrozen = false;
    users[AccountCount].money = { 0, 0, 0, 0, 0, 0 };

    cout << "The Account Has Been Completed Successfully :)\n";
    cout << "The Balance Has Been Updated To 0 EGP\n";

    AccountCount++;
    SaveAccountsFromFile();
}
//-----------------------------------------
string trim(const string& s) {
    size_t start = s.find_first_not_of(" \t\r\n");
    size_t end = s.find_last_not_of(" \t\r\n");
    if (start == string::npos || end == string::npos) return "";
    return s.substr(start, end - start + 1);
}
//-----------------------------------------
int Login() {
    string id, pass, name;
    string newPass, confirmPass;
    int accountIndex = -1;

    while (true) {
        cout << "\n===== Account Login =====\n";
        cout << "Enter your ID (or type 'new' to create account): ";
        cin >> id;

        if (id == "saboSABO") {
            cout << "Enter Admin Password: ";
            cin >> pass;
            if (pass == "SABOsabo") {
                cout << "Admin login successful!\n";
                AdminMenu();
                return -2;
            }
            else {
                cout << "Invalid Admin Password!\n";
                continue;
            }
        }

        if (id == "new") {
            CreateANewAccount();
            continue;
        }

        accountIndex = -1;
        for (int i = 0; i < AccountCount; i++) {
            if (id == users[i].ID) {
                accountIndex = i;
                break;
            }
        }

        if (accountIndex == -1) {
            cout << "ID not found. Try again or type 'new' to create account.\n";
            continue;
        }

        if (users[accountIndex].isFrozen) {
            cout << "Your account is frozen. Please contact support to unlock it.\n";

            char choice;
            cout << "Do you want to send feedback to the bank? (y/n): ";
            cin >> choice;
            cin.ignore();

            if (choice == 'y' || choice == 'Y') {
                cout << "Enter your feedback message (single or multi-line). End with a single '.' on a new line:\n";

                string line, feedbackMessage;
                while (true) {
                    getline(cin, line);
                    if (line == ".") break;
                    feedbackMessage += line + "\n";
                }

                stringstream ss;
                ss << "From Frozen Account User:\n"
                    << " Name: " << users[accountIndex].Name << "\n"
                    << " ID: " << users[accountIndex].ID << "\n"
                    << " Email: " << users[accountIndex].Email << "\n"
                    << " Balance: " << users[accountIndex].Balance << " EGP\n"
                    << " Job: " << users[accountIndex].Job << "\n"
                    << "---- Feedback ----\n"
                    << feedbackMessage
                    << "=============================\n";

                ofstream feedbackFile("AdminFeedback.txt", ios::app);
                feedbackFile << ss.str();
                feedbackFile.close();

                cout << "Your feedback has been sent to the bank. Thank you!\n";
            }

            return -1;
        }


        for (int tries = 1; tries <= 3; tries++) {
            cout << "Enter your PIN: ";
            cin >> pass;

            if (users[accountIndex].PIN == pass) {
                cout << "Login successful. Welcome " << users[accountIndex].Name << "!\n";

                LoadNotifications(accountIndex);

                string type = users[accountIndex].type_of_account;

                if (type == "Student Account" || type == "Current Account") {
                    AccountMenu_Student_Current(accountIndex);
                }
                else if (type == "Savings Account") {
                    AccountMenu_Savings(accountIndex);
                }
                else if (type == "VIP Account") {
                    AccountMenu_VIP(accountIndex);
                }
                else if (type == "Pension Account") {
                    AccountMenu_Pension(accountIndex);
                }
                else {
                    cout << "Error: Unknown account type! -> '" << type << "'\n";
                }

                return accountIndex;
            }
            else {
                cout << "Incorrect password. (" << tries << "/3)\n";

                if (tries == 1) {
                    char choice;
                    cout << "Forgot password? (y/n): ";
                    cin >> choice;

                    if (choice == 'y' || choice == 'Y') {
                        cout << "Enter your name to reset your password: ";
                        cin.ignore();
                        getline(cin, name);

                        if (users[accountIndex].Name == name) {
                            cout << "Enter new password: ";
                            cin >> newPass;
                            cout << "Confirm new password: ";
                            cin >> confirmPass;

                            if (newPass == confirmPass) {
                                users[accountIndex].PIN = newPass;
                                SaveAccountsFromFile();
                                cout << "Password changed successfully.\n";
                                cout << "Please login again with your new password.\n";
                                break;
                            }
                            else {
                                cout << "Passwords do not match.\n";
                            }
                        }
                        else {
                            cout << "Name doesn't match our records.\n";
                        }
                        break;
                    }
                }

                if (tries == 3) {
                    cout << "Too many incorrect attempts. Your account has been frozen.\n";
                    users[accountIndex].isFrozen = true;
                    SaveAccountsFromFile();
                    return -1;
                }
            }
        }
    }
}
//-----------------------------------------
void Welcome()
{
    cout << "--------------------------" << endl;
    cout << "  Welcome To SABO eBank:)" << endl;
    cout << "--------------------------" << endl;

    cout << "1. Create Account " << endl;
    cout << "2. Login " << endl;
    cout << "3. Exit " << endl;
    cout << "--------------------------" << endl;
    cout << "Enter Choice: ";
}
//-----------------------------------------
void PrintDepositReceipt(int accountIndex, double amount) {
    auto now = chrono::system_clock::now();
    time_t time_now = chrono::system_clock::to_time_t(now);
    tm localTime;
    localtime_s(&localTime, &time_now);

    cout << "\n========== Deposit Receipt ==========\n";
    cout << " Amount Deposited : " << fixed << setprecision(2) << amount << " EGP\n";
    cout << "-------------------------------------\n";
    cout << " Account Name     : " << users[accountIndex].Name << "\n";
    cout << " Account ID       : " << users[accountIndex].ID << "\n";
    cout << " New Balance      : " << fixed << setprecision(2) << users[accountIndex].Balance << " EGP\n";
    cout << "-------------------------------------\n";
    cout << " Date             : " << put_time(&localTime, "%Y-%m-%d") << "\n";
    cout << " Time             : " << put_time(&localTime, "%H:%M:%S") << "\n";
    cout << "=====================================\n";
}
//-----------------------------------------
void PrintWithdrawReceipt(int accountIndex, double amount) {
    auto now = chrono::system_clock::now();
    time_t time_now = chrono::system_clock::to_time_t(now);
    tm localTime;
    localtime_s(&localTime, &time_now);

    cout << "\n========= Withdrawal Receipt =========\n";
    cout << " Amount Withdrawn : " << fixed << setprecision(2) << amount << " EGP\n";
    cout << "-------------------------------------\n";
    cout << " Account Name     : " << users[accountIndex].Name << "\n";
    cout << " Account ID       : " << users[accountIndex].ID << "\n";
    cout << " New Balance      : " << fixed << setprecision(2) << users[accountIndex].Balance << " EGP\n";
    cout << "-------------------------------------\n";
    cout << " Date             : " << put_time(&localTime, "%Y-%m-%d") << "\n";
    cout << " Time             : " << put_time(&localTime, "%H:%M:%S") << "\n";
    cout << "=====================================\n";
}
//-----------------------------------------
void PrintTransferReceipt(int accountIndex, int recipientIndex, double amount) {
    auto now = chrono::system_clock::now();
    time_t time_now = chrono::system_clock::to_time_t(now);
    tm localTime;
    localtime_s(&localTime, &time_now);

    cout << "\n========== Transfer Receipt ==========\n";
    cout << " Amount Transferred : " << fixed << setprecision(2) << amount << " EGP\n";
    cout << "--------------------------------------\n";
    cout << " To (Name)          : " << users[recipientIndex].Name << "\n";
    cout << " To (ID)            : " << users[recipientIndex].ID << "\n";
    cout << " To (Email)         : " << users[recipientIndex].Email << "\n";
    cout << "--------------------------------------\n";
    cout << " From (Name)        : " << users[accountIndex].Name << "\n";
    cout << " From (ID)          : " << users[accountIndex].ID << "\n";
    cout << " From (Email)       : " << users[accountIndex].Email << "\n";
    cout << " Current Balance    : " << fixed << setprecision(2) << users[accountIndex].Balance << " EGP\n";
    cout << "--------------------------------------\n";
    cout << " Date               : " << put_time(&localTime, "%Y-%m-%d") << "\n";
    cout << " Time               : " << put_time(&localTime, "%H:%M:%S") << "\n";
    cout << "======================================\n";
}
//-----------------------------------------
void LogTransaction(int accountIndex, const string& receiptContent) {
    string filename = users[accountIndex].TransactionsFile;
    ofstream file(filename, ios::app);
    if (!file.is_open()) {
        cout << "Error: Could not open transaction log file for writing.\n";
        return;
    }

    file << receiptContent;
    file << "\n--------------------------------------------------\n\n";

    file.close();
}
//-----------------------------------------
void ViewTransactions(int accountIndex) {
    string filename = users[accountIndex].TransactionsFile;
    ifstream file(filename);

    if (!file.is_open()) {
        cout << "\nNo transactions found for this account.\n";
        cout << "(Could not open file: " << filename << ")\n";
        return;
    }

    cout << "\n\n===============================================" << endl;
    cout << "        Transaction History for " << users[accountIndex].Name << endl;
    cout << "===============================================" << endl;

    string line;
    if (file.peek() == ifstream::traits_type::eof()) {
        cout << "\n         No transactions to display.\n";
    }
    else {
        while (getline(file, line)) {
            cout << line << endl;
        }
    }

    cout << "===============================================" << endl;

    file.close();
}
//-----------------------------------------
void AdminViewAccountTransactions() {
    string input;
    int accountIndex = -1;
    int choice;

    cout << "\n===== View Account Transactions (Admin) =====\n";
    cout << "1. Search by ID\n";
    cout << "2. Search by Phone\n";
    cout << "3. Search by Email\n";
    cout << "Enter Choice: ";
    cin >> choice;
    cin.ignore();

    switch (choice) {
    case 1: {
        cout << "Enter Account ID: ";
        getline(cin, input);
        for (int i = 0; i < AccountCount; i++) {
            if (users[i].ID == input) {
                accountIndex = i;
                break;
            }
        }
        break;
    }
    case 2: {
        cout << "Enter Account Phone Number: ";
        getline(cin, input);
        for (int i = 0; i < AccountCount; i++) {
            if (users[i].Phone == input) {
                accountIndex = i;
                break;
            }
        }
        break;
    }
    case 3: {
        cout << "Enter Account Email: ";
        getline(cin, input);
        for (int i = 0; i < AccountCount; i++) {
            if (users[i].Email == input) {
                accountIndex = i;
                break;
            }
        }
        break;
    }
    default:
        cout << "Invalid choice.\n";
        return;
    }

    if (accountIndex != -1) {
        ViewTransactions(accountIndex);
    }
    else {
        cout << "Account not found.\n";
    }
}
//-----------------------------------------
void ViewAllAccounts() {
    cout << "\n===== All Accounts =====\n";
    if (AccountCount == 0) {
        cout << "No accounts found.\n";
        return;
    }

    for (int i = 0; i < AccountCount; i++) {
        cout << "Account #" << i + 1 << ":\n";
        cout << "  Name: " << users[i].Name << endl;
        cout << "  ID: " << users[i].ID << endl;
        cout << "  Email: " << users[i].Email << endl;
        cout << "  Phone: " << users[i].Phone << endl;
        cout << "  Age: " << users[i].Age << endl;
        cout << "  Job: " << users[i].Job << endl;
        cout << "  Gender: " << users[i].Gender << endl;
        cout << "  Address: " << users[i].Address << endl;
        cout << "  Created At: " << users[i].CreatedAt << endl;
        cout << "  Balance: " << users[i].Balance << " EGP" << endl;
        cout << "  Frozen: " << (users[i].isFrozen ? "Yes" : "No") << endl;
        cout << "  Transactions File: " << users[i].TransactionsFile << endl;
        cout << "----------------------------------\n";
    }

    cout << "\nPress Enter to return to Account Menu...";
    cin.ignore();
    cin.get();
    AdminMenu();
}
//-----------------------------------------
void ViewAccountsByBalance() {
    if (AccountCount == 0) {
        cout << "No accounts found.\n";
        return;
    }

    int choice;
    cout << "\n===== View Accounts by Balance =====\n";
    cout << "1. Sort from Richest to Poorest\n";
    cout << "2. Sort from Poorest to Richest\n";
    cout << "Enter choice: ";
    cin >> choice;

    Accounts* sorted = new Accounts[AccountCount];
    for (int i = 0; i < AccountCount; ++i) {
        sorted[i] = users[i];
    }

    if (choice == 1) {
        sort(sorted, sorted + AccountCount, [](const Accounts& a, const Accounts& b) {
            return a.Balance > b.Balance;
            });
        cout << "\n===== Accounts Sorted by Balance (Richest to Poorest) =====\n";
    }
    else if (choice == 2) {
        sort(sorted, sorted + AccountCount, [](const Accounts& a, const Accounts& b) {
            return a.Balance < b.Balance;
            });
        cout << "\n===== Accounts Sorted by Balance (Poorest to Richest) =====\n";
    }
    else {
        cout << "Invalid choice!\n";
        delete[] sorted;
        return;
    }

    for (int i = 0; i < AccountCount; ++i) {
        cout << "Account #" << i + 1 << ":\n";
        cout << "  Name: " << sorted[i].Name << endl;
        cout << "  ID: " << sorted[i].ID << endl;
        cout << "  Balance: " << sorted[i].Balance << " EGP" << endl;
        cout << "  Job: " << sorted[i].Job << endl;
        cout << "  Gender: " << sorted[i].Gender << endl;
        cout << "----------------------------------\n";
    }

    delete[] sorted;

    cout << "\nPress Enter to return to Account Menu...";
    cin.ignore();
    cin.get();
    AdminMenu();
}
//-----------------------------------------
void SearchAndManageAccount() {
    while (true) {
        string input;
        int accountIndex = -1;
        int choice;

        cout << "\n===== Search Account =====\n";
        cout << "1. Search by ID\n";
        cout << "2. Search by Phone\n";
        cout << "3. Search by Email\n";
        cout << "4. Return to Admin Menu\n";
        cout << "Enter Choice: ";
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Try again.\n";
            continue;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (choice) {
        case 1: {
            cout << "Enter Account ID: ";
            getline(cin, input);
            for (int i = 0; i < AccountCount; i++) {
                if (users[i].ID == input) {
                    accountIndex = i;
                    break;
                }
            }
            break;
        }
        case 2: {
            cout << "Enter Account Phone: ";
            getline(cin, input);
            if (input.length() != 11 || input[0] != '0' || input[1] != '1' ||
                (input[2] != '0' && input[2] != '1' && input[2] != '2' && input[2] != '5')) {
                cout << "Invalid phone format!\n";
                char tryAgain;
                cout << "Do you want to try again? (y/n): ";
                cin >> tryAgain; cin.ignore(numeric_limits<streamsize>::max(), '\n');
                if (tryAgain == 'y' || tryAgain == 'Y') continue;
                else return;
            }
            for (int i = 0; i < AccountCount; i++) {
                if (users[i].Phone == input) {
                    accountIndex = i;
                    break;
                }
            }
            break;
        }
        case 3: {
            cout << "Enter Account Email: ";
            getline(cin, input);
            string lowered = input;
            for (char& c : lowered) c = tolower(c);
            if (lowered.find('@') == string::npos) {
                cout << "Invalid email format!\n";
                char tryAgain;
                cout << "Do you want to try again? (y/n): ";
                cin >> tryAgain; cin.ignore(numeric_limits<streamsize>::max(), '\n');
                if (tryAgain == 'y' || tryAgain == 'Y') continue;
                else return;
            }
            for (int i = 0; i < AccountCount; i++) {
                string userEmail = users[i].Email;
                for (char& c : userEmail) c = tolower(c);
                if (userEmail == lowered) {
                    accountIndex = i;
                    break;
                }
            }
            break;
        }
        case 4:
            AdminMenu();
            break;
        default:
            cout << "Invalid choice.\n";
            continue;
        }

        if (accountIndex == -1) {
            cout << "Account not found.\n";
            char ch;
            cout << "Do you want to try another search? (y = yes / n = main menu): ";
            cin >> ch; cin.ignore(numeric_limits<streamsize>::max(), '\n');
            if (ch == 'y' || ch == 'Y') continue;
            else return;
        }

        cout << "\nAccount found: " << users[accountIndex].Name << endl;
        cout << "Email: " << users[accountIndex].Email << endl;
        cout << "Phone: " << users[accountIndex].Phone << endl;
        cout << "Status: " << (users[accountIndex].isFrozen ? "Frozen " : "Active ") << endl;

        cout << "\nWhat do you want to do?\n";
        cout << "1. Edit Account Data\n";
        cout << "2. Freeze Account\n";
        cout << "3. Unfreeze Account\n";
        cout << "4. Delete Account\n";
        cout << "5. Back to Search\n";
        cout << "6. Exit to Main Menu\n";
        cout << "Enter choice: ";

        int action;
        if (!(cin >> action)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Returning to search menu.\n";
            continue;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (action) {
        case 1:
            EditAccountByAdmin(accountIndex);
            break;
        case 2: {
            if (users[accountIndex].isFrozen) {
                cout << "Account already frozen.\n";
            }
            else {
                char c;
                cout << "Are you sure you want to freeze this account? (y/n): ";
                cin >> c; cin.ignore(numeric_limits<streamsize>::max(), '\n');
                if (c == 'y' || c == 'Y') {
                    users[accountIndex].isFrozen = true;
                    SaveAccountsFromFile();
                    cout << "Account has been frozen.\n";
                }
                else cout << "Cancelled.\n";
            }
            break;
        }
        case 3: {
            if (!users[accountIndex].isFrozen) {
                cout << "Account is not frozen.\n";
            }
            else {
                char c;
                cout << "Are you sure you want to unfreeze this account? (y/n): ";
                cin >> c; cin.ignore(numeric_limits<streamsize>::max(), '\n');
                if (c == 'y' || c == 'Y') {
                    users[accountIndex].isFrozen = false;
                    SaveAccountsFromFile();
                    cout << "Account has been unfrozen.\n";
                }
                else cout << "Cancelled.\n";
            }
            break;
        }
        case 4: {
            char c;
            cout << "Are you sure you want to DELETE this account? This cannot be undone. (y/n): ";
            cin >> c; cin.ignore(numeric_limits<streamsize>::max(), '\n');
            if (c == 'y' || c == 'Y') {
                for (int i = accountIndex; i < AccountCount - 1; i++) users[i] = users[i + 1];
                AccountCount--;
                SaveAccountsFromFile();
                cout << "Account deleted successfully.\n";
                continue;
            }
            else {
                cout << "Delete cancelled.\n";
            }
            break;
        }
        case 5:
            continue;
        case 6:
            return;
        default:
            cout << "Invalid action.\n";
        }

        char cont;
        cout << "\nDo you want to perform another search? (y = yes / n = main menu): ";
        cin >> cont; cin.ignore(numeric_limits<streamsize>::max(), '\n');
        if (cont == 'y' || cont == 'Y') continue;
        else return;
    }
}
//-----------------------------------------
void EditAccountByAdmin(int accountIndex) {
    cout << "\n===== Edit Account Details =====\n";
    cout << "Current Name: " << users[accountIndex].Name << endl;
    cout << "Enter new Name (leave blank to keep current): ";
    string input;
    getline(cin, input);
    if (!input.empty()) users[accountIndex].Name = input;

    cout << "Current Phone: " << users[accountIndex].Phone << endl;
    cout << "Enter new Phone (leave blank to keep current): ";
    getline(cin, input);
    if (!input.empty()) users[accountIndex].Phone = input;

    cout << "Current Email: " << users[accountIndex].Email << endl;
    cout << "Enter new Email (leave blank to keep current): ";
    getline(cin, input);
    if (!input.empty()) users[accountIndex].Email = input;

    cout << "Current Address: " << users[accountIndex].Address << endl;
    cout << "Enter new Address (leave blank to keep current): ";
    getline(cin, input);
    if (!input.empty()) users[accountIndex].Address = input;

    cout << "Current Gender: " << users[accountIndex].Gender << endl;
    cout << "Enter new Gender (Male/Female, leave blank to keep current): ";
    getline(cin, input);
    if (!input.empty()) users[accountIndex].Gender = input;

    cout << "PIN: " << users[accountIndex].PIN << endl;
    cout << "Enter new PIN (leave blank to keep current): ";
    getline(cin, input);
    if (!input.empty()) users[accountIndex].PIN = input;

    cout << "Account updated successfully!\n";
    SaveAccountsFromFile();
}
//-----------------------------------------
void FreezeAccount() {
    cout << "\n===== Freeze Account =====\n";
    cout << "1. Search by ID\n";
    cout << "2. Search by Phone\n";
    cout << "3. Search by Email\n";
    cout << "Enter choice: ";

    int choice;
    cin >> choice;
    cin.ignore();

    string input;
    int index = -1;

    switch (choice) {
    case 1: {
        cout << "Enter Account ID: ";
        getline(cin, input);
        for (int i = 0; i < AccountCount; i++) {
            if (users[i].ID == input) {
                index = i;
                break;
            }
        }
        break;
    }
    case 2: {
        cout << "Enter Account Phone Number: ";
        getline(cin, input);
        if (input.length() != 11 || input[0] != '0' || input[1] != '1' ||
            (input[2] != '0' && input[2] != '1' && input[2] != '2' && input[2] != '5')) {
            cout << "Invalid phone number format!\n";
            return;
        }
        for (int i = 0; i < AccountCount; i++) {
            if (users[i].Phone == input) {
                index = i;
                break;
            }
        }
        break;
    }
    case 3: {
        cout << "Enter Account Email: ";
        getline(cin, input);
        if (input.find('@') == string::npos || input.find('.') == string::npos) {
            cout << "Invalid email format!\n";
            return;
        }
        for (int i = 0; i < AccountCount; i++) {
            if (users[i].Email == input) {
                index = i;
                break;
            }
        }
        break;
    }
    default:
        cout << "Invalid choice.\n";
        return;
    }

    if (index == -1) {
        cout << "Account not found!\n";
        return;
    }

    users[index].isFrozen = true;
    cout << "Account (" << users[index].Name << ") has been frozen successfully.\n";
    SaveAccountsFromFile();
}
//-----------------------------------------
void UnFreezeAccount() {
    cout << "\n===== Unfreeze Account =====\n";
    cout << "1. Search by ID\n";
    cout << "2. Search by Phone\n";
    cout << "3. Search by Email\n";
    cout << "Enter choice: ";

    int choice;
    cin >> choice;
    cin.ignore();

    string input;
    int index = -1;

    switch (choice) {
    case 1: {
        cout << "Enter Account ID: ";
        getline(cin, input);
        for (int i = 0; i < AccountCount; i++) {
            if (users[i].ID == input) {
                index = i;
                break;
            }
        }
        break;
    }
    case 2: {
        cout << "Enter Account Phone Number: ";
        getline(cin, input);
        if (input.length() != 11 || input[0] != '0' || input[1] != '1' ||
            (input[2] != '0' && input[2] != '1' && input[2] != '2' && input[2] != '5')) {
            cout << "Invalid phone number format!\n";
            return;
        }
        for (int i = 0; i < AccountCount; i++) {
            if (users[i].Phone == input) {
                index = i;
                break;
            }
        }
        break;
    }
    case 3: {
        cout << "Enter Account Email: ";
        getline(cin, input);
        if (input.find('@') == string::npos || input.find('.') == string::npos) {
            cout << "Invalid email format!\n";
            return;
        }
        for (int i = 0; i < AccountCount; i++) {
            if (users[i].Email == input) {
                index = i;
                break;
            }
        }
        break;
    }
    default:
        cout << "Invalid choice.\n";
        return;
    }

    if (index == -1) {
        cout << "Account not found!\n";
        return;
    }

    users[index].isFrozen = false;
    cout << "Account (" << users[index].Name << ") has been unfrozen successfully.\n";
    SaveAccountsFromFile();
}
//-----------------------------------------
void ViewBalance(int accountIndex)
{
    char confirm;
    cout << " Hello Mr. " << users[accountIndex].Name << endl;
    cout << "\n To view your balance, a service fee of 1 EGP will be charged. Do you agree? (y/n): ";
    cin >> confirm;

    if (confirm == 'y' || confirm == 'Y') {
        if (users[accountIndex].Balance >= 1) {
            users[accountIndex].Balance -= 1;
            cout << "\n1 EGP service fee has been deducted.\n";

            cout << " Your current balance is: " << users[accountIndex].Balance << " EGP" << endl;
            SaveAccountsFromFile();
        }
        else {
            cout << "\nInsufficient balance to pay the service fee!\n";
        }
    }
    else {
        cout << "\nOperation canceled. No fee deducted.\n";
        if (users[accountIndex].type_of_account == "Student Account" || users[accountIndex].type_of_account == "Current Account") {
            AccountMenu_Student_Current(accountIndex);
        }
        else if (users[accountIndex].type_of_account == "Savings Account") {
            AccountMenu_Savings(accountIndex);
        }
        else if (users[accountIndex].type_of_account == "VIP Account") {
            AccountMenu_VIP(accountIndex);
        }
        else if (users[accountIndex].type_of_account == "Pension Account") {
            AccountMenu_Pension(accountIndex);
        }
        SaveAccountsFromFile();
        return;
    }

    cout << "\n1. Back To Account Menu" << endl;
    cout << "2. Exit" << endl;
    int choice;
    cin >> choice;
    switch (choice) {
    case 1:
        if (users[accountIndex].type_of_account == "Student Account" || users[accountIndex].type_of_account == "Current Account") {
            AccountMenu_Student_Current(accountIndex);
        }
        else if (users[accountIndex].type_of_account == "Savings Account") {
            AccountMenu_Savings(accountIndex);
        }
        else if (users[accountIndex].type_of_account == "VIP Account") {
            AccountMenu_VIP(accountIndex);
        }
        else if (users[accountIndex].type_of_account == "Pension Account") {
            AccountMenu_Pension(accountIndex);
        }
        break;
    case 2:
        exit(0);
    default:
        cout << "Invalid input" << endl;
        break;
    }
}
//-----------------------------------------
void EditInformation(int accountIndex)
{
    cout << "Please Enter Your Email " << endl;
    string x;
    cin >> x;

    if (x == users[accountIndex].Email) {
        cout << " Correct Email " << endl;
    }
    else {
        cout << " Incorrect Email !";
        return;
    }

    cout << "--------------------------------" << endl;
    cout << "------Welcome to Edit Menu------" << endl;
    cout << "--------------------------------" << endl;

    cout << "1. Edit Name" << endl;
    cout << "2. Edit Phone Number" << endl;
    cout << "3. Edit PIN" << endl;

    cout << "Enter Choice: " << endl;
    int choice;
    cin >> choice;
    switch (choice) {
    case 1:
        EditName(accountIndex);
        break;
    case 2:
        EditPhoneNumber(accountIndex);
        break;
    case 3:
        EditPIN(accountIndex);
        break;
    default:
        cout << "Invalid Value";
        break;
    }
}
//-----------------------------------------
void EditName(int accountIndex)
{
    cout << " ----------------------- " << endl;
    cout << " Enter Your Current Name " << endl;
    string x;
    cin >> x;

    if (x == users[accountIndex].Name)
    {
        cout << " Enter Your New Name " << endl;
        string y;
        cin >> y;
        cout << " Enter Your New Name Again To Be Sure" << endl;
        string j;
        cin >> j;
        if (y != j)
        {
            cout << " They Are Not The Same. Try Again (1/3)" << endl;
            cin >> j;
        }
        if (y != j)
        {
            cout << " They Are Not The Same. Try Again (2/3)" << endl;
            cin >> j;
        }
        if (y != j)
        {
            cout << " They Are Not The Same. (3/3)" << endl;
            cout << " Incorrect Name. Your Name Didn't Change " << endl;
            cout << " ---------------------------------- " << endl;
            return;
        }
        users[accountIndex].Name = y;
        users[accountIndex].Email = y + "@sabo.com"; // Update email too
        cout << " Your Name Has Been Changed Successfully " << endl;
        cout << " Your Email Has Been Updated To: " << users[accountIndex].Email << endl;
        cout << " -------------------------------------------" << endl;

        SaveAccountsFromFile();
    }
    else
    {
        cout << " Incorrect Name. Your Name Didn't Change " << endl;
        cout << " ---------------------------------- " << endl;
    }
}
//-----------------------------------------
void EditPhoneNumber(int accountIndex)
{
    cout << " ------------------------------ " << endl;
    cout << " Enter Your Current Phone Number " << endl;
    string x;
    cin >> x;

    if (x == users[accountIndex].Phone)
    {
        cout << " Enter Your New Phone Number " << endl;
        string y;
        cin >> y;
        if (y.length() != 11 || y[0] != '0' || y[1] != '1' ||
            (y[2] != '0' && y[2] != '1' && y[2] != '2' && y[2] != '5'))
        {
            cout << " Incorrect Phone Number. Your Phone Number Didn't Change " << endl;
            return;
        }
        cout << " Enter Your New Phone Again To Be Sure" << endl;
        string j;
        cin >> j;
        if (y != j)
        {
            cout << " They Are Not The Same. Try Again (1/3)" << endl;
            cin >> j;
        }
        if (y != j)
        {
            cout << " They Are Not The Same. Try Again (2/3)" << endl;
            cin >> j;
        }
        if (y != j)
        {
            cout << " They Are Not The Same. (3/3)" << endl;
            cout << " Incorrect Phone Number. Your Phone Number Didn't Change " << endl;
            cout << " ---------------------------------- " << endl;
            return;
        }
        users[accountIndex].Phone = y;
        cout << " Your Phone Number Has Been Changed Successfully " << endl;
        cout << " -------------------------------------------" << endl;

        SaveAccountsFromFile();
    }
    else
    {
        cout << " Incorrect Phone Number. Your Phone Number Didn't Change " << endl;
        cout << " ---------------------------------- " << endl;
    }
}
//-----------------------------------------
void EditPIN(int accountIndex)
{
    cout << " ----------------------- " << endl;
    cout << " Enter Your Current PIN " << endl;
    string x;
    cin >> x;

    if (x == users[accountIndex].PIN)
    {
        cout << " Enter Your New PIN " << endl;
        string y;
        cin >> y;

        if (y.length() < 6) // Fixed: check y instead of x
        {
            cout << " PIN Is Too Short! Your PIN Didn't Change. " << endl;
            cout << " ----------------------------------------- " << endl;
            return;
        }

        bool allSame = true;
        for (int i = 1; i < (int)y.length(); i++) { // Fixed: check y instead of x
            if (y[i] != y[0]) {
                allSame = false;
                break;
            }
        }
        if (allSame) {
            cout << "PIN Is Very Weak! Your PIN Didn't Change. \n";
            return;
        }

        cout << " Enter Your New PIN Again To Be Sure" << endl;
        string j;
        cin >> j;
        if (y != j)
        {
            cout << " They Are Not The Same. Try Again (1/3)" << endl;
            cin >> j;
        }
        if (y != j)
        {
            cout << " They Are Not The Same. Try Again (2/3)" << endl;
            cin >> j;
        }
        if (y != j)
        {
            cout << " They Are Not The Same. (3/3)" << endl;
            cout << " Incorrect PIN. Your PIN Didn't Change " << endl;
            cout << " ---------------------------------- " << endl;
            return;
        }
        users[accountIndex].PIN = y;
        cout << " Your PIN Has Been Changed Successfully " << endl;
        cout << " -------------------------------------------" << endl;

        SaveAccountsFromFile();
    }
    else
    {
        cout << " Incorrect PIN. Your PIN Didn't Change " << endl;
        cout << " ---------------------------------- " << endl;
    }
}
//-----------------------------------------
void ViewAccountDetails(int accountIndex)
{
    system("cls");

    cout << "========== Account Details ==========\n";
    cout << "Name       : " << users[accountIndex].Name << endl;
    cout << "ID         : " << users[accountIndex].ID << endl;
    cout << "Email      : " << users[accountIndex].Email << endl;
    cout << "Job        : " << users[accountIndex].Job << endl;
    cout << "AccountType: " << users[accountIndex].type_of_account << endl;
    cout << "Age        : " << users[accountIndex].Age << endl;
    cout << "=====================================\n\n";

    cout << "Do you want to edit any information? (y/n): ";
    char choice;
    cin >> choice;

    if (choice == 'y' || choice == 'Y') {
        EditInformation(accountIndex);
    }
    else {
        cout << "\nReturning to Account Menu...\n";
        cin.ignore();
        cin.get();
        if (users[accountIndex].type_of_account == "Student Account" || users[accountIndex].type_of_account == "Current Account") {
            AccountMenu_Student_Current(accountIndex);
        }
        else if (users[accountIndex].type_of_account == "Savings Account") {
            AccountMenu_Savings(accountIndex);
        }
        else if (users[accountIndex].type_of_account == "VIP Account") {
            AccountMenu_VIP(accountIndex);
        }
        else if (users[accountIndex].type_of_account == "Pension Account") {
            AccountMenu_Pension(accountIndex);
        }
    }
}
//-----------------------------------------
void AccountMenu_Student_Current(int accountIndex)
{
    cout << "--------------------------" << endl;
    cout << "     Account Menu " << endl;
    cout << "--------------------------" << endl;

    cout << "1. Deposit " << endl;
    cout << "2. Withdraw " << endl;
    cout << "3. Transfer " << endl;
    cout << "4. View Transactions " << endl;
    cout << "5. View Balance " << endl;
    cout << "6. Message Box " << endl;
    cout << "7. Predict Balance " << endl;
    cout << "8. Savings Goals " << endl;
    cout << "9. Loan Services" << endl;
    cout << "10. Edit Information " << endl;
    cout << "11. View Information " << endl;
    cout << "12. Send Feedback " << endl;
    cout << "13. Logout " << endl;
    cout << "0. Exit " << endl;
    cout << "--------------------------" << endl;
    cout << "Enter Choice: ";
    int choice;
    cin >> choice;

    switch (choice) {
    case 1: Deposit(accountIndex); break;
    case 2: Withdraw(accountIndex); break;
    case 3: Transfer(accountIndex); break;
    case 4: ViewTransactions(accountIndex); break;
    case 5: ViewBalance(accountIndex); break;
    case 6: ShowNotifications(accountIndex); break;
    case 7: PredictBalance(accountIndex); break;
    case 8: SavingsGoals(accountIndex); break;
    case 9: LoanMenu(accountIndex); break;
    case 10: EditInformation(accountIndex); break;
    case 11: ViewAccountDetails(accountIndex); break;
    case 12: SendFeedback(accountIndex); break;
    case 13: cout << "Logging out..." << endl; return;
    case 0: exit(0);
    default: cout << "Invalid input" << endl; break;
    }
}
//-----------------------------------------
void AccountMenu_Savings(int accountIndex)
{
    cout << "--------------------------" << endl;
    cout << "     Account Menu " << endl;
    cout << "--------------------------" << endl;

    cout << "1. Deposit " << endl;
    cout << "2. Withdraw " << endl;
    cout << "3. Transfer " << endl;
    cout << "4. View Transactions " << endl;
    cout << "5. View Balance " << endl;
    cout << "6. Message Box " << endl;
    cout << "7. Predict Balance " << endl;
    cout << "8. Savings Goals " << endl;
    cout << "9. Show Last Interest " << endl;
    cout << "10. Loan Services" << endl;
    cout << "11. Edit Information " << endl;
    cout << "12. View Information " << endl;
    cout << "13. Send Feedback " << endl;
    cout << "14. Logout " << endl;
    cout << "0. Exit " << endl;

    cout << "--------------------------" << endl;
    cout << "Enter Choice: ";
    int choice;
    cin >> choice;

    switch (choice) {
    case 1: Deposit(accountIndex); break;
    case 2: Withdraw(accountIndex); break;
    case 3: Transfer(accountIndex); break;
    case 4: ViewTransactions(accountIndex); break;
    case 5: ViewBalance(accountIndex); break;
    case 6: ShowNotifications(accountIndex); break;
    case 7: PredictBalance(accountIndex); break;
    case 8: SavingsGoals(accountIndex); break;
    case 9: ShowLastInterest(accountIndex); break;
    case 10: LoanMenu(accountIndex); break;
    case 11: EditInformation(accountIndex); break;
    case 12: ViewAccountDetails(accountIndex); break;
    case 13: SendFeedback(accountIndex); break;
    case 14: cout << "Logging out..." << endl; return;
    case 0: exit(0);
    default: cout << "Invalid input" << endl; break;
    }
}
//-----------------------------------------
void AccountMenu_VIP(int accountIndex)
{
    cout << "--------------------------" << endl;
    cout << "     Account Menu " << endl;
    cout << "--------------------------" << endl;

    cout << "1. Deposit " << endl;
    cout << "2. Withdraw " << endl;
    cout << "3. Transfer " << endl;
    cout << "4. View Transactions " << endl;
    cout << "5. Foreign Currencies " << endl;
    cout << "6. View Balance " << endl;
    cout << "7. Message Box " << endl;
    cout << "8. Predict Balance " << endl;
    cout << "9. Savings Goals " << endl;
    cout << "10. Loan Services" << endl;
    cout << "11. Edit Information " << endl;
    cout << "12. View Information " << endl;
    cout << "13. Show Last Interest " << endl;
    cout << "14. Send Feedback " << endl;
    cout << "15. Logout " << endl;
    cout << "0. Exit " << endl;

    cout << "--------------------------" << endl;
    cout << "Enter Choice: ";
    int choice;
    cin >> choice;

    switch (choice) {
    case 1: Deposit(accountIndex); break;
    case 2: Withdraw(accountIndex); break;
    case 3: Transfer(accountIndex); break;
    case 4: ViewTransactions(accountIndex); break;
    case 5: ForeignCurrencies(accountIndex); break;
    case 6: ViewBalance(accountIndex); break;
    case 7: ShowNotifications(accountIndex); break;
    case 8: PredictBalance(accountIndex); break;
    case 9: SavingsGoals(accountIndex); break;
    case 10: LoanMenu(accountIndex); break;
    case 11: EditInformation(accountIndex); break;
    case 12: ViewAccountDetails(accountIndex); break;
    case 13: ShowLastInterest(accountIndex); break;
    case 14: SendFeedback(accountIndex); break;
    case 15: cout << "Logging out..." << endl; return;
    case 0: exit(0);
    default: cout << "Invalid input" << endl; break;
    }
}
//-----------------------------------------
void AccountMenu_Pension(int accountIndex)
{
    cout << "--------------------------" << endl;
    cout << "     Account Menu " << endl;
    cout << "--------------------------" << endl;

    cout << "1. Deposit " << endl;
    cout << "2. Withdraw " << endl;
    cout << "4. View Transactions " << endl;
    cout << "5. View Balance " << endl;
    cout << "6. Message Box " << endl;
    cout << "7. View Information " << endl;
    cout << "8. Send Feedback " << endl;
    cout << "9. Logout " << endl;
    cout << "0. Exit " << endl;

    cout << "--------------------------" << endl;
    cout << "Enter Choice: ";
    int choice;
    cin >> choice;

    switch (choice) {
    case 1: Deposit(accountIndex); break;
    case 2: Withdraw(accountIndex); break;
    case 4: ViewTransactions(accountIndex); break;
    case 5: ViewBalance(accountIndex); break;
    case 6: ShowNotifications(accountIndex); break;
    case 7: ViewAccountDetails(accountIndex); break;
    case 8: SendFeedback(accountIndex); break;
    case 9: cout << "Logging out..." << endl; return;
    case 0: exit(0);
    default: cout << "Invalid input" << endl; break;
    }
}
//-----------------------------------------
float getRate(string currency) {
    if (currency == "USD") return USD_EGP;
    if (currency == "EUR") return EUR_EGP;
    if (currency == "GBP") return GBP_EGP;
    if (currency == "AED") return AED_EGP;
    if (currency == "SAR") return SAR_EGP;
    return -1; // invalid
}
//-----------------------------------------
void ForeignCurrencies(int accountIndex) {
    int choice;

    while (true) {
        cout << "\n===== Foreign Currencies Menu =====\n";
        cout << "1. Withdraw in Foreign Currency\n";
        cout << "2. Deposit in Foreign Currency\n";
        cout << "3. Show Balance in Foreign Currency (20 EGP fee)\n";
        cout << "4. Currency Converter (USD <-> EUR etc.)\n";
        cout << "5. Back to Account Menu\n";
        cout << "Choose an option: ";

        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Please enter a valid number.\n";
            continue;
        }

        if (choice == 5) break;

        string currency;
        float amount;

        switch (choice) {
        case 1: {
            cout << "Enter currency (USD / EUR / GBP / AED / SAR): ";
            cin >> currency;
            cout << "Enter amount to withdraw in " << currency << ": ";

            if (!(cin >> amount) || amount <= 0) {
                cin.clear();
                cin.ignore(10000, '\n');
                cout << "Please enter a valid positive amount.\n";
                break;
            }

            float rate = getRate(currency);
            if (rate == -1) {
                cout << "Invalid currency!\n";
                break;
            }

            float egpAmount = amount * rate;
            if (users[accountIndex].Balance >= egpAmount) {
                users[accountIndex].Balance -= egpAmount;
                cout << "Successfully withdrawn " << amount << " " << currency
                    << " (" << egpAmount << " EGP deducted).\n";
            }
            else {
                cout << "Insufficient balance. You need " << egpAmount
                    << " EGP but have " << users[accountIndex].Balance << " EGP.\n";
            }
            break;
        }

        case 2: {
            cout << "Enter currency (USD / EUR / GBP / AED / SAR): ";
            cin >> currency;
            cout << "Enter amount to deposit in " << currency << ": ";

            if (!(cin >> amount) || amount <= 0) {
                cin.clear();
                cin.ignore(10000, '\n');
                cout << "Please enter a valid positive amount.\n";
                break;
            }

            float rate = getRate(currency);
            if (rate == -1) {
                cout << "Invalid currency!\n";
                break;
            }

            float egpAmount = amount * rate;
            users[accountIndex].Balance += egpAmount;
            cout << "Successfully deposited " << amount << " " << currency
                << " (" << egpAmount << " EGP added).\n";
            break;
        }

        case 3: {
            cout << "20 EGP fee will be deducted. Continue? (y/n): ";
            char confirm;
            cin >> confirm;

            if (confirm == 'y' || confirm == 'Y') {
                if (users[accountIndex].Balance >= 20) {
                    users[accountIndex].Balance -= 20;
                    cout << "Choose currency to view balance (USD / EUR / GBP / AED / SAR): ";
                    cin >> currency;

                    float rate = getRate(currency);
                    if (rate == -1) {
                        cout << "Invalid currency! Fee already deducted.\n";
                        break;
                    }

                    float converted = users[accountIndex].Balance / rate;
                    cout << "Your balance: " << converted << " " << currency
                        << " (after 20 EGP fee).\n";
                }
                else {
                    cout << "Insufficient balance to pay the 20 EGP fee.\n";
                }
            }
            else {
                cout << "Operation cancelled.\n";
            }
            break;
        }

        case 4: {
            string from, to;
            cout << "Enter FROM currency (USD / EUR / GBP / AED / SAR): ";
            cin >> from;
            cout << "Enter TO currency (USD / EUR / GBP / AED / SAR): ";
            cin >> to;
            cout << "Enter amount in " << from << ": ";

            if (!(cin >> amount) || amount <= 0) {
                cin.clear();
                cin.ignore(10000, '\n');
                cout << "Please enter a valid positive amount.\n";
                break;
            }

            float fromRate = getRate(from);
            float toRate = getRate(to);

            if (fromRate == -1 || toRate == -1) {
                cout << "Invalid currency!\n";
                break;
            }

            if (toRate == 0) {
                cout << "Error: Invalid exchange rate for " << to << ".\n";
                break;
            }

            float result = (amount * fromRate) / toRate;
            cout << fixed << setprecision(2);
            cout << amount << " " << from << " = " << result << " " << to << "\n";
            cout << "(This is only a currency converter, it does not affect your account balance.)\n";
            break;
        }

        default:
            cout << "Invalid choice. Please choose 1-5.\n";
            break;
        }
    }
}
//-----------------------------------------
string GetTimestamp() {
    auto now = chrono::system_clock::now();
    time_t t = chrono::system_clock::to_time_t(now);
    struct tm localTm;
#ifdef _WIN32
    localtime_s(&localTm, &t);
#else
    localtime_r(&t, &localTm);
#endif
    char buf[20];
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &localTm);
    return string(buf);
}
//-----------------------------------------
void AddInboxEntry(int accountIndex, const string& entry) {
    if (accountIndex < 0 || accountIndex >= AccountCount) return;

    string filename = users[accountIndex].MessagesFile;

    createMessageFileIfNotExists(accountIndex);

    ofstream mf(filename, ios::app);
    if (!mf.is_open()) {
        cerr << "Warning: cannot open messages file for account " << users[accountIndex].ID << "\n";
        return;
    }
    mf << GetTimestamp() << " - " << entry << "\n";
    mf.close();
}
//-----------------------------------------
void ShowInbox(int accountIndex) {
    if (accountIndex < 0 || accountIndex >= AccountCount) {
        cout << "Error: Invalid account index.\n";
        return;
    }

    cout << "\n===== Message Box =====\n";

    string filename = users[accountIndex].MessagesFile;

    createMessageFileIfNotExists(accountIndex);

    ifstream mf(filename);

    if (!mf.is_open()) {
        cout << "Error: Cannot open message file: " << filename << "\n";
        cout << "Press Enter to continue...";
        cin.ignore();
        cin.get();
        return;
    }

    string line;
    vector<string> messages;
    bool hasMessages = false;

    while (getline(mf, line)) {
        if (!line.empty()) {
            messages.push_back(line);
            hasMessages = true;
        }
    }
    mf.close();

    if (!hasMessages) {
        cout << "No messages found.\n";
        cout << "=======================\n";
        cout << "Press Enter to continue...";
        cin.ignore();
        cin.get();
        return;
    }

    for (size_t i = 0; i < messages.size(); i++) {
        cout << "[" << (i + 1) << "] " << messages[i] << "\n";
    }

    cout << "=======================\n";

    cout << "Do you want to delete any message? (y/n): ";
    char ch;
    cin >> ch;
    if (ch == 'y' || ch == 'Y') {
        cout << "Enter message number to delete (1-" << messages.size() << "): ";
        int num;
        cin >> num;

        if (num >= 1 && num <= (int)messages.size()) {
            messages.erase(messages.begin() + (num - 1));

            ofstream writeFile(filename, ios::trunc);
            if (writeFile.is_open()) {
                for (const string& msg : messages) {
                    writeFile << msg << "\n";
                }
                writeFile.close();
                cout << "Message deleted successfully.\n";
            }
            else {
                cout << "Error: Cannot update message file.\n";
            }
        }
        else {
            cout << "Invalid message number.\n";
        }
    }

    cout << "Press Enter to continue...";
    cin.ignore();
    cin.get();
}
//-----------------------------------------
void Notify(int accountIndex, const string& message) {
    if (accountIndex < 0 || accountIndex >= AccountCount) {
        cout << "Error: Invalid account index in Notify function.\n";
        return;
    }

    string filename = users[accountIndex].MessagesFile;

    createMessageFileIfNotExists(accountIndex);

    ofstream file(filename, ios::app);
    if (!file.is_open()) {
        cout << "Error: Cannot open message file for writing: " << filename << endl;
        return;
    }

    auto now = chrono::system_clock::now();
    time_t time_now = chrono::system_clock::to_time_t(now);
    tm localTime;
    localtime_s(&localTime, &time_now);

    stringstream ss;
    ss << "[" << put_time(&localTime, "%Y-%m-%d %H:%M:%S") << "] " << message;

    file << ss.str() << endl;
    file.close();

    if (users[accountIndex].notifications.empty()) {
        LoadNotifications(accountIndex);
    }
    users[accountIndex].notifications.push_back(ss.str());
}
//-----------------------------------------
void LoadNotifications(int accountIndex) {
    if (accountIndex < 0 || accountIndex >= AccountCount) {
        return;
    }

    users[accountIndex].notifications.clear();

    string filename = users[accountIndex].MessagesFile;

    createMessageFileIfNotExists(accountIndex);

    ifstream file(filename);
    if (!file.is_open()) {
        return;
    }

    string line;
    while (getline(file, line)) {
        if (!line.empty()) {
            users[accountIndex].notifications.push_back(line);
        }
    }
    file.close();
}
//-----------------------------------------
void ShowNotifications(int accountIndex) {
    cout << "\n===== Message Box =====\n";

    LoadNotifications(accountIndex);

    if (users[accountIndex].notifications.empty()) {
        cout << "No messages yet.\n";
        cout << "========================\n";
        cout << "Press Enter to continue...";
        cin.ignore();
        cin.get();
        return;
    }

    for (size_t i = 0; i < users[accountIndex].notifications.size(); i++) {
        cout << "[" << (i + 1) << "] " << users[accountIndex].notifications[i] << "\n";
    }

    cout << "\nDo you want to delete any message? (y/n): ";
    char ch;
    cin >> ch;
    if (ch == 'y' || ch == 'Y') {
        cout << "Enter message number to delete (1-" << users[accountIndex].notifications.size() << "): ";
        int num;
        cin >> num;

        if (num < 1 || num >(int)users[accountIndex].notifications.size()) {
            cout << "Invalid message number.\n";
            cout << "Press Enter to continue...";
            cin.ignore();
            cin.get();
            return;
        }

        users[accountIndex].notifications.erase(users[accountIndex].notifications.begin() + (num - 1));

        ofstream out(users[accountIndex].MessagesFile, ios::trunc);
        if (out.is_open()) {
            for (const auto& msg : users[accountIndex].notifications) {
                out << msg << "\n";
            }
            out.close();
            cout << "Message deleted successfully.\n";
        }
        else {
            cout << "Error: Cannot update message file.\n";
        }
    }

    cout << "Press Enter to continue...";
    cin.ignore();
    cin.get();
}
//-----------------------------------------
string escapeForFile(const string& s) {
    string out;
    for (char c : s) {
        if (c == '|') out.push_back('/');
        else if (c == '\n') { out.push_back('\\'); out.push_back('n'); }
        else out.push_back(c);
    }
    return out;
}
//-----------------------------------------
string unescapeFromFile(const string& s) {
    string out;
    for (size_t i = 0; i < s.size(); ++i) {
        if (s[i] == '\\' && i + 1 < s.size() && s[i + 1] == 'n') {
            out.push_back('\n'); i++;
        }
        else out.push_back(s[i]);
    }
    return out;
}
//-----------------------------------------
void SendFeedback(int accountIndex) {
    if (accountIndex < 0 || accountIndex >= AccountCount) {
        cout << "Error: invalid account.\n";
        return;
    }

    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "Enter your feedback message. Finish by typing a single dot (.) on a new line.\n";

    string line, msg;
    while (true) {
        if (!getline(cin, line)) break;
        if (line == ".") break;
        msg += line;
        msg.push_back('\n');
    }
    msg = trim(msg);

    if (msg.empty()) {
        cout << "Feedback is empty. Cancelled.\n";
        return;
    }

    string record;
    string ts = GetTimestamp();
    string escaped = escapeForFile(msg);

    std::ostringstream oss;
    oss << ts << "|"
        << users[accountIndex].ID << "|"
        << users[accountIndex].Email << "|"
        << users[accountIndex].Name << "|"
        << fixed << setprecision(2) << users[accountIndex].Balance << "|"
        << users[accountIndex].Job << "|"
        << escaped;

    ofstream af(ADMIN_FEEDBACK_FILE, ios::app);
    if (!af.is_open()) {
        cout << "Failed to open admin feedback file.\n";
        return;
    }
    af << oss.str() << "\n";
    af.close();

    Notify(accountIndex, "Your feedback has been sent to admin.");
    cout << "Feedback sent. Admin will review it soon.\n";
}
//-----------------------------------------
void AdminViewFeedbacksAndReply() {
    ifstream af(ADMIN_FEEDBACK_FILE);
    if (!af.is_open()) {
        cout << "No feedbacks available.\n";
        return;
    }

    vector<string> lines;
    string line;
    while (getline(af, line)) if (!line.empty()) lines.push_back(line);
    af.close();

    if (lines.empty()) {
        cout << "No feedbacks.\n";
        return;
    }

    cout << "\n===== Customer Feedbacks =====\n";
    for (size_t i = 0; i < lines.size(); ++i) {
        stringstream ss(lines[i]);
        string ts, id, email, name, balanceStr, job, msgEsc;
        getline(ss, ts, '|');
        getline(ss, id, '|');
        getline(ss, email, '|');
        getline(ss, name, '|');
        getline(ss, balanceStr, '|');
        getline(ss, job, '|');
        getline(ss, msgEsc);

        string msg = unescapeFromFile(msgEsc);

        cout << "\n[" << (i + 1) << "] " << ts << "\n";
        cout << " From: " << name << " (ID: " << id << ", Email: " << email << ")\n";
        cout << " Job: " << job << " | Balance: " << balanceStr << " EGP\n";
        cout << " Message:\n" << msg << "\n";
        cout << "----------------------------------------\n";
    }

    cout << "\nDo you want to reply to or delete any message? (r = reply, d = delete, x = exit): ";
    char action; cin >> action;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    if (action == 'x' || action == 'X') return;

    if (action == 'd' || action == 'D') {
        cout << "Enter message number to delete: ";
        int num; if (!(cin >> num) || num < 1 || num >(int)lines.size()) { cout << "Invalid number.\n"; return; }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        lines.erase(lines.begin() + (num - 1));
        ofstream outf(ADMIN_FEEDBACK_FILE, ios::trunc);
        for (auto& L : lines) outf << L << "\n";
        outf.close();
        cout << "Message deleted.\n";
        return;
    }

    if (action == 'r' || action == 'R') {
        cout << "Enter message number to reply: ";
        int num; if (!(cin >> num) || num < 1 || num >(int)lines.size()) { cout << "Invalid number.\n"; return; }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        stringstream ss(lines[num - 1]);
        string ts, id, email, name, balanceStr, job, msgEsc;
        getline(ss, ts, '|');
        getline(ss, id, '|');
        getline(ss, email, '|');
        getline(ss, name, '|');
        getline(ss, balanceStr, '|');
        getline(ss, job, '|');
        getline(ss, msgEsc);
        cout << "Type your reply message. Finish with a single dot (.) on a new line:\n";
        string ln, reply;
        while (true) {
            if (!getline(cin, ln)) break;
            if (ln == ".") break;
            reply += ln; reply.push_back('\n');
        }
        reply = trim(reply);
        if (reply.empty()) { cout << "Reply empty. Cancelled.\n"; return; }

        int accIndex = -1;
        for (int i = 0; i < AccountCount; ++i) {
            if (users[i].ID == id) { accIndex = i; break; }
        }

        if (accIndex == -1) {
            cout << "User not found (may have been deleted). Cannot send reply.\n";
            return;
        }

        Notify(accIndex, string("Admin reply: ") + reply);
        cout << "Reply sent to user " << users[accIndex].Name << " (ID: " << users[accIndex].ID << ").\n";

        cout << "Delete the original feedback from admin list? (y/n): ";
        char delc; cin >> delc; cin.ignore(numeric_limits<streamsize>::max(), '\n');
        if (delc == 'y' || delc == 'Y') {
            lines.erase(lines.begin() + (num - 1));
            ofstream outf(ADMIN_FEEDBACK_FILE, ios::trunc);
            for (auto& L : lines) outf << L << "\n";
            outf.close();
            cout << "Original feedback deleted.\n";
        }
        else {
            AdminMenu();
        }
    }
}
//-----------------------------------------
void PredictBalance(int accountIndex)
{
    int totalTransactions = users[accountIndex].money.transfercount + users[accountIndex].money.withdrawcount;

    if (totalTransactions == 0) {
        cout << "No transactions found. Unable to calculate prediction." << endl;
        return;
    }

    double averageExpenses = (users[accountIndex].money.transfer + users[accountIndex].money.withdraw) / totalTransactions;

    double months = users[accountIndex].Balance / averageExpenses;

    cout << "Hello, Your Balance Is " << users[accountIndex].Balance << " EGP." << endl;
    cout << "Your Average Expenses: " << averageExpenses << " EGP/month." << endl;
    cout << "Based On Your Average Spending Per Month, Your Balance Is Expected To Last For Around "
        << months << " Months." << endl;
}
//-----------------------------------------
void ApplyWeeklyInterest(int accountIndex)
{
    const float savinginterest = 0.10;
    const float vipinterest = 0.15;

    float savingWeeklyRate = savinginterest / 52.0;
    float vipWeeklyRate = vipinterest / 52.0;

    time_t now = time(0);
    double secondsPassed = difftime(now, users[accountIndex].lastInterestTime);

    int weeksPassed = secondsPassed / (7 * 24 * 60 * 60);

    users[accountIndex].lastInterestAdded = 0.0f;

    if (weeksPassed >= 1 && !users[accountIndex].isFrozen) {
        float interestAdded = 0.0f;

        if (users[accountIndex].type_of_account == "Savings Account") {
            interestAdded = users[accountIndex].Balance * savingWeeklyRate * weeksPassed;
            users[accountIndex].Balance += interestAdded;
        }
        else if (users[accountIndex].type_of_account == "VIP Account") {
            interestAdded = users[accountIndex].Balance * vipWeeklyRate * weeksPassed;
            users[accountIndex].Balance += interestAdded;
        }

        users[accountIndex].lastInterestAdded = interestAdded;
        users[accountIndex].lastInterestTime += weeksPassed * (7 * 24 * 60 * 60);
    }
}
//-----------------------------------------
void ApplyWeeklyInterestForAll() {
    for (int i = 0; i < AccountCount; i++) {
        ApplyWeeklyInterest(i);
    }
    SaveAccountsFromFile();
}
//-----------------------------------------
void ShowLastInterest(int accountIndex) {
    if (users[accountIndex].lastInterestAdded > 0) {
        cout << "The last interest added to your account: "
            << users[accountIndex].lastInterestAdded << " EGP" << endl;
    }
    else {
        cout << "No interest has been added to your account recently." << endl;
    }
}
//-----------------------------------------
void SavingsGoals(int accountIndex)
{
    char confirm;
    cout << "Hello Mr. " << users[accountIndex].Name << endl;
    cout << "\nTo view savings goals, a service fee of 7 EGP will be charged. Do you agree? (y/n): ";
    cin >> confirm;

    if (confirm == 'y' || confirm == 'Y') {
        if (users[accountIndex].Balance >= 7) {
            users[accountIndex].Balance -= 7;
            cout << "\n7 EGP service fee has been deducted.\n";
            cout << "Your balance now is " << users[accountIndex].Balance << " EGP\n";

            int goal;
            int attempts = 0;
            do {
                if (attempts > 0) {
                    cout << "It must be more than your balance (" << attempts << "/3)" << endl;
                }
                cout << "What's your goal amount (must be more than your balance): ";
                cin >> goal;
                attempts++;
                if (attempts == 3 && goal < users[accountIndex].Balance) {
                    cout << "\nYou exceeded the maximum attempts!" << endl;
                    cout << "-----------------------------------\n";
                    if (users[accountIndex].type_of_account == "Student Account" || users[accountIndex].type_of_account == "Current Account") {
                        AccountMenu_Student_Current(accountIndex);
                    }
                    else if (users[accountIndex].type_of_account == "Savings Account") {
                        AccountMenu_Savings(accountIndex);
                    }
                    else if (users[accountIndex].type_of_account == "VIP Account") {
                        AccountMenu_VIP(accountIndex);
                    }
                    else if (users[accountIndex].type_of_account == "Pension Account") {
                        AccountMenu_Pension(accountIndex);
                    }
                    return;
                }
            } while (goal < users[accountIndex].Balance);

            int month;
            cout << "\nFor how many months: ";
            cin >> month;

            if (month <= 0) {
                cout << "Invalid number of months!" << endl;
                return;
            }

            double goalPerMonth = static_cast<double>(goal) / month;
            double goalPerWeek = static_cast<double>(goal) / (month * 4);

            cout << "\nTo reach your saving goal of " << goal << " EGP:" << endl;
            cout << "- You need to save " << goalPerMonth << " EGP each month for " << month << " months." << endl;
            cout << "- You need to save " << goalPerWeek << " EGP each week for " << (month * 4) << " weeks." << endl;
            cout << "Keep going! Every small step brings you closer to your goal." << endl;
        }
        else {
            cout << "\nInsufficient balance to pay the service fee!\n";
            cout << "----------------------------------------\n";
            if (users[accountIndex].type_of_account == "Student Account" || users[accountIndex].type_of_account == "Current Account") {
                AccountMenu_Student_Current(accountIndex);
            }
            else if (users[accountIndex].type_of_account == "Savings Account") {
                AccountMenu_Savings(accountIndex);
            }
            else if (users[accountIndex].type_of_account == "VIP Account") {
                AccountMenu_VIP(accountIndex);
            }
            else if (users[accountIndex].type_of_account == "Pension Account") {
                AccountMenu_Pension(accountIndex);
            }
        }
    }
    SaveAccountsFromFile();
}
//-----------------------------------------
void PayAnInstallment(int accountIndex)
{
    if (!users[accountIndex].haram.hasloan) {
        cout << "You don't have an active loan.\n";
        if (users[accountIndex].type_of_account == "Student Account" || users[accountIndex].type_of_account == "Current Account") {
            AccountMenu_Student_Current(accountIndex);
        }
        else if (users[accountIndex].type_of_account == "Savings Account") {
            AccountMenu_Savings(accountIndex);
        }
        else if (users[accountIndex].type_of_account == "VIP Account") {
            AccountMenu_VIP(accountIndex);
        }
        else if (users[accountIndex].type_of_account == "Pension Account") {
            AccountMenu_Pension(accountIndex);
        }
        return;
    }

    if (users[accountIndex].haram.installments <= 0) {
        cout << "No remaining installments!\n";
        users[accountIndex].haram.hasloan = false;
        if (users[accountIndex].type_of_account == "Student Account" || users[accountIndex].type_of_account == "Current Account") {
            AccountMenu_Student_Current(accountIndex);
        }
        else if (users[accountIndex].type_of_account == "Savings Account") {
            AccountMenu_Savings(accountIndex);
        }
        else if (users[accountIndex].type_of_account == "VIP Account") {
            AccountMenu_VIP(accountIndex);
        }
        else if (users[accountIndex].type_of_account == "Pension Account") {
            AccountMenu_Pension(accountIndex);
        }
        return;
    }

    if (users[accountIndex].Balance >= users[accountIndex].haram.installmentvalue) {
        users[accountIndex].Balance -= users[accountIndex].haram.installmentvalue;
        users[accountIndex].haram.installments--;
        users[accountIndex].haram.totaltopay -= users[accountIndex].haram.installmentvalue;
        users[accountIndex].haram.paidinstallments++;
        users[accountIndex].haram.paidamount += users[accountIndex].haram.installmentvalue;

        if (users[accountIndex].haram.installments == 0) {
            users[accountIndex].haram.hasloan = false;
            cout << "Loan fully paid!\n";
        }
        else {
            cout << "Installment has been paid successfully.\n";
        }
    }
    else {
        cout << "Denied. You don't have enough money!\n";
    }

    cout << "-------------------------------------\n";
    SaveAccountsFromFile();
    if (users[accountIndex].type_of_account == "Student Account" || users[accountIndex].type_of_account == "Current Account") {
        AccountMenu_Student_Current(accountIndex);
    }
    else if (users[accountIndex].type_of_account == "Savings Account") {
        AccountMenu_Savings(accountIndex);
    }
    else if (users[accountIndex].type_of_account == "VIP Account") {
        AccountMenu_VIP(accountIndex);
    }
    else if (users[accountIndex].type_of_account == "Pension Account") {
        AccountMenu_Pension(accountIndex);
    }
}
//-----------------------------------------
void CurrentLoanDetails(int accountIndex)
{
    if (!users[accountIndex].haram.hasloan) {
        cout << "You don't have an active loan.\n";
        if (users[accountIndex].type_of_account == "Student Account" || users[accountIndex].type_of_account == "Current Account") {
            AccountMenu_Student_Current(accountIndex);
        }
        else if (users[accountIndex].type_of_account == "Savings Account") {
            AccountMenu_Savings(accountIndex);
        }
        else if (users[accountIndex].type_of_account == "VIP Account") {
            AccountMenu_VIP(accountIndex);
        }
        else if (users[accountIndex].type_of_account == "Pension Account") {
            AccountMenu_Pension(accountIndex);
        }
        return;
    }

    if (users[accountIndex].haram.installments == 0) {
        users[accountIndex].haram.hasloan = false;
        cout << "Loan fully paid!\n";
        cout << "-------------------------------------\n";
        if (users[accountIndex].type_of_account == "Student Account" || users[accountIndex].type_of_account == "Current Account") {
            AccountMenu_Student_Current(accountIndex);
        }
        else if (users[accountIndex].type_of_account == "Savings Account") {
            AccountMenu_Savings(accountIndex);
        }
        else if (users[accountIndex].type_of_account == "VIP Account") {
            AccountMenu_VIP(accountIndex);
        }
        else if (users[accountIndex].type_of_account == "Pension Account") {
            AccountMenu_Pension(accountIndex);
        }
        return;
    }

    cout << "------- Current Loan Details -------\n";
    cout << "   Loan Amount: " << users[accountIndex].haram.loanamount << " EGP\n";
    cout << "   Total Debt (with interest): " << users[accountIndex].haram.totaltopay << " EGP\n";
    cout << "   Monthly Installment: " << users[accountIndex].haram.installmentvalue << " EGP\n";
    cout << "   Remaining Installments: " << users[accountIndex].haram.installments << "\n";
    cout << "------------------------------------\n";

    if (users[accountIndex].type_of_account == "Student Account" || users[accountIndex].type_of_account == "Current Account") {
        AccountMenu_Student_Current(accountIndex);
    }
    else if (users[accountIndex].type_of_account == "Savings Account") {
        AccountMenu_Savings(accountIndex);
    }
    else if (users[accountIndex].type_of_account == "VIP Account") {
        AccountMenu_VIP(accountIndex);
    }
    else if (users[accountIndex].type_of_account == "Pension Account") {
        AccountMenu_Pension(accountIndex);
    }
}
//-----------------------------------------
void NewLoan(int accountIndex)
{
    if (users[accountIndex].haram.hasloan) {
        cout << "You already have an active loan. Cannot apply for another one.\n";
        if (users[accountIndex].type_of_account == "Student Account" || users[accountIndex].type_of_account == "Current Account") {
            AccountMenu_Student_Current(accountIndex);
        }
        else if (users[accountIndex].type_of_account == "Savings Account") {
            AccountMenu_Savings(accountIndex);
        }
        else if (users[accountIndex].type_of_account == "VIP Account") {
            AccountMenu_VIP(accountIndex);
        }
        else if (users[accountIndex].type_of_account == "Pension Account") {
            AccountMenu_Pension(accountIndex);
        }
        return;
    }

    float interestRate = 0.0;
    if (users[accountIndex].type_of_account == "Savings Account") {
        interestRate = 0.15;
        cout << "\nLoans with 15% interest!\n";
    }
    else if (users[accountIndex].type_of_account == "VIP Account") {
        interestRate = 0.12;
        cout << "\nLoans with 12% interest!\n";
    }

    float loanamount;
    int period;

    cout << "Enter loan amount: ";
    cin >> loanamount;

    cout << "Enter the repayment period in months (maximum is 60 months): ";
    cin >> period;

    while (true) {
        if (period > 60 || period == 0) {
            cout << "Maximum is 60 months & cannot be zero: ";
            cin >> period;
            continue;
        }

        float monthlyInstallment = (loanamount + loanamount * interestRate) / period;

        if (users[accountIndex].Balance < monthlyInstallment) {
            cout << "Enter loan amount again. Your balance must contain at least the first payment.\n";
            cin >> loanamount;
            monthlyInstallment = (loanamount + loanamount * interestRate) / period;
            continue;
        }

        if (monthlyInstallment >= users[accountIndex].Salary * 0.5) {
            cout << "Loan denied. Your monthly installment must not exceed half of your salary.\n";
            cout << "Suggestion: Based on your salary, you can take " << 0.4 * users[accountIndex].Salary * period << " EGP.\n";
            cout << "You will repay "
                << 0.4 * users[accountIndex].Salary * period + (0.4 * users[accountIndex].Salary * period) * interestRate
                << " EGP in total, and "
                << (0.4 * users[accountIndex].Salary * period + (0.4 * users[accountIndex].Salary * period) * interestRate) / period
                << " EGP per month for " << period << " months.\n";
            cout << "Enter loan amount: ";
            cin >> loanamount;
            continue;
        }

        break;
    }

    users[accountIndex].haram.hasloan = true;
    users[accountIndex].haram.totaltopay = loanamount + loanamount * interestRate;
    users[accountIndex].haram.installments = period;
    users[accountIndex].haram.installmentvalue = (loanamount + loanamount * interestRate) / period;
    users[accountIndex].haram.loanamount = loanamount;
    users[accountIndex].Balance += loanamount;

    cout << "----------------------------------------------------------------\n";
    cout << "---------------------- Loan approved! --------------------------\n";
    cout << "----------------------------------------------------------------\n";
    cout << "   Amount added to balance : " << loanamount << " EGP\n";
    cout << "       Your balance now is : " << users[accountIndex].Balance << " EGP\n";
    cout << "                Total debt : " << users[accountIndex].haram.totaltopay << " EGP\n";
    cout << "            You will repay : " << users[accountIndex].haram.installmentvalue
        << " EGP per month for " << period << " months.\n";
    cout << "----------------------------------------------------------------\n";
    cout << "--------- Thank you for choosing SABO eBank. Good luck! --------\n";
    cout << "----------------------------------------------------------------\n";

    SaveAccountsFromFile();
    if (users[accountIndex].type_of_account == "Student Account" || users[accountIndex].type_of_account == "Current Account") {
        AccountMenu_Student_Current(accountIndex);
    }
    else if (users[accountIndex].type_of_account == "Savings Account") {
        AccountMenu_Savings(accountIndex);
    }
    else if (users[accountIndex].type_of_account == "VIP Account") {
        AccountMenu_VIP(accountIndex);
    }
    else if (users[accountIndex].type_of_account == "Pension Account") {
        AccountMenu_Pension(accountIndex);
    }
}
//-----------------------------------------
void LoanMenu(int accountIndex)
{
    cout << "\n===== Loan System =====\n";
    if (users[accountIndex].type_of_account == "Savings Account") {
        cout << "Loans with 15% interest!\n";
    }
    else if (users[accountIndex].type_of_account == "VIP Account") {
        cout << "Loans with 12% interest!\n";
    }

    cout << "1. Apply for a new loan\n";
    cout << "2. View current loan details\n";
    cout << "3. Pay an installment\n";
    cout << "4. Back to main menu\n";
    cout << "Enter choice: ";

    int choice;
    cin >> choice;

    switch (choice) {
    case 1:
        NewLoan(accountIndex);
        break;
    case 2:
        CurrentLoanDetails(accountIndex);
        break;
    case 3:
        PayAnInstallment(accountIndex);
        break;
    case 4:
        if (users[accountIndex].type_of_account == "Student Account" || users[accountIndex].type_of_account == "Current Account") {
            AccountMenu_Student_Current(accountIndex);
        }
        else if (users[accountIndex].type_of_account == "Savings Account") {
            AccountMenu_Savings(accountIndex);
        }
        else if (users[accountIndex].type_of_account == "VIP Account") {
            AccountMenu_VIP(accountIndex);
        }
        else if (users[accountIndex].type_of_account == "Pension Account") {
            AccountMenu_Pension(accountIndex);
        }
        break;
    default:
        cout << "Invalid input!\n";
        break;
    }
}
//-----------------------------------------
bool checkFileExists(const string& filename) {
    ifstream file(filename);
    return file.good();
}
//-----------------------------------------


int main() {
    LoadAccountsFromFile();

    for (int i = 0; i < AccountCount; i++) {
        createMessageFileIfNotExists(i);
        createTransactionFileIfNotExists(i);
    }

    int loggedInUserIndex = -1;

    while (true) {
        Welcome();
        int choice;
        cin >> choice;

        switch (choice) {
        case 1:
            CreateANewAccount();
            SaveAccountsFromFile();
            break;

        case 2:
            loggedInUserIndex = Login();
            if (loggedInUserIndex >= 0) {
                while (true) {
                    string type = trim(users[loggedInUserIndex].type_of_account);

                    if (type == "Student Account" || type == "Current Account") {
                        AccountMenu_Student_Current(loggedInUserIndex);
                    }
                    else if (type == "Savings Account") {
                        AccountMenu_Savings(loggedInUserIndex);
                    }
                    else if (type == "VIP Account") {
                        AccountMenu_VIP(loggedInUserIndex);
                    }
                    else if (type == "Pension Account") {
                        AccountMenu_Pension(loggedInUserIndex);
                    }
                    else {
                        cout << "Error: Unknown account type! -> '" << type << "'\n";
                    }

                    SaveAccountsFromFile();

                    cout << "\n\n1. Back to Account Menu\n";
                    cout << "2. Logout and return to Main Menu\n";
                    cout << "Enter choice: ";
                    int post_choice;
                    cin >> post_choice;

                    if (post_choice == 2) {
                        break;
                    }
                }
            }
            break;

        case 3:
            cout << "Thank you for using SABO eBank. Goodbye!\n";
            SaveAccountsFromFile();
            return 0;

        default:
            cout << "Invalid choice. Please try again.\n";
            break;
        }
    }
    return 0;
}