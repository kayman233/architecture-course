#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include "Account.h"
#include "Client.h"

struct BankConfig {
    uint32_t debitPercent = 5;
    // ...
};

class Bank {
    std::string swiftCode_;
    std::string name_;
    BankConfig config_;
    std::vector<std::unique_ptr<Account>> accounts_;
    std::unordered_map<uint32_t, Client> clients_;

public:
    Bank(std::string swiftCode, std::string name, BankConfig config);

    uint32_t registerClient(Client client);
    Client client(uint32_t id) const;

    uint32_t openDebit(uint32_t clientId);
    void openDeposit(uint32_t clientId);
};
