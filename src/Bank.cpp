#include "Bank.h"

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
