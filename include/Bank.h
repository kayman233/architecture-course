#pragma once

#include <memory>
#include <optional>
#include <string>
#include <unordered_map>

#include "Account.h"
#include "Client.h"
#include "Transaction.h"

struct BankConfig {
    uint32_t debitPercent = 5;
    uint32_t depositPercent = 1;
    uint32_t maxSumForSusClient = 1000; // максимальная сумма для сомнительных клиентов
    uint32_t commission = 1;
    uint32_t expiryYears = 4;
    uint32_t creditLimitMinus = 5000;
};

class Bank {
    std::string swiftCode_;
    std::string name_;
    BankConfig config_;
    std::vector<std::unique_ptr<Account>> accounts_;
    std::unordered_map<uint32_t, Client> clients_;
    std::vector<Transaction> transactions_;

public:
    Bank(std::string swiftCode, std::string name, BankConfig config);

    uint32_t registerClient(Client client);
    Client client(uint32_t id) const;

    uint32_t openDebit(uint32_t clientId);
    uint32_t openDeposit(uint32_t clientId);
    uint32_t openCredit(uint32_t clientId);

    void recheckClient(uint32_t clientId);

    const std::string& getSwiftCode() const {
        return swiftCode_;
    }

    const std::string& getName() const {
        return name_;
    }

    const BankConfig& getConfig() const {
        return config_;
    }

    const std::vector<std::unique_ptr<Account>>& getAccounts() const;
    Account* findAccountById(uint32_t id);

    std::optional<std::string /* error msg */> addTransaction(Bank& toBank, const Transaction& tr);
    void rollbackTransaction(const std::string& uuid);
};
