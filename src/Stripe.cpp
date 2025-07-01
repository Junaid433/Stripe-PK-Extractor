#include "include/Stripe.hpp"
#include <curl/curl.h>
#include <iostream>
#include "include/nlohmann/json.hpp"

using json = nlohmann::json;

Stripe::Stripe(const std::string& secret_key,
               const std::string& publishable_key,
               const std::string& currency,
               int amount)
    : secret_key_(secret_key),
      publishable_key_(publishable_key),
      currency_(currency),
      amount_(amount)
{}

static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* out) {
    size_t totalSize = size * nmemb;
    out->append(static_cast<char*>(contents), totalSize);
    return totalSize;
}

std::string Stripe::CreateSession() {
    CURL* curl = curl_easy_init();
    std::string response_data;

    if (!curl) {
        std::cerr << "Failed to init CURL\n";
        return "";
    }

    struct curl_slist* headers = nullptr;
    std::string auth_header = "Authorization: Bearer " + secret_key_;
    headers = curl_slist_append(headers, auth_header.c_str());
    headers = curl_slist_append(headers, "Content-Type: application/x-www-form-urlencoded");

    std::string post_fields =
        "payment_method_types[]=card&"
        "line_items[0][price_data][currency]=" + currency_ + "&"
        "line_items[0][price_data][product_data][name]=T-shirt&"
        "line_items[0][price_data][unit_amount]=" + std::to_string(amount_) + "&"
        "line_items[0][quantity]=1&"
        "mode=payment&"
        "success_url=https://google.com/success?session_id={CHECKOUT_SESSION_ID}&"
        "cancel_url=https://google.com/cancel";

    curl_easy_setopt(curl, CURLOPT_URL, "https://api.stripe.com/v1/checkout/sessions");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post_fields.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_data);

    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        std::cerr << "CURL error: " << curl_easy_strerror(res) << "\n";
    }

    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);

    return response_data;
}

std::string Stripe::CreatePaymentMethod() {
    CURL* curl = curl_easy_init();
    std::string response_data;

    if (!curl) {
        std::cerr << "Failed to init CURL\n";
        return "";
    }

    struct curl_slist* headers = nullptr;
    std::string auth_header = "Authorization: Bearer " + publishable_key_;
    headers = curl_slist_append(headers, auth_header.c_str());
    headers = curl_slist_append(headers, "Content-Type: application/x-www-form-urlencoded");
    headers = curl_slist_append(headers, "User-Agent: Mozilla/5.0");

    std::string post_fields =
        "type=card&"
        "card[number]=374769010283784&"
        "card[exp_month]=12&"
        "card[exp_year]=2050&"
        "card[cvc]=6450";

    curl_easy_setopt(curl, CURLOPT_URL, "https://api.stripe.com/v1/payment_methods");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post_fields.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_data);

    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        std::cerr << "CURL error: " << curl_easy_strerror(res) << "\n";
    }

    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);
    return response_data;
}
