#include "Bank.h"
#include <chrono>
#include <ctime>
#include <algorithm>
#include <stdexcept>

Bank::Bank(std::string swiftCode, std::string name, BankConfig config)
    : swiftCode_(std::move(swiftCode)), name_(std::move(name)), config_(config) {}

uint32_t Bank::registerClient(Client client) {
    uint32_t id = clients_.size() + 1;
    client.setId(id);
    clients_.emplace(id, std::move(client));
    return id;
}

Client Bank::client(uint32_t id) const {
    return clients_.at(id);
}

uint32_t Bank::openDebit(uint32_t clientId) {
    uint32_t id = accounts_.size() + 1;
    auto debit = std::make_unique<Debit>(id, clientId, swiftCode_, 0, 0, config_.maxSumForSusClient,
                                         config_.debitPercent / 100.0f);
    accounts_.push_back(std::move(debit));
    return id;
}

uint32_t Bank::openDeposit(uint32_t clientId) {
    uint32_t id = accounts_.size() + 1;
    time_t rawtime;
    struct tm* timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    timeinfo->tm_year += 1;
    time_t expire = mktime(timeinfo);
    auto deposit = std::make_unique<Deposit>(id, clientId, this->swiftCode_, 0, 0, config_.maxSumForSusClient,
                                             config_.depositPercent / 100.0f, expire);
    accounts_.push_back(std::move(deposit));
    return id;
}

uint32_t Bank::openCredit(uint32_t clientId) {
    uint32_t id = accounts_.size() + 1;
    auto credit = std::make_unique<Credit>(id, clientId, this->swiftCode_, 0, 0, config_.maxSumForSusClient,
                                           config_.commission / 100.0f, config_.creditLimitMinus);
    accounts_.push_back(std::move(credit));
    return id;
}

const std::vector<std::unique_ptr<Account>>& Bank::getAccounts() const {
    return accounts_;
}

void Bank::recheckClient(uint32_t clientId) {
    this->client(clientId).setIsSus(false);
    for (const auto& account: accounts_) {
        if (account->getClientId() == clientId) {
            account->setLimit(-1);
        }
    }
}

Account* Bank::findAccountById(uint32_t id) {
    auto it = std::find_if(accounts_.begin(), accounts_.end(), [id](const auto& acc) { return acc->getId() == id; });
    if (it == accounts_.end()) {
        return nullptr;
    }
    return it->get();
}

std::optional<std::string /* error msg */> Bank::addTransaction(Bank& toBank, const Transaction& tr) {
    if (toBank.swiftCode_ != tr.receiverBankSwift) {
        throw std::runtime_error("Bad toBank <" + toBank.swiftCode_ + "> != Transaction receiver " +
                                 tr.receiverBankSwift);
    }

    auto fromAccount = findAccountById(tr.senderAccountId);
    if (!fromAccount) {
        return "В банке-отправителе нет счета с номером " + std::to_string(tr.senderAccountId);
    }

    auto error = fromAccount->checkRulesFromAccount(tr);
    if (error.has_value()) {
        return "Нарушаются правила счета, с которого выводятся деньги. " + *error;
    }

    auto toAccount = toBank.findAccountById(tr.receiverAccountId);
    if (!toAccount) {
        return "В банке-принимателе нет счета с номером " + std::to_string(tr.receiverAccountId);
    }

    error = toAccount->checkRulesToAccount(tr);
    if (error.has_value()) {
        return "Нарушаются правила счета, на который зачисляются деньги. " + *error;
    }

    transactions_.push_back(tr);
    fromAccount->addToAccount(-int64_t(tr.amount));
    fromAccount->setCurrentUsage(fromAccount->getCurrentUsage() + tr.amount);
    toAccount->addToAccount(int64_t(tr.amount));
    return {};
}
