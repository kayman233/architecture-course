#pragma once

#include <string>

class Bank;

class Client {
    uint32_t id_ = 0;
    std::string firstName_;
    std::string lastName_;
    std::string addr_;
    std::string passportId_;
    bool is_sus = true;

public:
    Client(std::string firstName, std::string lastName);

    uint32_t id() const;
    std::string firstName() const;
    std::string lastName() const;

    void setAddress(const std::string& addr);

    void setPassportId(const std::string& passportId);

    bool isSus() const {
        return is_sus;
    }

    void setIsSus(bool is_sus_) {
        Client::is_sus = is_sus_;
    }

    const std::string& getAddr() const;

    const std::string& getPassportId() const;

private:
    void setId(uint32_t id);

    friend class Bank;
};
