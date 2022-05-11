#include "Client.h"

#include <stdexcept>

Client::Client(std::string firstName, std::string lastName)
    : firstName_(std::move(firstName)), lastName_(std::move(lastName)) {}

uint32_t Client::id() const {
    return id_;
}
std::string Client::firstName() const {
    return firstName_;
}
std::string Client::lastName() const {
    return lastName_;
}

void Client::setAddress(const std::string& addr) {
    addr_ = addr;
}

void Client::setPassportId(const std::string& passportId) {
    passportId_ = passportId;
}

void Client::setId(uint32_t id) {
    if (id_ != 0) {
        throw std::runtime_error("Id for " + firstName_ + " " + lastName_ + " is already set to " +
                                 std::to_string(id_));
    }
    id_ = id;
}

const std::string& Client::getAddress() const {
    return addr_;
}

const std::string& Client::getPassportId() const {
    return passportId_;
}
