# UML Class Diagram

```
┌──────────────────────────────────────────────────────────────────────────────────┐
│                          STRATEGY PATTERN                                        │
│                                                                                  │
│    ┌─────────────────────┐                                                       │
│    │   <<interface>>     │                                                       │
│    │   BankingSystem     │                                                       │
│    │─────────────────────│                                                       │
│    │ + processPayment()  │                                                       │
│    └─────────────────────┘                                                       │
│              △                                                                   │
│    ┌─────────┴──────────┐                                                        │
│    │                    │                                                        │
│  ┌────────────────┐   ┌─────────────────────┐                                   │
│  │PaytmBanking    │   │ RazorpayBanking      │                                  │
│  │System          │   │ System               │                                  │
│  │──────────────  │   │──────────────────────│                                  │
│  │+processPayment │   │ + processPayment()   │                                  │
│  │ (80% success)  │   │   (90% success)      │                                  │
│  └────────────────┘   └──────────────────────┘                                  │
└──────────────────────────────────────────────────────────────────────────────────┘

┌──────────────────────────────────────────────────────────────────────────────────┐
│                       TEMPLATE METHOD PATTERN                                    │
│                                                                                  │
│    ┌─────────────────────────────────────────┐                                   │
│    │         <<abstract>>                    │                                   │
│    │         PaymentGateway                  │                                   │
│    │─────────────────────────────────────────│                                   │
│    │ # bankingSystem: BankingSystem*         │                                   │
│    │─────────────────────────────────────────│                                   │
│    │ + processPayment()   [template method]  │                                   │
│    │ # validatePayment()  [abstract]         │                                   │
│    │ # initiatePayment()  [abstract]         │                                   │
│    │ # confirmPayment()   [abstract]         │                                   │
│    └─────────────────────────────────────────┘                                   │
│                         △                                                        │
│             ┌───────────┼──────────────────────┐                                 │
│             │           │                      │                                 │
│   ┌─────────────────┐  ┌──────────────────┐  ┌────────────────────┐             │
│   │  PaytmGateway   │  │ RazorpayGateway  │  │PaymentGatewayProxy │             │
│   │─────────────────│  │──────────────────│  │────────────────────│             │
│   │+validatePayment │  │+validatePayment  │  │- realGateway*      │             │
│   │+initiatePayment │  │+initiatePayment  │  │- maxRetries: int   │             │
│   │+confirmPayment  │  │+confirmPayment   │  │────────────────────│             │
│   │                 │  │                  │  │+processPayment()   │             │
│   │uses ──────────► │  │uses ──────────►  │  │  [adds retry loop] │             │
│   │PaytmBankingSystem  │RazorpayBankingSystem  └────────────────────┘            │
│   └─────────────────┘  └──────────────────┘         │ wraps                     │
│                                                      └──► any PaymentGateway    │
└──────────────────────────────────────────────────────────────────────────────────┘

┌──────────────────────────────────────────────────────────────────────────────────┐
│                   SINGLETON + FACTORY METHOD PATTERN                             │
│                                                                                  │
│   ┌────────────────────────┐                                                     │
│   │  GatewayFactory        │◄── Singleton                                       │
│   │────────────────────────│                                                     │
│   │ - instance (static)    │                                                     │
│   │────────────────────────│                                                     │
│   │ + getInstance()        │                                                     │
│   │ + getGateway(type)     │──creates──► PaymentGatewayProxy(PaytmGateway, 3)   │
│   └────────────────────────┘             PaymentGatewayProxy(RazorpayGateway,1) │
│                                                                                  │
│   ┌────────────────────────┐                                                     │
│   │  PaymentService        │◄── Singleton                                       │
│   │────────────────────────│                                                     │
│   │ - gateway: Gateway*    │                                                     │
│   │────────────────────────│                                                     │
│   │ + setGateway(g)        │                                                     │
│   │ + processPayment(req)  │──delegates──► gateway->processPayment(req)         │
│   └────────────────────────┘                                                     │
│                                                                                  │
│   ┌────────────────────────┐                                                     │
│   │  PaymentController     │◄── Singleton  (client entry point)                 │
│   │────────────────────────│                                                     │
│   │ + handlePayment(       │──► GatewayFactory::getGateway()                   │
│   │    type, request)      │──► PaymentService::setGateway()                   │
│   └────────────────────────┘──► PaymentService::processPayment()               │
└──────────────────────────────────────────────────────────────────────────────────┘

Payment flow (happy path):
──────────────────────────
Client
  └─► PaymentController::handlePayment(PAYTM, req)
        └─► GatewayFactory::getGateway(PAYTM)
              └─► new PaymentGatewayProxy(new PaytmGateway(), 3)
        └─► PaymentService::setGateway(proxy)
        └─► PaymentService::processPayment(req)
              └─► PaymentGatewayProxy::processPayment(req)   [attempt 1]
                    └─► PaytmGateway::processPayment(req)    [template method]
                          ├─ validatePayment()   → amount > 0, currency == INR
                          ├─ initiatePayment()   → PaytmBankingSystem::processPayment()
                          └─ confirmPayment()    → always true
```
