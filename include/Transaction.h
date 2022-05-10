#pragma once

#include <ctime>
#include <iomanip>
#include <string>

struct Transaction {
    std::string uuid;

    unsigned int amount; // in cents
    std::string senderBank;
    std::string senderAccount;
    std::string receiverBank;
    std::string receiverAccount;
    tm timestamp;

    std::string serialize() const;
    static Transaction deserialize(const std::string& line);
};

std::ostream& operator<<(std::ostream& stream, const Transaction& tr);
