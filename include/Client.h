#pragma once

#include <string>

class Bank;

class Client {
    uint32_t id_ = 0;
    std::string firstName_;
    std::string lastName_;
    std::string addr_;
    std::string passportId_;

public:
    Client(std::string firstName, std::string lastName);

    uint32_t id() const;
    std::string firstName() const;
    std::string lastName() const;

    void setAddress(const std::string& addr);
    std::string address() const;

    void setPassportId(const std::string& passportId);
    std::string passportId() const;

private:
    void setId(uint32_t id);

    friend class Bank;
};
