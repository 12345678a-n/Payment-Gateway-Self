# Payment Gateway System - Class Diagram

```text
+------------------------------------------------+
|                     CLI                        |
+------------------------------------------------+
                    |
                    v

+------------------------------------------------+
|                 UserManager                    |
+------------------------------------------------+
| + addUser()                                    |
| + getUser()                                    |
| + debit()                                      |
| + credit()                                     |
| + listUsers()                                  |
+------------------------------------------------+

                    |
                    v

+------------------------------------------------+
|                    User                        |
+------------------------------------------------+
| username                                       |
| currency                                       |
| balance                                        |
+------------------------------------------------+


+------------------------------------------------+
|             PaymentController                  |
+------------------------------------------------+
| + handlePayment()                              |
+------------------------------------------------+
                    |
                    v

+------------------------------------------------+
|             PaymentGateway                     |
|               (Abstract)                       |
+------------------------------------------------+
| + processPayment()                             |
+------------------------------------------------+
            ^                      ^
            |                      |

+-------------------+   +-------------------+
|   PaytmGateway    |   | RazorpayGateway   |
+-------------------+   +-------------------+


+------------------------------------------------+
|            CurrencyConverter                   |
+------------------------------------------------+
| + convert()                                   |
| + printRates()                               |
+------------------------------------------------+


+------------------------------------------------+
|           TransactionHistory                   |
+------------------------------------------------+
| + record()                                   |
| + printAll()                                 |
| + printForUser()                             |
+------------------------------------------------+
                    |
                    v

+------------------------------------------------+
|                Transaction                     |
+------------------------------------------------+
| sender                                         |
| receiver                                       |
| amount                                         |
| currencies                                     |
| gateway                                        |
| status                                         |
+------------------------------------------------+
```

## Design Overview

### CLI

Handles user interaction and application flow.

### UserManager

Responsible for user creation, balance management, debit, and credit operations.

### PaymentController

Coordinates payment execution through selected gateways.

### PaymentGateway

Abstract interface representing a payment provider.

### PaytmGateway / RazorpayGateway

Concrete implementations of payment processing providers.

### CurrencyConverter

Performs currency conversion using maintained exchange rates.

### TransactionHistory

Stores and retrieves transaction records.

### Transaction

Represents a completed or failed payment event.
