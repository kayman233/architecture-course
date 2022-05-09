#include "Account.h"

void Account::addToAccount(int64_t value) {
    this->balance += value;
}

int Account::topUpAccount(int64_t value) {
    if (value <= 0) {
        return 0;
    }
    this->addToAccount(value);
    return 1;
}

int Account::transferMoneyTo(Account& other, int64_t value) {
    return 0;
}

int Account::getCash(int64_t value) {
    return 0;
}

int64_t Account::getCurrentUsage() const {
    return currentUsage;
}

void Account::setCurrentUsage(int64_t currentUsage) {
    Account::currentUsage = currentUsage;
}

int64_t Account::getLimit() const {
    return limit;
}

void Account::setLimit(int64_t limit) {
    Account::limit = limit;
}

bool Deposit::checkRules() {
    printf("Нельзя снимать и переводить деньги в депозите\n");
    return false;
}

int Deposit::getCash(int64_t value) {
    return this->checkRules();
}

int Deposit::transferMoneyTo(Account &other, int64_t value) {
    return this->checkRules();
}

bool Debit::checkRules() {
    if (this->getLimit() == -1) {
        return true;
    }
    if (this->getLimit() <= this->getCurrentUsage()) {
        printf("Превышен лимит\n");
        printf("Ваш лимит %lld\n", this->getLimit());
        printf("Вы использовали %lld\n", this->getCurrentUsage());
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
        int diff = this->getLimit() - this->getCurrentUsage();
        if (value > diff) {
            printf("Лимит будет превышен!\n");
            printf("Ваш лимит %lld\n", this->getLimit());
            printf("Вы использовали %lld\n", this->getCurrentUsage());
            printf("(чтобы увеличить лимит дополните информацию о вас)\n");
            return 0;
        }
    }

    this->addToAccount(-value);
    this->setCurrentUsage(this->getCurrentUsage() + value);

    return 1;
}

int Debit::transferMoneyTo(Account &other, int64_t value) {
    if (value <= 0) {
        return 0;
    }
    this->getCash(value);
    other.addToAccount(value);
}

bool Credit::checkRules() {
    if (this->getLimit() == -1) {
        return true;
    }
    if (this->getLimit() <= this->getCurrentUsage()) {
        printf("Превышен лимит\n");
        printf("Ваш лимит %lld\n", this->getLimit());
        printf("Вы использовали %lld\n", this->getCurrentUsage());
        printf("(чтобы увеличить лимит дополните информацию о вас)\n");
        return false;
    }
    if (this->getBalance() <= -this->getCreditLimit()) {
        printf("Превышен кредитный лимит\n");
        printf("Ваш баланс %lld\n", this->getBalance());
        printf("Кредитный лимит -%lld\n", this->getCreditLimit());

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
        int diff = this->getLimit() - this->getCurrentUsage();
        if (value > diff) {
            printf("Лимит будет превышен!\n");
            printf("Ваш лимит %lld\n", this->getLimit());
            printf("Вы использовали %lld\n", this->getCurrentUsage());
            printf("(чтобы увеличить лимит дополните информацию о вас)\n");

            return 0;
        }
    }
    if (-value + this->getBalance() <= -this->getCreditLimit()) {
        printf("Будет превышен кредитный лимит\n");
        printf("Ваш баланс %lld\n", this->getBalance());
        printf("Кредитный лимит -%lld\n", this->getCreditLimit());
        return 0;
    }
    int commission = 0;
    if (this->getBalance() < 0) {
        commission = int((float)this->getCommision() * value);
    }
    this->addToAccount(-value - commission);
    this->setCurrentUsage(this->getCurrentUsage() + value + commission);

    return 1;
}

int Credit::transferMoneyTo(Account &other, int64_t value) {
    if (value <= 0) {
        return 0;
    }
    this->getCash(value);
    other.addToAccount(value);
}
