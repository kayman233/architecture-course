#pragma once

#include <ctime>
#include <iomanip>
#include <string>
#include "UUID.h"

struct Transaction {
    std::string uuid = get_uuid();

    uint32_t amount;
    std::string senderBankSwift;
    uint32_t senderAccountId;
    std::string receiverBankSwift;
    uint32_t receiverAccountId;
    // timestamp;
};
