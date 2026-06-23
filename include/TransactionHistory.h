#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <iomanip>
#include <ctime>

// ----------------------------
// Represents a single completed (or failed) transaction.
// Stores both the sent amount (sender's currency) and
// the received amount (receiver's currency) for cross-currency transfers.
// ----------------------------
struct Transaction {
    std::string sender;
    std::string receiver;
    int         sentAmount;       // amount debited from sender
    std::string sentCurrency;
    int         receivedAmount;   // amount credited to receiver (after conversion)
    std::string receivedCurrency;
    std::string gateway;
    bool        success;
    std::string timestamp;

    Transaction(const std::string& s, const std::string& r,
                int sent,   const std::string& sentCurr,
                int recvd, const std::string& recvdCurr,
                const std::string& gw, bool ok)
        : sender(s), receiver(r),
          sentAmount(sent),     sentCurrency(sentCurr),
          receivedAmount(recvd), receivedCurrency(recvdCurr),
          gateway(gw), success(ok)
    {
        std::time_t t = std::time(nullptr);
        char buf[20];
        std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", std::localtime(&t));
        timestamp = buf;
    }
};

// ----------------------------
// TransactionHistory (Singleton)
// ----------------------------
class TransactionHistory {
private:
    static TransactionHistory instance;
    std::vector<Transaction> transactions;

    TransactionHistory() = default;
    TransactionHistory(const TransactionHistory&) = delete;
    TransactionHistory& operator=(const TransactionHistory&) = delete;

    void printHeader() const {
        std::cout << std::left
                  << std::setw(10) << "Sender"
                  << std::setw(10) << "Receiver"
                  << std::setw(14) << "Sent"
                  << std::setw(14) << "Received"
                  << std::setw(10) << "Gateway"
                  << std::setw(9)  << "Status"
                  << "Time\n";
        std::cout << std::string(85, '-') << "\n";
    }

    void printRow(const Transaction& t) const {
        std::string sent  = std::to_string(t.sentAmount)     + " " + t.sentCurrency;
        std::string recvd = std::to_string(t.receivedAmount) + " " + t.receivedCurrency;
        std::cout << std::left
                  << std::setw(10) << t.sender
                  << std::setw(10) << t.receiver
                  << std::setw(14) << sent
                  << std::setw(14) << recvd
                  << std::setw(10) << t.gateway
                  << std::setw(9)  << (t.success ? "SUCCESS" : "FAIL")
                  << t.timestamp   << "\n";
    }

public:
    static TransactionHistory& getInstance() {
        return instance;
    }

    void record(const Transaction& txn) {
        transactions.push_back(txn);
    }

    void printAll() const {
        if (transactions.empty()) {
            std::cout << "  No transactions yet.\n";
            return;
        }
        printHeader();
        for (const auto& t : transactions) printRow(t);
    }

    void printForUser(const std::string& name) const {
        bool found = false;
        for (const auto& t : transactions) {
            if (t.sender == name || t.receiver == name) {
                if (!found) { printHeader(); found = true; }
                printRow(t);
            }
        }
        if (!found) std::cout << "  No transactions found for '" << name << "'.\n";
    }
};
