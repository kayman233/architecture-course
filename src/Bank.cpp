#include "Bank.h"
#include <chrono>
#include <ctime>

Bank::Bank(std::string swiftCode, std::string name, BankConfig config)
    : swiftCode_(std::move(swiftCode)), name_(std::move(name)), config_(config) {}

uint32_t Bank::registerClient(Client client) {
    uint32_t id = clients_.size() + 1;
    client.setId(id);
    clients_.emplace(id, std::move(client));
    return id;
}

Client Bank::client(uint32_t id) const { return clients_.at(id); }

uint32_t Bank::openDebit(uint32_t clientId) {
    uint32_t id = accounts_.size() + 1;
    auto debit = std::make_unique<Debit>(id, clientId, 0, 0, config_.debitPercent / 100.0f);
    accounts_.push_back(std::move(debit));
    return id;
}

uint32_t Bank::openDeposit(uint32_t clientId) {
    uint32_t id = accounts_.size() + 1;
    time_t rawtime;
    struct tm* timeinfo;
    time (&rawtime);
    timeinfo = localtime(&rawtime);
    timeinfo->tm_year += 1;
    time_t expire = mktime(timeinfo);
    auto deposit = std::make_unique<Deposit>(id, clientId, 0,
                                           0, config_.depositPercent / 100.0f, expire);
    accounts_.push_back(std::move(deposit));
    return id;
}

uint32_t Bank::openCredit(uint32_t clientId) {
    uint32_t id = accounts_.size() + 1;
    auto credit = std::make_unique<Credit>(id, clientId, 0, 0, config_.commision / 100.0f);
    accounts_.push_back(std::move(credit));
    return id;
}
