#pragma once

#include "Gateways.h"

// ----------------------------
// Supported gateway types
// ----------------------------
enum class GatewayType {
    PAYTM,
    RAZORPAY
};

// ----------------------------
// GatewayFactory (Singleton + Factory Method Pattern)
// Single point of creation for all payment gateway instances.
// Each gateway is wrapped in a Proxy with retry logic.
// ----------------------------
class GatewayFactory {
private:
    static GatewayFactory instance;

    GatewayFactory() = default;
    GatewayFactory(const GatewayFactory&) = delete;
    GatewayFactory& operator=(const GatewayFactory&) = delete;

public:
    static GatewayFactory& getInstance() {
        return instance;
    }

    PaymentGateway* getGateway(GatewayType type) {
        if (type == GatewayType::PAYTM) {
            return new PaymentGatewayProxy(new PaytmGateway(), 3);
        } else {
            return new PaymentGatewayProxy(new RazorpayGateway(), 1);
        }
    }
};

// ----------------------------
// PaymentService (Singleton)
// Unified API surface exposed to the rest of the application.
// Holds the currently active gateway and delegates payment calls to it.
// ----------------------------
class PaymentService {
private:
    static PaymentService instance;
    PaymentGateway* gateway;

    PaymentService() : gateway(nullptr) {}
    ~PaymentService() { delete gateway; }

    PaymentService(const PaymentService&) = delete;
    PaymentService& operator=(const PaymentService&) = delete;

public:
    static PaymentService& getInstance() {
        return instance;
    }

    void setGateway(PaymentGateway* g) {
        delete gateway;    // clean up previous gateway before switching
        gateway = g;
    }

    bool processPayment(PaymentRequest* request) {
        if (!gateway) {
            std::cout << "[PaymentService] No payment gateway selected.\n";
            return false;
        }
        return gateway->processPayment(request);
    }
};

// ----------------------------
// PaymentController (Singleton)
// Entry point for all client-facing payment requests.
// Coordinates Factory → Service → Gateway flow.
// ----------------------------
class PaymentController {
private:
    static PaymentController instance;

    PaymentController() = default;
    PaymentController(const PaymentController&) = delete;
    PaymentController& operator=(const PaymentController&) = delete;

public:
    static PaymentController& getInstance() {
        return instance;
    }

    bool handlePayment(GatewayType type, PaymentRequest* req) {
        PaymentGateway* paymentGateway = GatewayFactory::getInstance().getGateway(type);
        PaymentService::getInstance().setGateway(paymentGateway);
        return PaymentService::getInstance().processPayment(req);
    }
};
