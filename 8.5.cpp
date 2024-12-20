#include <iostream>
#include <string>
#include <iomanip> // для std::fixed и std::setprecision
#include <stdexcept> // для std::runtime_error


class BankAccount {
public:
    BankAccount(long long accountNumber, const std::string& ownerName, double balance)
        : accountNumber_(accountNumber), ownerName_(ownerName), balance_(balance) {
        if (balance_ < 0) {
            throw std::runtime_error("Initial balance cannot be negative.");
        }
    }

    void deposit(double amount) {
        if (amount <= 0) {
            throw std::runtime_error("Deposit amount must be positive.");
        }
        balance_ += amount;
    }

    void withdraw(double amount) {
        if (amount <= 0) {
            throw std::runtime_error("Withdrawal amount must be positive.");
        }
        if (balance_ < amount) {
            throw std::runtime_error("Insufficient funds.");
        }
        balance_ -= amount;
    }

    double getBalance() const { return balance_; }

    long long getAccountNumber() const { return accountNumber_; }

    const std::string& getOwnerName() const { return ownerName_; }

    virtual void printInfo() const { // <--- virtual keyword added
        std::cout << "Номер счета: " << accountNumber_ << std::endl;
        std::cout << "Владелец: " << ownerName_ << std::endl;
        std::cout << "Баланс: " << std::fixed << std::setprecision(2) << balance_ << std::endl;
    }

protected:
    long long accountNumber_;
    std::string ownerName_;
    double balance_;
};


class SavingsAccount : public BankAccount {
public:
    SavingsAccount(long long accountNumber, const std::string& ownerName, double balance, double interestRate)
        : BankAccount(accountNumber, ownerName, balance), interestRate_(interestRate) {
        if (interestRate_ < 0) {
            throw std::runtime_error("Interest rate cannot be negative.");
        }
    }

    void accrueInterest() {
        double interest = balance_ * interestRate_;
        deposit(interest);
    }

    void printInfo() const override {
        BankAccount::printInfo();
        std::cout << "Процентная ставка: " << std::fixed << std::setprecision(2) << interestRate_ * 100 << "%" << std::endl;
    }

private:
    double interestRate_;
};


int main() {
    setlocale(LC_ALL, "Russian");
    try {
        BankAccount checkingAccount(1234567890, "Alice", 1000);
        SavingsAccount savingsAccount(9876543210, "Bob", 5000, 0.05);

        checkingAccount.deposit(500);
        checkingAccount.withdraw(200);
        savingsAccount.deposit(1000);
        savingsAccount.accrueInterest();
        savingsAccount.withdraw(500);

        std::cout << "Информация о счете checkingAccount:\n";
        checkingAccount.printInfo();
        std::cout << "\nИнформация о счете savingsAccount:\n";
        savingsAccount.printInfo();

    }
    catch (const std::runtime_error& error) {
        std::cerr << "Ошибка: " << error.what() << std::endl;
    }

    return 0;
}
