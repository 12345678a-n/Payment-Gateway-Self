#pragma once

#include <iostream>
#include <string>
#include <stdexcept>

// ----------------------------
// Data structure for payment details
// ----------------------------
struct PaymentRequest {
    std::string sender;
    std::string receiver;
    double amount;
    std::string currency;

    PaymentRequest(const std::string& sender, const std::string& receiver,
                   double amt, const std::string& curr)
        : sender(sender), receiver(receiver), amount(amt), currency(curr) {}
};

// ----------------------------
// Banking System interface and implementations (Strategy Pattern)
// Each concrete BankingSystem simulates a real-world payment processor.
// ----------------------------
class BankingSystem {
public:
    virtual bool processPayment(double amount) = 0;
    virtual ~BankingSystem() = default;
};

class PaytmBankingSystem : public BankingSystem {
public:
    bool processPayment(double amount) override {
        std::cout << "[BankingSystem-Paytm] Processing payment of " << amount << "...\n";
        // Simulate 80% success rate
        return (rand() % 100) < 80;
    }
};

class RazorpayBankingSystem : public BankingSystem {
public:
    bool processPayment(double amount) override {
        std::cout << "[BankingSystem-Razorpay] Processing payment of " << amount << "...\n";
        // Simulate 90% success rate
        return (rand() % 100) < 90;
    }
};
