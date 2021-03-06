#pragma once

#include <ctime>
#include <optional>
#include <set>
#include <string>
#include <utility>
#include <vector>

class Transaction;

class Account {
public:
    uint32_t getId() const {
        return id;
    }

    Account(uint32_t id, uint32_t clientId, const std::string& bankSwift, int64_t balance, int64_t currentUsage,
            int64_t limit, std::string type)
        : id(id), client_id(clientId), bank_swift(bankSwift), balance(balance), currentUsage(currentUsage),
          limit(limit), type(std::move(type)) {}

    virtual ~Account() = default;

    uint32_t getClientId() const {
        return client_id;
    }

    std::string getBankSwift() const {
        return bank_swift;
    }

    int64_t getBalance() const {
        return balance;
    }

    void setBalance(int64_t _balance) {
        Account::balance = _balance;
    }

    void addToAccount(int64_t value);

    bool topUpAccount(int64_t value);

    virtual int getCash(int64_t value);

    virtual bool transferMoneyTo(Account& other, int64_t value);

    virtual std::optional<std::string /* error msg */> checkRulesFromAccount(const Transaction& tr) = 0;
    virtual std::optional<std::string /* error msg */> checkRulesToAccount(const Transaction& tr) = 0;

private:
    uint32_t id;
    uint32_t client_id;
    std::string bank_swift;
    int64_t balance;
    int64_t currentUsage = 0;
    int64_t limit;
    std::string type;

public:
    const std::string& getType() const {
        return type;
    }

    int64_t getCurrentUsage() const;

    void setCurrentUsage(int64_t currentUsage);

    int64_t getLimit() const;

    void setLimit(int64_t limit);
};

class Deposit : public Account {
public:
    Deposit(uint32_t id, uint32_t clientId, const std::string& bankSwift, int64_t balance, int64_t currentUsage,
            int64_t limit, float percentPerYear, time_t expiryDate)
        : Account(id, clientId, bankSwift, balance, currentUsage, limit, "Депозит"), percent_per_year(percentPerYear),
          expiry_date(expiryDate) {}

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

    int getCash(int64_t value) override;

    bool transferMoneyTo(Account& other, int64_t value) override;

    std::optional<std::string /* error msg */> checkRulesFromAccount(const Transaction& tr) override;
    std::optional<std::string /* error msg */> checkRulesToAccount(const Transaction& tr) override;

private:
    float percent_per_year;
    time_t expiry_date;
};

class Debit : public Account {
public:
    Debit(uint32_t id, uint32_t clientId, const std::string& bankSwift, int64_t balance, int64_t currentUsage,
          int64_t limit, float percent)
        : Account(id, clientId, bankSwift, balance, currentUsage, limit, "Дебетовый счет"), percent(percent) {}

    float getPercent() const {
        return percent;
    }

    void setPercent(float _percent) {
        Debit::percent = _percent;
    }

    int getCash(int64_t value) override;

    bool transferMoneyTo(Account& other, int64_t value) override;

    std::optional<std::string /* error msg */> checkRulesFromAccount(const Transaction& tr) override;
    std::optional<std::string /* error msg */> checkRulesToAccount(const Transaction& tr) override;

private:
    float percent;
};

class Credit : public Account {
public:
    float getCommission() const {
        return commission;
    }
    void setCommission(float _commission) {
        Credit::commission = _commission;
    }

    int getCash(int64_t value) override;
    bool transferMoneyTo(Account& other, int64_t value) override;

private:
    float commission;
    int64_t creditLimit{};

public:
    Credit(uint32_t id, uint32_t clientId, const std::string& bankSwift, int64_t balance, int64_t currentUsage,
           int64_t limit, float commission, int64_t creditLimit)
        : Account(id, clientId, bankSwift, balance, currentUsage, limit, "Кредитный счет"), commission(commission),
          creditLimit(creditLimit) {}

    int64_t getCreditLimit() const {
        return creditLimit;
    }

    void setCreditLimit(int64_t _creditLimit) {
        Credit::creditLimit = _creditLimit;
    }

    std::optional<std::string /* error msg */> checkRulesFromAccount(const Transaction& tr) override;
    std::optional<std::string /* error msg */> checkRulesToAccount(const Transaction& tr) override;
};
