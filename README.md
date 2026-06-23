# Payment Gateway System (LLD Project)

A modular C++ Payment Gateway System that simulates real-world digital payment processing. The project supports multiple payment providers, multi-currency transactions, wallet management, currency conversion, and transaction history tracking while demonstrating core Low-Level Design (LLD) principles and object-oriented design patterns.

---

## Features

### User Management

* Create and manage users.
* Maintain wallet balances.
* Support multiple currencies:

  * INR
  * USD
  * EUR
  * GBP
  * JPY

### Payment Processing

* Transfer money between users.
* Multiple payment providers:

  * Paytm
  * Razorpay
* Runtime gateway selection.
* Automatic refund on failed payments.

### Currency Conversion

* Automatic currency conversion during cross-currency transfers.
* Centralized exchange rate management.
* Exchange rate viewing through CLI.

### Transaction History

* Record successful transactions.
* Record failed transactions.
* View all transactions.
* View transaction history for a specific user.

### Command Line Interface

* Interactive menu-driven application.
* Input validation and error handling.
* User-friendly transaction flow.

---

## Design Patterns Used

### Singleton Pattern

Used for globally shared services:

* UserManager
* CurrencyConverter
* TransactionHistory
* PaymentController

Benefits:

* Single source of truth.
* Controlled access to shared resources.
* Simplified management of system-wide services.

### Strategy / Polymorphism

Implemented through the PaymentGateway abstraction.

Payment Providers:

* PaytmGateway
* RazorpayGateway

Benefits:

* Extensible architecture.
* Easy addition of new payment providers.
* Decouples payment logic from business logic.

---

## Project Structure

```text
payment-gateway/
│
├── diagrams/
│   └── class_diagram.md
│
├── include/
│   ├── CLI.h
│   ├── CurrencyConverter.h
│   ├── Gateways.h
│   ├── PaymentGateway.h
│   ├── PaymentService.h
│   ├── TransactionHistory.h
│   └── UserManager.h
│
├── src/
│   ├── PaymentService.cpp
│   └── main.cpp
│
├── .gitignore
├── CHANGES.md
├── Makefile
└── README.md
```

---

## System Architecture

```text
                 CLI
                  |
                  v
       +-------------------+
       | PaymentController |
       +-------------------+
          /      |       \
         /       |        \
        v        v         v

 PaytmGateway RazorpayGateway
        |
        v

 CurrencyConverter

        |
        v

 TransactionHistory

        |
        v

   UserManager
```

---

## Build Instructions

### Compile

```bash
make
```

### Run

```bash
./payment_gateway
```

---


## Learning Outcomes

This project demonstrates:

* Object-Oriented Programming
* Low-Level Design
* Design Patterns
* Interface-based Architecture
* Modular Software Design
* C++ STL Usage
* Clean Code Principles

---

## Author

Saket Pratap Singh

IIT Kanpur | Aerospace Engineering

Competitive Programming • Systems Design • Software Development
