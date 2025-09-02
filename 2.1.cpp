#include <iostream>
#include <iomanip>
using namespace std;

// 基类 Account
class Account {
private:
    double balance;
public:
    Account(double initialBalance) {
        balance = (initialBalance >= 0) ? initialBalance : 0;
    }
    virtual ~Account() {}

    virtual void credit(double amount) {
        balance += amount;
    }

    virtual bool debit(double amount) {
        if (amount > balance) {
            cout << "Debit amount exceeded account balance" << endl;
            return false;
        }
        balance -= amount;
        return true;
    }

    double getBalance() const {
        return balance;
    }
};

// 派生类 SavingsAccount
class SavingsAccount : public Account {
private:
    double interestRate;
public:
    SavingsAccount(double initialBalance, double rate)
        : Account(initialBalance) {
        interestRate = (rate >= 0) ? rate : 0;
    }

    double calculateInterest() const {
        return getBalance() * interestRate / 100;
    }
};

// 派生类 CheckingAccount
class CheckingAccount : public Account {
private:
    double transactionFee;

    bool chargeFee() {
        if (getBalance() >= transactionFee) {
            Account::debit(transactionFee);
            return true;
        }
        return false;
    }
public:
    CheckingAccount(double initialBalance, double fee)
        : Account(initialBalance) {
        transactionFee = (fee >= 0) ? fee : 0;
    }

    void credit(double amount) override {
        double originalBalance = getBalance();
        Account::credit(amount);
        if (!chargeFee()) {
            cout << "Transaction fee exceeded account balance while crediting" << endl;
            Account::debit(amount); // 恢复原始余额
        }
    }

    bool debit(double amount) override {
        double originalBalance = getBalance();
        bool success = Account::debit(amount);
        if (success) {
            if (!chargeFee()) {
                cout << "Transaction fee exceeded account balance while debiting" << endl;
                Account::credit(amount); // 恢复原始余额
                return false;
            }
            return true;
        }
        return false;
    }
};

int main() {
    Account* accounts[3];
    accounts[0] = new SavingsAccount(100, 3);    // 余额100元，利息3%
    accounts[1] = new CheckingAccount(100, 5);   // 余额100元，交易费5元
    accounts[2] = new CheckingAccount(50, 5);    // 余额50元，交易费5元

    for (int i = 0; i < 3; i++) {
        cout << "第" << i + 1 << "次循环的结果:" << endl;
        accounts[i]->debit(200);  // 借款200元
        accounts[i]->debit(46);
        accounts[i]->credit(50);  // 存款50元
        accounts[i]->debit(49);
        accounts[i]->debit(43);
        accounts[i]->credit(1);

        // 将Account指针强制转换为SavingsAccount指针
        SavingsAccount* derivedPtr = dynamic_cast<SavingsAccount*>(accounts[i]);
        if (derivedPtr != nullptr) {  // 如果类型兼容，转换成功
            derivedPtr->credit(derivedPtr->calculateInterest());
        }

        cout << fixed << setprecision(2);
        cout << "账户的余额为: " << accounts[i]->getBalance() << endl << endl;
    }

    // 释放内存
    for (int i = 0; i < 3; i++) {
        delete accounts[i];
    }

    return 0;
}