#pragma once

#include <string>
#include <unordered_map>
#include <iostream>
#include <iomanip>
#include "CurrencyConverter.h"

// ----------------------------
// Represents a user with an integer wallet balance.
// ----------------------------
struct User {
    std::string name;
    std::string currency;
    int         balance;

    User() : name(""), currency("INR"), balance(0) {}
    User(const std::string& n, const std::string& c, int b)
        : name(n), currency(c), balance(b) {}
};

// ----------------------------
// UserManager (Singleton)
// ----------------------------
class UserManager {
private:
    static UserManager instance;
    std::unordered_map<std::string, User> users;

    UserManager() = default;
    UserManager(const UserManager&) = delete;
    UserManager& operator=(const UserManager&) = delete;

public:
    static UserManager& getInstance() {
        return instance;
    }

    bool addUser(const std::string& name, const std::string& currency, int balance) {
        if (users.count(name)) {
            std::cout << "[UserManager] User '" << name << "' already exists.\n";
            return false;
        }
        if (!CurrencyConverter::getInstance().isSupported(currency)) {
            std::cout << "[UserManager] Currency '" << currency << "' is not supported.\n";
            return false;
        }
        if (balance < 0) {
            std::cout << "[UserManager] Initial balance cannot be negative.\n";
            return false;
        }
        users[name] = User(name, currency, balance);
        std::cout << "[UserManager] User '" << name << "' created with "
                  << currency << " " << balance << ".\n";
        return true;
    }

    bool userExists(const std::string& name) const {
        return users.count(name) > 0;
    }

    User* getUser(const std::string& name) {
        auto it = users.find(name);
        if (it == users.end()) return nullptr;
        return &it->second;
    }

    bool debit(const std::string& name, int amount) {
        User* u = getUser(name);
        if (!u) return false;
        if (u->balance < amount) {
            std::cout << "[UserManager] Insufficient balance for '" << name
                      << "' (has " << u->currency << " " << u->balance
                      << ", needs " << amount << ").\n";
            return false;
        }
        u->balance -= amount;
        return true;
    }

    void credit(const std::string& name, int amount) {
        User* u = getUser(name);
        if (u) u->balance += amount;
    }

    void listUsers() const {
        if (users.empty()) {
            std::cout << "  No users registered yet.\n";
            return;
        }
        std::cout << std::left
                  << std::setw(16) << "Name"
                  << std::setw(8)  << "Currency"
                  << "Balance\n";
        std::cout << std::string(36, '-') << "\n";
        for (const auto& [name, user] : users) {
            std::cout << std::setw(16) << user.name
                      << std::setw(8)  << user.currency
                      << user.balance  << "\n";
        }
    }
};
