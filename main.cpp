#include <iostream>
#include <string>
#include <cstring>
#include <chrono>
#include "httplib.h"
#include "nlohmann/json.hpp"
#include "Algorithm.hpp"
#include "Stripe.hpp"

#define RED "\033[31m"
#define GREEN "\033[32m"
#define CYAN "\033[36m"
#define RESET "\033[0m"
#define BOLD "\033[1m"

nlohmann::json process_sk(const std::string& secret_key, bool verify, bool json_output = true, bool no_output = true) {
    auto start = std::chrono::steady_clock::now();
    nlohmann::json output_json;
    output_json["input"]["secret_key"] = secret_key;
    output_json["input"]["verify"] = verify;

    if (secret_key.empty()) {
        output_json["error"] = "Secret Key required to create checkout session.";
        return output_json;
    }

    Stripe stripe(secret_key, "");
    std::string cs_json = stripe.CreateSession();

    try {
        auto json_obj = nlohmann::json::parse(cs_json);
        if (json_obj.contains("error")) {
            output_json["error"] = json_obj["error"]["message"];
            return output_json;
        }

        std::string url = json_obj["url"];
        output_json["checkout_url"] = url;

        auto hash_pos = url.find('#');
        if (hash_pos == std::string::npos) {
            output_json["error"] = "Checkout URL does not contain '#' fragment.";
            return output_json;
        }

        std::string fragment = url.substr(hash_pos + 1);
        std::string urlDecoded = Algorithm::urlDecode(fragment);
        std::string base64Decoded = Algorithm::base64Decode(urlDecoded);
        std::string deobfuscated = Algorithm::xorDecode(base64Decoded, 5);
        std::string apiKey = Algorithm::extractString(deobfuscated, "\"apiKey\":\"", "\"");

        output_json["publishable_key"] = apiKey;

        if (verify) {
            Stripe stripeWithPK(secret_key, apiKey);
            std::string pk_status = stripeWithPK.CreatePaymentMethod();

            try {
                auto pm_json = nlohmann::json::parse(pk_status);
                bool valid = pm_json.contains("id");
                output_json["publishable_key_validation"] = valid ? "success" : "failed";
            } catch (const std::exception& e) {
                output_json["error"] = std::string("Failed to parse payment method response: ") + e.what();
                return output_json;
            }
        }

    } catch (const std::exception& e) {
        output_json["error"] = std::string("Failed to parse session response: ") + e.what();
        return output_json;
    }

    auto end = std::chrono::steady_clock::now();
    auto runtime = std::chrono::duration_cast<std::chrono::seconds>(end - start);
    output_json["runtime_seconds"] = runtime.count();
    return output_json;
}

int main(int argc, char* argv[]) {
    std::string secret_key;
    bool verify = true;
    bool no_output = false;
    bool json_output = false;
    bool start_server = false;

    for (int i = 1; i < argc; ++i) {
        if (strncmp(argv[i], "--sk=", 5) == 0) {
            secret_key = argv[i] + 5;
        } else if (strncmp(argv[i], "--verify=", 9) == 0) {
            std::string val = argv[i] + 9;
            verify = (val == "true" || val == "1");
        } else if (strcmp(argv[i], "--no-output") == 0) {
            no_output = true;
        } else if (strcmp(argv[i], "--json-output") == 0) {
            json_output = true;
            no_output = true;
        } else if (strcmp(argv[i], "--server") == 0) {
            start_server = true;
        }
    }

    if (start_server) {
        httplib::Server svr;

        svr.Post("/run", [](const httplib::Request& req, httplib::Response& res) {
            try {
                auto input = nlohmann::json::parse(req.body);
                std::string sk = input.value("sk", "");
                bool verify = input.value("verify", true);

                auto result = process_sk(sk, verify);
                res.set_content(result.dump(4), "application/json");
            } catch (...) {
                res.status = 400;
                res.set_content(R"({"error":"Invalid JSON"})", "application/json");
            }
        });

        std::cout << GREEN << BOLD << "[Server]" << RESET << " Listening on http://0.0.0.0:8080\n";
        svr.listen("0.0.0.0", 8080);
        return 0;
    }

    nlohmann::json output = process_sk(secret_key, verify, json_output, no_output);

    if (json_output) {
        std::cout << output.dump(4) << std::endl;
    } else if (!no_output) {
        if (output.contains("error")) {
            std::cerr << RED << BOLD << "[Error]" << RESET << " " << output["error"] << "\n";
        } else {
            std::cout << GREEN << BOLD << "[Output] Checkout URL: " << RESET << output["checkout_url"] << "\n";
            std::cout << GREEN << BOLD << "[Output] Publishable Key: " << RESET << output["publishable_key"] << "\n";
            if (verify && output.contains("publishable_key_validation"))
                std::cout << GREEN << BOLD << "[Output] Publishable Key Validation: " << RESET << output["publishable_key_validation"] << "\n";
        }

        std::cout << CYAN << BOLD << "[Info]" << RESET << " Program finished in " << output["runtime_seconds"] << "s\n";
    }

    return 0;
}
