# Changelog

## Version 2.0

### Major Enhancements

#### Multi-Currency Support

* Added support for INR, USD, EUR, GBP, and JPY.
* Introduced automatic currency conversion for cross-currency transactions.

#### Payment Gateway Integration

* Added Paytm gateway.
* Added Razorpay gateway.
* Implemented gateway selection at runtime.

#### Transaction Tracking

* Added transaction history management.
* Added successful transaction recording.
* Added failed transaction recording.
* Added user-specific transaction queries.

#### User Management

* Added wallet balance management.
* Added user validation before transaction execution.
* Added balance checking functionality.

#### CLI Improvements

* Interactive menu-driven interface.
* Improved user feedback.
* Better error handling and validation.

---

## Design Improvements

### Architecture

* Introduced centralized PaymentController.
* Improved separation of concerns.
* Reduced coupling between modules.

### Design Patterns

Implemented:

* Singleton Pattern
* Strategy Pattern (Gateway Abstraction)
* Interface-Based Design

---

## Version 1.0

Initial release containing:

* Basic user management.
* Simple payment simulation.
* Single-currency support.
* Basic gateway processing.
