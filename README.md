# Payment Gateway System — C++ LLD Project

A **Low-Level Design (LLD)** implementation of a payment gateway system in modern C++17, demonstrating four industry-standard design patterns working together in a realistic domain.

---

## Design Patterns Used

| Pattern | Where | Why |
|---|---|---|
| **Strategy** | `BankingSystem` hierarchy | Swap payment processors (Paytm, Razorpay) without changing gateway logic |
| **Template Method** | `PaymentGateway` base class | Fix the payment pipeline (validate → initiate → confirm); let subclasses customise each step |
| **Proxy** | `PaymentGatewayProxy` | Add retry logic transparently around any gateway without modifying it |
| **Singleton + Factory Method** | `GatewayFactory`, `PaymentService`, `PaymentController` | Single point of creation and a unified API surface for client code |

---

## Architecture

```
Client / main()
     │
     ▼
PaymentController  (Singleton)
     │  handlePayment(type, request)
     ▼
GatewayFactory  (Singleton + Factory Method)
     │  getGateway(type)  →  returns PaymentGatewayProxy wrapping a concrete gateway
     ▼
PaymentService  (Singleton)
     │  processPayment(request)
     ▼
PaymentGatewayProxy  (Proxy — adds retry logic)
     │  processPayment(request)
     ▼
PaymentGateway  (Template Method)
  validatePayment()
  initiatePayment()
  confirmPayment()
     │
     ├── PaytmGateway    (Strategy: uses PaytmBankingSystem)
     └── RazorpayGateway (Strategy: uses RazorpayBankingSystem)
```

---

## Project Structure

```
payment-gateway/
├── include/
│   ├── PaymentGateway.h   # PaymentRequest, BankingSystem + concrete processors
│   ├── Gateways.h         # Abstract PaymentGateway, concrete gateways, Proxy
│   └── PaymentService.h   # GatewayFactory, PaymentService, PaymentController
├── src/
│   ├── main.cpp           # Demo: 4 realistic payment scenarios
│   └── PaymentService.cpp # Static singleton instance definitions
├── tests/
│   └── test_payment.cpp   # 16 manual test cases (no external framework needed)
├── diagrams/
│   └── class_diagram.md   # UML class diagram in text form
├── Makefile
├── .gitignore
└── README.md
```

---

## Getting Started

### Prerequisites
- `g++` with C++17 support (`g++ --version` should be ≥ 7)
- `make`

### Build & Run

```bash
# Clone the repo
git clone https://github.com/<your-username>/payment-gateway.git
cd payment-gateway

# Build
make

# Run the demo
make run

# Run tests
make test

# Clean build artifacts
make clean
```

---

## Sample Output

```
===================================
  Payment Gateway Simulation (LLD)
===================================

==============================
  Scenario 1: Paytm | INR 1000 | Aditya -> Shubham
==============================
[Paytm] Validating payment for Aditya.
[Paytm] Initiating payment of 1000 INR for Aditya.
[BankingSystem-Paytm] Processing payment of 1000...
[Paytm] Confirming payment for Aditya.
>> Result: SUCCESS

==============================
  Scenario 3: Paytm | USD 200 | Raj -> Priya (Invalid currency for Paytm)
==============================
[Paytm] Validating payment for Raj.
[Paytm] Validation failed: amount must be > 0 and currency must be INR.
[Proxy] Retrying payment (attempt 2/3) for Raj.
...
[Proxy] Payment failed after 3 attempt(s) for Raj.
>> Result: FAIL
```

---

## Key Design Decisions

**Why Template Method for `PaymentGateway`?**
The payment flow (validate → initiate → confirm) is fixed, but each gateway implements the steps differently. Template Method enforces the order while letting subclasses define behaviour — avoiding code duplication.

**Why Proxy for retries instead of putting it in the gateway?**
Retrying is a cross-cutting concern. The Proxy keeps retry logic in one place; adding a new gateway requires zero changes to retry behaviour.

**Why static Singletons instead of `static local` (Meyers Singleton)?**
The static data member approach makes the single-instance guarantee explicit and mirrors patterns commonly seen in production C++ codebases for didactic clarity.

**Why separate `BankingSystem` from `PaymentGateway`?**
A gateway's routing/validation logic is independent of the underlying bank's API. Separating them (Strategy) means you can swap a payment processor without rewriting gateway rules.

---

## Extending the System

Adding a new gateway (e.g. Stripe):
1. Add `StripeBankingSystem : BankingSystem` in `PaymentGateway.h`
2. Add `StripeGateway : PaymentGateway` in `Gateways.h`
3. Add `GatewayType::STRIPE` to the enum in `PaymentService.h`
4. Add a `case` in `GatewayFactory::getGateway()`

No other files change — Open/Closed Principle in action.

---

## Test Coverage

The test suite (`tests/test_payment.cpp`) covers:

- Validation rules (currency and amount checks) for both gateways
- Proxy retry flow (1 and 3 retries)
- Singleton identity assertions
- Null-gateway guard in `PaymentService`
- Full end-to-end flows via `PaymentController`

Run with `make test` — no external framework required.
