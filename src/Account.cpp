#include "Account.h"

void Account::addToAccount(int64_t value) {
    this->balance += value;
}

bool Account::topUpAccount(int64_t value) {
    if (value <= 0) {
        return false;
    }
    this->addToAccount(value);
    return true;
}

bool Account::transferMoneyTo(Account& other, int64_t value) {
    return false;
}

int Account::getCash(int64_t value) {
    return 0;
}

int64_t Account::getCurrentUsage() const {
    return currentUsage;
}

void Account::setCurrentUsage(int64_t usage) {
    currentUsage = usage;
}

int64_t Account::getLimit() const {
    return limit;
}

void Account::setLimit(int64_t lim) {
    limit = lim;
}

bool Deposit::checkRules() {
    printf("Нельзя снимать и переводить деньги в депозите\n");
    return false;
}

int Deposit::getCash(int64_t value) {
    return this->checkRules();
}

bool Deposit::transferMoneyTo(Account& other, int64_t value) {
    return this->checkRules();
}

bool Debit::checkRules() {
    if (this->getLimit() == -1) {
        return true;
    }
    if (this->getLimit() <= this->getCurrentUsage()) {
        printf("Превышен лимит\n");
        printf("Ваш лимит %ld\n", this->getLimit());
        printf("Вы использовали %ld\n", this->getCurrentUsage());
        printf("(чтобы увеличить лимит дополните информацию о вас)\n");
        return false;
    }
    return true;
}

int Debit::getCash(int64_t value) {
    if (!this->checkRules()) {
        return 0;
    }
    if (value <= 0) {
        return 0;
    }
    if (value > this->getBalance()) {
        printf("Недостаточно средств\n");
        return 0;
    }
    if (this->getLimit() != -1) {
        int64_t diff = this->getLimit() - this->getCurrentUsage();
        if (value > diff) {
            printf("Лимит будет превышен!\n");
            printf("Ваш лимит %ld\n", this->getLimit());
            printf("Вы использовали %ld\n", this->getCurrentUsage());
            printf("(чтобы увеличить лимит дополните информацию о вас)\n");
            return 0;
        }
    }

    this->addToAccount(-value);
    this->setCurrentUsage(this->getCurrentUsage() + value);

    return 1;
}

bool Debit::transferMoneyTo(Account& other, int64_t value) {
    if (value <= 0) {
        return false;
    }
    this->getCash(value);
    other.addToAccount(value);
    return true;
}

bool Credit::checkRules() {
    if (this->getLimit() == -1) {
        return true;
    }
    if (this->getLimit() <= this->getCurrentUsage()) {
        printf("Превышен лимит\n");
        printf("Ваш лимит %ld\n", this->getLimit());
        printf("Вы использовали %ld\n", this->getCurrentUsage());
        printf("(чтобы увеличить лимит дополните информацию о вас)\n");
        return false;
    }
    if (this->getBalance() <= -this->getCreditLimit()) {
        printf("Превышен кредитный лимит\n");
        printf("Ваш баланс %ld\n", this->getBalance());
        printf("Кредитный лимит -%ld\n", this->getCreditLimit());

        return false;
    }
    return true;
}

int Credit::getCash(int64_t value) {
    if (!this->checkRules()) {
        return 0;
    }
    if (value <= 0) {
        return 0;
    }
    if (this->getLimit() != -1) {
        int64_t diff = this->getLimit() - this->getCurrentUsage();
        if (value > diff) {
            printf("Лимит будет превышен!\n");
            printf("Ваш лимит %ld\n", this->getLimit());
            printf("Вы использовали %ld\n", this->getCurrentUsage());
            printf("(чтобы увеличить лимит дополните информацию о вас)\n");

            return 0;
        }
    }
    if (-value + this->getBalance() <= -this->getCreditLimit()) {
        printf("Будет превышен кредитный лимит\n");
        printf("Ваш баланс %ld\n", this->getBalance());
        printf("Кредитный лимит -%ld\n", this->getCreditLimit());
        return 0;
    }
    int commission = 0;
    if (this->getBalance() < 0) {
        commission = int((double) this->getCommission() * (double) value);
    }
    this->addToAccount(-value - commission);
    this->setCurrentUsage(this->getCurrentUsage() + value + commission);

    return 1;
}

bool Credit::transferMoneyTo(Account& other, int64_t value) {
    if (value <= 0) {
        return false;
    }
    this->getCash(value);
    other.addToAccount(value);
    return true;
}
