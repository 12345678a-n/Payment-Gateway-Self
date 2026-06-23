# Changes from Original Code

This document lists every change made to the original `payment_gateway.cpp`, along with the reason. The core logic and design patterns are **unchanged** — all modifications improve code quality, correctness, or completeness.

---

## Bug Fixes

### 1. `reciever` typo corrected to `receiver`
- **Where:** `PaymentRequest` struct, field name and constructor parameter
- **Why:** Typo in original. Consistent spelling matters for readability and searchability.

### 2. `PaytmBankingSystem::processPayment` was silent
- **Where:** `PaytmBankingSystem::processPayment()`
- **Original:** No log output (only `RazorpayBankingSystem` printed a message)
- **Fixed:** Added `[BankingSystem-Paytm] Processing payment of X...` log, making both processors consistent.

### 3. Proxy retry counter started at 0, log showed `attempt 2` on first retry
- **Where:** `PaymentGatewayProxy::processPayment()`
- **Original:** Loop `for (int attempt = 0; attempt < retries; ++attempt)` with `attempt+1` in the log
- **Fixed:** Loop now runs `for (int attempt = 1; attempt <= maxRetries; ++attempt)`. Log shows `attempt 2/3`, `attempt 3/3` — unambiguous and human-readable.

---

## Code Quality Improvements

### 4. Replaced `using namespace std;` with explicit `std::` prefix
- **Why:** `using namespace std;` in headers pollutes every file that includes them — a known anti-pattern in production C++.

### 5. Member initialiser lists used in constructors
- **Where:** `PaymentRequest`, `PaymentGateway`, `PaymentGatewayProxy`, `PaymentService`
- **Why:** Initialiser lists are more efficient (no double-initialisation) and idiomatic modern C++.

### 6. `= default` / `= delete` used where applicable
- **Where:** Singleton destructors, copy constructors, assignment operators
- **Why:** More explicit intent than empty `{}` bodies; `= default` lets the compiler generate the optimal implementation.

### 7. `virtual ~BankingSystem() {}` → `virtual ~BankingSystem() = default`
- **Why:** `= default` is preferred for compiler-generated destructors.

### 8. Added `delete gateway` before reassignment in `PaymentService::setGateway()`
- **Original:** `if (gateway) delete gateway; gateway = g;`
- **Fixed:** Simply `delete gateway; gateway = g;` — `delete nullptr` is safe in C++, so the guard is unnecessary noise.

---

## Architecture Improvements

### 9. Split monolithic file into headers + source
| File | Contents |
|---|---|
| `include/PaymentGateway.h` | `PaymentRequest`, `BankingSystem`, concrete processors |
| `include/Gateways.h` | `PaymentGateway` (abstract), `PaytmGateway`, `RazorpayGateway`, `PaymentGatewayProxy` |
| `include/PaymentService.h` | `GatewayType`, `GatewayFactory`, `PaymentService`, `PaymentController` |
| `src/PaymentService.cpp` | Static singleton instance definitions |
| `src/main.cpp` | Demo / client code |

- **Why:** Single-file monoliths don't scale. Splitting by responsibility makes each class independently reviewable and mirrors real project structure.

### 10. `#pragma once` instead of include guards
- **Why:** Universally supported, less error-prone, and cleaner than `#ifndef` guards.

---

## Demo (main.cpp) Improvements

### 11. Added two extra scenarios
- **Scenario 3:** Paytm rejecting a non-INR (USD) payment — shows validation failure path + proxy retry exhaustion
- **Scenario 4:** Razorpay rejecting a zero-amount payment — shows validation failure at Razorpay

- **Why:** The original only showed two happy-path calls. Edge cases are what interviewers look for.

### 12. Better log messaging
- Proxy retry log now shows `attempt X/maxRetries` for full clarity
- Validation failure reasons are printed by the gateway, not just by `PaymentGateway` base class

---

## New Files Added

| File | Purpose |
|---|---|
| `tests/test_payment.cpp` | 16 manual test cases — no external framework needed |
| `Makefile` | `make`, `make run`, `make test`, `make clean` |
| `diagrams/class_diagram.md` | Full ASCII UML class diagram + flow trace |
| `README.md` | Project overview, architecture, usage, design decisions |
| `.gitignore` | Standard C++ gitignore |
| `CHANGES.md` | This file |

---

## What Was NOT Changed

- All four design patterns (Strategy, Template Method, Proxy, Singleton + Factory Method) are identical to the original.
- The class hierarchy and ownership model are unchanged.
- The `GatewayType` enum, retry counts (Paytm: 3, Razorpay: 1), and success probabilities (80%, 90%) are unchanged.
- `srand(time(nullptr))` seeding is unchanged.
