#include "Account.h"
#include "Transaction.h"

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

int Deposit::getCash(int64_t value) {
    printf("Нельзя снимать и переводить деньги в депозите\n");
    return false;
}

bool Deposit::transferMoneyTo(Account& other, int64_t value) {
    printf("Нельзя снимать и переводить деньги в депозите\n");
    return false;
}

std::optional<std::string /* error msg */> Deposit::checkRulesFromAccount(const Transaction& tr) {
    throw std::runtime_error("not implemented");
}

std::optional<std::string /* error msg */> Deposit::checkRulesToAccount(const Transaction& tr) {
    return {}; // можно пополнять депозит
}

int Debit::getCash(int64_t value) {
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

std::optional<std::string /* error msg */> Debit::checkRulesFromAccount(const Transaction& tr) {
    if (getBalance() < tr.amount) {
        return "Недостаточно средств";
    }
    if (this->getLimit() != -1) {
        int64_t diff = this->getLimit() - (this->getCurrentUsage() + tr.amount);
        if (diff < 0) {
            return "Лимит на перевод со счета будет превышен на $" + std::to_string(-diff);
        }
    }
    return {};
}

std::optional<std::string /* error msg */> Debit::checkRulesToAccount(const Transaction& tr) {
    return {}; // Можно пополнять дебетовый счет
}

int Credit::getCash(int64_t value) {
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

std::optional<std::string /* error msg */> Credit::checkRulesFromAccount(const Transaction& tr) {
    if (this->getLimit() != -1) {
        int64_t diff = this->getLimit() - (this->getCurrentUsage() + tr.amount);
        if (diff < 0) {
            return "Лимит на перевод со счета будет превышен на $" + std::to_string(-diff);
        }
    }
    int64_t creditOverflow = int64_t(tr.amount) - (this->getBalance() + this->getCreditLimit());
    if (creditOverflow > 0) {
        return "Превышение кредитного лимита на $" + std::to_string(creditOverflow);
    }
    return {};
}

std::optional<std::string /* error msg */> Credit::checkRulesToAccount(const Transaction& tr) {
    return {};
}
