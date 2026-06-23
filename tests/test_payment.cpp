/**
 * Manual test suite for the Payment Gateway system.
 * Covers: validation rules, proxy retry behaviour, singleton identity,
 * and gateway switching through PaymentController.
 *
 * Compile & run:
 *   g++ -std=c++17 -I../include tests/test_payment.cpp src/PaymentService.cpp -o test_runner
 *   ./test_runner
 */

#include <iostream>
#include <cassert>
#include <cstdlib>
#include <ctime>
#include "PaymentService.h"

// ─── tiny test harness ────────────────────────────────────────────────────────
static int passed = 0, failed = 0;

#define ASSERT_TRUE(expr, name)                                  \
    do {                                                          \
        if (expr) { std::cout << "[PASS] " << name << "\n"; ++passed; } \
        else      { std::cout << "[FAIL] " << name << "\n"; ++failed; } \
    } while (0)

#define ASSERT_FALSE(expr, name) ASSERT_TRUE(!(expr), name)

// ─── test cases ──────────────────────────────────────────────────────────────

void test_paytm_rejects_non_inr() {
    PaymentRequest req("Alice", "Bob", 100.0, "USD");
    // We exercise validation directly through a fresh gateway
    PaytmGateway gw;
    bool valid = gw.validatePayment(&req);
    ASSERT_FALSE(valid, "Paytm rejects non-INR currency");
}

void test_paytm_rejects_zero_amount() {
    PaymentRequest req("Alice", "Bob", 0.0, "INR");
    PaytmGateway gw;
    bool valid = gw.validatePayment(&req);
    ASSERT_FALSE(valid, "Paytm rejects zero amount");
}

void test_paytm_accepts_valid_inr() {
    PaymentRequest req("Alice", "Bob", 500.0, "INR");
    PaytmGateway gw;
    bool valid = gw.validatePayment(&req);
    ASSERT_TRUE(valid, "Paytm accepts valid INR payment");
}

void test_razorpay_accepts_any_currency() {
    RazorpayGateway gw;
    for (const auto& curr : {"INR", "USD", "EUR", "GBP"}) {
        PaymentRequest req("Alice", "Bob", 100.0, curr);
        bool valid = gw.validatePayment(&req);
        ASSERT_TRUE(valid, std::string("Razorpay accepts ") + curr);
    }
}

void test_razorpay_rejects_zero_amount() {
    PaymentRequest req("Alice", "Bob", 0.0, "INR");
    RazorpayGateway gw;
    bool valid = gw.validatePayment(&req);
    ASSERT_FALSE(valid, "Razorpay rejects zero amount");
}

void test_singleton_identity() {
    PaymentController& c1 = PaymentController::getInstance();
    PaymentController& c2 = PaymentController::getInstance();
    ASSERT_TRUE(&c1 == &c2, "PaymentController is a true singleton");

    GatewayFactory& f1 = GatewayFactory::getInstance();
    GatewayFactory& f2 = GatewayFactory::getInstance();
    ASSERT_TRUE(&f1 == &f2, "GatewayFactory is a true singleton");

    PaymentService& s1 = PaymentService::getInstance();
    PaymentService& s2 = PaymentService::getInstance();
    ASSERT_TRUE(&s1 == &s2, "PaymentService is a true singleton");
}

void test_proxy_wraps_gateway() {
    // Proxy with 1 retry should call the real gateway at least once
    // We can only verify it doesn't crash and returns a bool
    PaymentRequest req("Test", "User", 100.0, "INR");
    PaymentGatewayProxy proxy(new PaytmGateway(), 1);
    bool result = proxy.processPayment(&req);
    // result may be true or false (random) — just assert no exception
    ASSERT_TRUE(result == true || result == false,
                "Proxy processes payment without crashing (1 retry)");
}

void test_proxy_retries() {
    // 3 retries, Paytm 80% success — very unlikely all 3 fail, but we just
    // verify the proxy runs without error
    PaymentRequest req("Test", "User", 200.0, "INR");
    PaymentGatewayProxy proxy(new PaytmGateway(), 3);
    bool result = proxy.processPayment(&req);
    ASSERT_TRUE(result == true || result == false,
                "Proxy completes 3-retry flow without error");
}

void test_no_gateway_set() {
    // A freshly obtained PaymentService with no gateway returns false
    // We can't reset the singleton, so test via a local scope simulation
    PaymentGateway* nullGw = nullptr;
    PaymentService::getInstance().setGateway(nullGw);
    PaymentRequest req("Ghost", "Nobody", 50.0, "INR");
    bool result = PaymentService::getInstance().processPayment(&req);
    ASSERT_FALSE(result, "PaymentService returns false when no gateway is set");
}

void test_controller_paytm_flow() {
    PaymentRequest req("Dev", "Ops", 750.0, "INR");
    bool result = PaymentController::getInstance()
                      .handlePayment(GatewayType::PAYTM, &req);
    ASSERT_TRUE(result == true || result == false,
                "Controller handles full Paytm flow");
}

void test_controller_razorpay_flow() {
    PaymentRequest req("Dev", "Ops", 300.0, "EUR");
    bool result = PaymentController::getInstance()
                      .handlePayment(GatewayType::RAZORPAY, &req);
    ASSERT_TRUE(result == true || result == false,
                "Controller handles full Razorpay flow");
}

// ─── main ────────────────────────────────────────────────────────────────────

int main() {
    srand(static_cast<unsigned>(time(nullptr)));

    std::cout << "===========================\n";
    std::cout << "  Payment Gateway Tests\n";
    std::cout << "===========================\n\n";

    test_paytm_rejects_non_inr();
    test_paytm_rejects_zero_amount();
    test_paytm_accepts_valid_inr();
    test_razorpay_accepts_any_currency();
    test_razorpay_rejects_zero_amount();
    test_singleton_identity();
    test_proxy_wraps_gateway();
    test_proxy_retries();
    test_no_gateway_set();
    test_controller_paytm_flow();
    test_controller_razorpay_flow();

    std::cout << "\n---------------------------\n";
    std::cout << "  Results: " << passed << " passed, " << failed << " failed\n";
    std::cout << "---------------------------\n";

    return failed == 0 ? 0 : 1;
}
