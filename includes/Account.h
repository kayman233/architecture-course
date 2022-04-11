#pragma once
#include <vector>
#include <string>
#include <set>
#include <ctime>

class Account {
public:
    uint32_t getId() const {
        return id;
    }

    Account(uint32_t id, uint32_t clientId, uint32_t bankId, int64_t balance) : id(id), client_id(clientId),
                                                                                bank_id(bankId), balance(balance) {}

    uint32_t getClientId() const {
        return client_id;
    }

    uint32_t getBankId() const {
        return bank_id;
    }

    int64_t getBalance() const {
        return balance;
    }

    void setBalance(int64_t _balance) {
        Account::balance = _balance;
    }

    void addToAccount(int64_t value);

    int getCash(int64_t value);

    virtual bool checkRules() = 0;

    int transferMoneyTo(Account& other, int64_t value);

private:
    uint32_t id;
    uint32_t client_id;
    uint32_t bank_id;
    int64_t balance;
};

class Deposit: public Account {
    public:
    Deposit(uint32_t id, uint32_t clientId, uint32_t bankId, int64_t balance, float percentPerYear, time_t expiryDate)
            : Account(id, clientId, bankId, balance), percent_per_year(percentPerYear), expiry_date(expiryDate) {}

    float getPercentPerYear() const {
        return percent_per_year;
    }

    void setPercentPerYear(float percentPerYear) {
        percent_per_year = percentPerYear;
    }

    time_t getExpiryDate() const {
        return expiry_date;
    }

    void setExpiryDate(time_t expiryDate) {
        expiry_date = expiryDate;
    }

    bool checkRules() override;

    int getCash(int64_t value);

    int transferMoneyTo(Account& other, int64_t value);


private:
    float percent_per_year;
    time_t expiry_date;
};

class Debit: public Account {
public:
    Debit(uint32_t id, uint32_t clientId, uint32_t bankId, int64_t balance, float percent) : Account(id, clientId,
                                                                                                     bankId, balance),
                                                                                             percent(percent) {}

    float getPercent() const {
        return percent;
    }

    void setPercent(float _percent) {
        Debit::percent = _percent;
    }

    bool checkRules() override;


private:
    float percent;
};

class Credit: public Account {
public:
    float getCommision() const {
        return commision;
    }

    Credit(uint32_t id, uint32_t clientId, uint32_t bankId, int64_t balance, float commision) : Account(id, clientId,
                                                                                                        bankId,
                                                                                                        balance),
                                                                                                commision(commision) {}

    void setCommision(float _commision) {
        Credit::commision = _commision;
    }

    bool checkRules() override;


private:
    float commision;
    int64_t limit{};
};
