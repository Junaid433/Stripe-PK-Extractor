#ifndef STRIPE_HPP
#define STRIPE_HPP

#include <string>

class Stripe{
    public:
        explicit Stripe(
                const std::string& secret_key,
                const std::string& publishable_key,
                const std::string& currency = "usd", 
                int amount = 100
            );
        std::string CreateSession();
        std::string CreatePaymentMethod();
    private:
        std::string secret_key_;
        std::string publishable_key_;
        std::string currency_;
        int amount_;
};

#endif