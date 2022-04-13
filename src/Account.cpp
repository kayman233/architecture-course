#include "Account.h"

void Account::addToAccount(int64_t value) {
    this->balance += value;
}

int Account::transferMoneyTo(Account& other, int64_t value) {
    if (value <= 0) {
        return 0;
    }
    other.addToAccount(value);
    return 1;
}

int Account::getCash(int64_t value) {
    if (value <= 0) {
        return 0;
    }
    this->addToAccount(-value);

    return 1;
}

bool Deposit::checkRules() {
    return true;
}

int Deposit::getCash(int64_t value) {
    return 0;
}

int Deposit::transferMoneyTo(Account &other, int64_t value) {
    return 0;
}

bool Debit::checkRules() {
    return true;
}

bool Credit::checkRules() {
    return true;
}
