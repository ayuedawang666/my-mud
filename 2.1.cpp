#include <iostream>
#include <iomanip>
using namespace std;

// ���� Account
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

// ������ SavingsAccount
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

// ������ CheckingAccount
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
            Account::debit(amount); // �ָ�ԭʼ���
        }
    }

    bool debit(double amount) override {
        double originalBalance = getBalance();
        bool success = Account::debit(amount);
        if (success) {
            if (!chargeFee()) {
                cout << "Transaction fee exceeded account balance while debiting" << endl;
                Account::credit(amount); // �ָ�ԭʼ���
                return false;
            }
            return true;
        }
        return false;
    }
};

int main() {
    Account* accounts[3];
    accounts[0] = new SavingsAccount(100, 3);    // ���100Ԫ����Ϣ3%
    accounts[1] = new CheckingAccount(100, 5);   // ���100Ԫ�����׷�5Ԫ
    accounts[2] = new CheckingAccount(50, 5);    // ���50Ԫ�����׷�5Ԫ

    for (int i = 0; i < 3; i++) {
        cout << "��" << i + 1 << "��ѭ���Ľ��:" << endl;
        accounts[i]->debit(200);  // ���200Ԫ
        accounts[i]->debit(46);
        accounts[i]->credit(50);  // ���50Ԫ
        accounts[i]->debit(49);
        accounts[i]->debit(43);
        accounts[i]->credit(1);

        // ��Accountָ��ǿ��ת��ΪSavingsAccountָ��
        SavingsAccount* derivedPtr = dynamic_cast<SavingsAccount*>(accounts[i]);
        if (derivedPtr != nullptr) {  // ������ͼ��ݣ�ת���ɹ�
            derivedPtr->credit(derivedPtr->calculateInterest());
        }

        cout << fixed << setprecision(2);
        cout << "�˻������Ϊ: " << accounts[i]->getBalance() << endl << endl;
    }

    // �ͷ��ڴ�
    for (int i = 0; i < 3; i++) {
        delete accounts[i];
    }

    return 0;
}