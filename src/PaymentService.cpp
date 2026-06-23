#include "PaymentService.h"
#include "UserManager.h"
#include "TransactionHistory.h"
#include "CurrencyConverter.h"

// Static singleton instance definitions
GatewayFactory     GatewayFactory::instance;
PaymentService     PaymentService::instance;
PaymentController  PaymentController::instance;
UserManager        UserManager::instance;
TransactionHistory TransactionHistory::instance;
CurrencyConverter  CurrencyConverter::instance;
