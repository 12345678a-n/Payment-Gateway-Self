#pragma once

#include "PaymentGateway.h"

// ----------------------------
// Abstract base class for Payment Gateway (Template Method Pattern)
// Defines the skeleton of the payment flow; concrete subclasses fill in the steps.
// ----------------------------
class PaymentGateway {
protected:
    BankingSystem* bankingSystem;

public:
    PaymentGateway() : bankingSystem(nullptr) {}

    virtual ~PaymentGateway() {
        delete bankingSystem;
    }

    // Template method: defines the standard payment pipeline
    virtual bool processPayment(PaymentRequest* request) {
        if (!validatePayment(request)) {
            std::cout << "[PaymentGateway] Validation failed for " << request->sender << ".\n";
            return false;
        }
        if (!initiatePayment(request)) {
            std::cout << "[PaymentGateway] Initiation failed for " << request->sender << ".\n";
            return false;
        }
        if (!confirmPayment(request)) {
            std::cout << "[PaymentGateway] Confirmation failed for " << request->sender << ".\n";
            return false;
        }
        return true;
    }

    virtual bool validatePayment(PaymentRequest* request) = 0;
    virtual bool initiatePayment(PaymentRequest* request) = 0;
    virtual bool confirmPayment(PaymentRequest* request) = 0;
};

// ----------------------------
// Concrete Payment Gateway: Paytm
// Only accepts INR currency.
// ----------------------------
class PaytmGateway : public PaymentGateway {
public:
    PaytmGateway() {
        bankingSystem = new PaytmBankingSystem();
    }

    bool validatePayment(PaymentRequest* request) override {
        std::cout << "[Paytm] Validating payment for " << request->sender << ".\n";
        if (request->amount <= 0 || request->currency != "INR") {
            std::cout << "[Paytm] Validation failed: amount must be > 0 and currency must be INR.\n";
            return false;
        }
        return true;
    }

    bool initiatePayment(PaymentRequest* request) override {
        std::cout << "[Paytm] Initiating payment of " << request->amount
                  << " " << request->currency << " for " << request->sender << ".\n";
        return bankingSystem->processPayment(request->amount);
    }

    bool confirmPayment(PaymentRequest* request) override {
        std::cout << "[Paytm] Confirming payment for " << request->sender << ".\n";
        return true; // Confirmation always succeeds in this simulation
    }
};

// ----------------------------
// Concrete Payment Gateway: Razorpay
// Accepts any currency, amount must be positive.
// ----------------------------
class RazorpayGateway : public PaymentGateway {
public:
    RazorpayGateway() {
        bankingSystem = new RazorpayBankingSystem();
    }

    bool validatePayment(PaymentRequest* request) override {
        std::cout << "[Razorpay] Validating payment for " << request->sender << ".\n";
        if (request->amount <= 0) {
            std::cout << "[Razorpay] Validation failed: amount must be > 0.\n";
            return false;
        }
        return true;
    }

    bool initiatePayment(PaymentRequest* request) override {
        std::cout << "[Razorpay] Initiating payment of " << request->amount
                  << " " << request->currency << " for " << request->sender << ".\n";
        return bankingSystem->processPayment(request->amount);
    }

    bool confirmPayment(PaymentRequest* request) override {
        std::cout << "[Razorpay] Confirming payment for " << request->sender << ".\n";
        return true; // Confirmation always succeeds in this simulation
    }
};

// ----------------------------
// Proxy class: wraps any PaymentGateway to add retry logic (Proxy Pattern)
// Transparently intercepts processPayment() and retries on failure.
// ----------------------------
class PaymentGatewayProxy : public PaymentGateway {
    PaymentGateway* realGateway;
    int maxRetries;

public:
    PaymentGatewayProxy(PaymentGateway* gateway, int retries)
        : realGateway(gateway), maxRetries(retries) {}

    ~PaymentGatewayProxy() override {
        delete realGateway;
    }

    bool processPayment(PaymentRequest* request) override {
        bool result = false;
        for (int attempt = 1; attempt <= maxRetries; ++attempt) {
            if (attempt > 1) {
                std::cout << "[Proxy] Retrying payment (attempt " << attempt
                          << "/" << maxRetries << ") for " << request->sender << ".\n";
            }
            result = realGateway->processPayment(request);
            if (result) break;
        }
        if (!result) {
            std::cout << "[Proxy] Payment failed after " << maxRetries
                      << " attempt(s) for " << request->sender << ".\n";
        }
        return result;
    }

    // Delegate individual steps to the real gateway (satisfies the abstract interface)
    bool validatePayment(PaymentRequest* request) override {
        return realGateway->validatePayment(request);
    }
    bool initiatePayment(PaymentRequest* request) override {
        return realGateway->initiatePayment(request);
    }
    bool confirmPayment(PaymentRequest* request) override {
        return realGateway->confirmPayment(request);
    }
};
