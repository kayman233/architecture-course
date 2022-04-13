#include "Transaction.h"

std::ostream &operator<<(std::ostream &stream, const Transaction &tr) {
    return stream << "Transaction " << std::to_string(tr.amount) << " (cents) from " << tr.senderBank << ", account "
                  << tr.senderAccount << " to " << tr.receiverBank << ", account " << tr.receiverAccount << " dated "
                  << std::put_time(&tr.timestamp, "%c %Z");
}

std::string Transaction::serialize() const {
    tm timestampCopy = timestamp;
    std::stringstream ss;
    ss << std::to_string(amount) << '\t' << senderBank << '\t' << senderAccount << '\t' << receiverBank
       << receiverAccount << '\t' << std::mktime(&timestampCopy);
    return ss.str();
}

Transaction Transaction::deserialize(const std::string &line) {
    throw std::runtime_error("todo");
}
