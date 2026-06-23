#pragma once

#include <iostream>
#include <string>
#include <limits>
#include "UserManager.h"
#include "TransactionHistory.h"
#include "CurrencyConverter.h"
#include "PaymentService.h"

// ----------------------------
// CLI — interactive shell for the payment gateway system
// ----------------------------
class CLI {
private:
    bool running;

    void printBanner() const {
        std::cout << R"(
+===========================================+
|     Payment Gateway System  v2.0          |
|     Interactive CLI                       |
|     Currencies: INR | USD | EUR | GBP | JPY |
+===========================================+
)";
    }

    void printMenu() const {
        std::cout << "\n--- MENU ---\n"
                  << "  1. Add user\n"
                  << "  2. List users\n"
                  << "  3. Send payment\n"
                  << "  4. View all transactions\n"
                  << "  5. View transactions for a user\n"
                  << "  6. Check wallet balance\n"
                  << "  7. View exchange rates\n"
                  << "  8. Exit\n"
                  << "Choose: ";
    }

    void clearInput() {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    // ── handlers ──────────────────────────────────────────────

    void handleAddUser() {
        std::string name, currency;
        int balance;

        std::cout << "Enter username: ";
        std::cin >> name;

        std::cout << "Enter currency (INR/USD/EUR/GBP/JPY): ";
        std::cin >> currency;

        std::cout << "Enter initial balance (whole number): ";
        while (!(std::cin >> balance)) {
            std::cout << "Invalid input. Enter a whole number: ";
            clearInput();
        }

        UserManager::getInstance().addUser(name, currency, balance);
    }

    void handleListUsers() {
        std::cout << "\n--- Registered Users ---\n";
        UserManager::getInstance().listUsers();
    }

    void handleSendPayment() {
        std::string sender, receiver;
        int amount;

        std::cout << "Sender username: ";
        std::cin >> sender;
        if (!UserManager::getInstance().userExists(sender)) {
            std::cout << "User '" << sender << "' not found.\n";
            return;
        }

        std::cout << "Receiver username: ";
        std::cin >> receiver;
        if (!UserManager::getInstance().userExists(receiver)) {
            std::cout << "User '" << receiver << "' not found.\n";
            return;
        }

        if (sender == receiver) {
            std::cout << "Sender and receiver cannot be the same.\n";
            return;
        }

        std::cout << "Amount (in sender's currency, whole number): ";
        while (!(std::cin >> amount) || amount <= 0) {
            std::cout << "Invalid amount. Enter a positive whole number: ";
            clearInput();
        }

        std::cout << "Gateway (1 = Paytm, 2 = Razorpay): ";
        int gwChoice;
        while (!(std::cin >> gwChoice) || (gwChoice != 1 && gwChoice != 2)) {
            std::cout << "Enter 1 or 2: ";
            clearInput();
        }

        GatewayType type       = (gwChoice == 1) ? GatewayType::PAYTM : GatewayType::RAZORPAY;
        std::string gatewayName = (gwChoice == 1) ? "Paytm" : "Razorpay";

        User* senderUser   = UserManager::getInstance().getUser(sender);
        User* receiverUser = UserManager::getInstance().getUser(receiver);

        std::string senderCurr   = senderUser->currency;
        std::string receiverCurr = receiverUser->currency;

        // Convert amount to receiver's currency
        int convertedAmount = CurrencyConverter::getInstance()
                                  .convert(amount, senderCurr, receiverCurr);
        if (convertedAmount < 0) {
            std::cout << "Currency conversion failed. Aborting.\n";
            return;
        }

        // Show the conversion to the user before confirming
        if (senderCurr != receiverCurr) {
            std::cout << "[CurrencyConverter] " << amount << " " << senderCurr
                      << " = " << convertedAmount << " " << receiverCurr << "\n";
        }

        // Debit sender in their own currency
        if (!UserManager::getInstance().debit(sender, amount)) {
            std::cout << "Payment aborted: insufficient balance.\n";
            TransactionHistory::getInstance().record(
                Transaction(sender, receiver,
                            amount, senderCurr,
                            convertedAmount, receiverCurr,
                            gatewayName, false)
            );
            return;
        }

        std::cout << "\n";
        PaymentRequest req(sender, receiver, amount, senderCurr);
        bool result = PaymentController::getInstance().handlePayment(type, &req);

        if (result) {
            // Credit receiver in their own currency
            UserManager::getInstance().credit(receiver, convertedAmount);
            std::cout << ">> Payment SUCCESS: " << sender << " sent "
                      << senderCurr << " " << amount;
            if (senderCurr != receiverCurr)
                std::cout << " -> " << receiverCurr << " " << convertedAmount;
            std::cout << " to " << receiver << " via " << gatewayName << ".\n";
        } else {
            // Refund sender in their own currency
            UserManager::getInstance().credit(sender, amount);
            std::cout << ">> Payment FAILED. " << senderCurr << " "
                      << amount << " refunded to " << sender << ".\n";
        }

        TransactionHistory::getInstance().record(
            Transaction(sender, receiver,
                        amount, senderCurr,
                        convertedAmount, receiverCurr,
                        gatewayName, result)
        );
    }

    void handleAllTransactions() {
        std::cout << "\n--- All Transactions ---\n";
        TransactionHistory::getInstance().printAll();
    }

    void handleUserTransactions() {
        std::string name;
        std::cout << "Enter username: ";
        std::cin >> name;
        std::cout << "\n--- Transactions for '" << name << "' ---\n";
        TransactionHistory::getInstance().printForUser(name);
    }

    void handleCheckBalance() {
        std::string name;
        std::cout << "Enter username: ";
        std::cin >> name;
        User* u = UserManager::getInstance().getUser(name);
        if (!u) {
            std::cout << "User '" << name << "' not found.\n";
            return;
        }
        std::cout << "Balance: " << u->currency << " " << u->balance << "\n";
    }

    void handleExchangeRates() {
        CurrencyConverter::getInstance().printRates();
    }

public:
    CLI() : running(true) {}

    void run() {
        printBanner();
        while (running) {
            printMenu();
            int choice;
            if (!(std::cin >> choice)) {
                clearInput();
                std::cout << "Invalid input.\n";
                continue;
            }
            std::cout << "\n";
            switch (choice) {
                case 1: handleAddUser();          break;
                case 2: handleListUsers();        break;
                case 3: handleSendPayment();      break;
                case 4: handleAllTransactions();  break;
                case 5: handleUserTransactions(); break;
                case 6: handleCheckBalance();     break;
                case 7: handleExchangeRates();    break;
                case 8:
                    std::cout << "Goodbye!\n";
                    running = false;
                    break;
                default:
                    std::cout << "Invalid option. Choose 1-8.\n";
            }
        }
    }
};
