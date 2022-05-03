#pragma once

#include <vector>
#include "Transaction.h"

class TransactionStorage {
private:
    std::vector<Transaction> transactions;

public:
    void saveToFile(const std::string& path) const;
    static TransactionStorage readFromFile(const std::string& path);

    void addTransaction(const Transaction& tr);
    void rollbackTransaction(const std::string uuid);
};
