#pragma once

#include <string>
#include <unordered_map>
#include <iostream>

// ----------------------------
// CurrencyConverter (Singleton)
// Uses fixed integer conversion rates relative to a base unit (INR).
// All rates are integers — e.g. 1 USD = 83 INR, 1 EUR = 90 INR.
//
// Supported currencies: INR, USD, EUR, GBP, JPY
// ----------------------------
class CurrencyConverter {
private:
    static CurrencyConverter instance;

    // Rate table: how many INR is 1 unit of this currency
    // INR is the internal base (rate = 1)
    std::unordered_map<std::string, int> toINR = {
        { "INR", 1   },
        { "USD", 83  },
        { "EUR", 90  },
        { "GBP", 105 },
        { "JPY", 1   }   // 1 JPY ≈ 1 INR (rounded for simplicity)
    };

    CurrencyConverter() = default;
    CurrencyConverter(const CurrencyConverter&) = delete;
    CurrencyConverter& operator=(const CurrencyConverter&) = delete;

public:
    static CurrencyConverter& getInstance() {
        return instance;
    }

    bool isSupported(const std::string& currency) const {
        return toINR.count(currency) > 0;
    }

    // Convert `amount` from `from` currency to `to` currency.
    // Uses integer arithmetic: convert to INR first, then to target.
    // Returns -1 if either currency is unsupported.
    int convert(int amount, const std::string& from, const std::string& to) const {
        if (!isSupported(from) || !isSupported(to)) {
            std::cout << "[CurrencyConverter] Unsupported currency: "
                      << (!isSupported(from) ? from : to) << "\n";
            return -1;
        }
        if (from == to) return amount;

        // amount (in `from`) → INR → `to`
        int inINR = amount * toINR.at(from);
        int result = inINR / toINR.at(to);
        return result;
    }

    void printRates() const {
        std::cout << "\n--- Exchange Rates (base: INR) ---\n";
        for (const auto& [curr, rate] : toINR) {
            std::cout << "  1 " << curr << " = " << rate << " INR\n";
        }
    }
};
