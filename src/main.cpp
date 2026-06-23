#include <iostream>
#include <cstdlib>
#include <ctime>
#include "PaymentService.h"

// Helper to print a section header
void printHeader(const std::string& title) {
    std::cout << "\n==============================\n";
    std::cout << "  " << title << "\n";
    std::cout << "==============================\n";
}

int main() {
    srand(static_cast<unsigned>(time(nullptr)));

    std::cout << "===================================\n";
    std::cout << "  Payment Gateway Simulation (LLD)\n";
    std::cout << "===================================\n";

    // -------------------------------------------------------
    // Scenario 1: Valid INR payment via Paytm (3 retries)
    // -------------------------------------------------------
    printHeader("Scenario 1: Paytm | INR 1000 | Aditya -> Shubham");
    {
        PaymentRequest req("Aditya", "Shubham", 1000.0, "INR");
        bool result = PaymentController::getInstance().handlePayment(GatewayType::PAYTM, &req);
        std::cout << ">> Result: " << (result ? "SUCCESS" : "FAIL") << "\n";
    }

    // -------------------------------------------------------
    // Scenario 2: Multi-currency payment via Razorpay (1 retry)
    // -------------------------------------------------------
    printHeader("Scenario 2: Razorpay | USD 500 | Shubham -> Aditya");
    {
        PaymentRequest req("Shubham", "Aditya", 500.0, "USD");
        bool result = PaymentController::getInstance().handlePayment(GatewayType::RAZORPAY, &req);
        std::cout << ">> Result: " << (result ? "SUCCESS" : "FAIL") << "\n";
    }

    // -------------------------------------------------------
    // Scenario 3: Invalid currency rejected by Paytm
    // -------------------------------------------------------
    printHeader("Scenario 3: Paytm | USD 200 | Raj -> Priya (Invalid currency for Paytm)");
    {
        PaymentRequest req("Raj", "Priya", 200.0, "USD");
        bool result = PaymentController::getInstance().handlePayment(GatewayType::PAYTM, &req);
        std::cout << ">> Result: " << (result ? "SUCCESS" : "FAIL") << "\n";
    }

    // -------------------------------------------------------
    // Scenario 4: Zero-amount rejected at validation
    // -------------------------------------------------------
    printHeader("Scenario 4: Razorpay | INR 0 | Meera -> Karan (Invalid amount)");
    {
        PaymentRequest req("Meera", "Karan", 0.0, "INR");
        bool result = PaymentController::getInstance().handlePayment(GatewayType::RAZORPAY, &req);
        std::cout << ">> Result: " << (result ? "SUCCESS" : "FAIL") << "\n";
    }

    std::cout << "\n===================================\n";
    std::cout << "  Simulation Complete\n";
    std::cout << "===================================\n";

    return 0;
}
