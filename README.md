````markdown
# Stripe-PK-Extractor - Stripe Secret Key to Publishable Key Extractor & Validator

Stripe-PK-Extractor is a C++ tool designed to generate Stripe checkout sessions from secret keys, extract the associated publishable key, and optionally validate the publishable key by creating a test payment method. It supports both command-line usage and an HTTP server mode with JSON API responses.

---

## Table of Contents

- [Introduction](#introduction)  
- [Features](#features)  
- [Installation](#installation)  
- [Usage](#usage)  
  - [Command-line Interface (CLI)](#command-line-interface-cli)  
  - [HTTP Server Mode](#http-server-mode)  
- [Command-Line Arguments](#command-line-arguments)  
- [Output Formats](#output-formats)  
- [Examples](#examples)  
- [Development & Build](#development--build)  
- [Dependencies](#dependencies)  
- [Project Structure](#project-structure)  
- [Contributing](#contributing)  
- [License](#license)  
- [Contact](#contact)

---

## Introduction

Stripe uses two main types of API keys:

- **Secret Key (sk_...)** — Full access key used for server-side operations  
- **Publishable Key (pk_...)** — Limited access key used on client-side

GetPK simplifies the extraction of the publishable key linked to a secret key by creating a checkout session and decoding the embedded information. This helps developers confirm and validate keys easily and automate workflows.

---

## Features

- Generate a Stripe checkout session URL using the secret key  
- Extract the publishable key from the checkout session URL  
- Validate the extracted publishable key by creating a dummy payment method  
- Output results in a human-friendly colored format or JSON  
- Silent mode for script automation (`--no-output`)  
- HTTP server mode that exposes the functionality via JSON API  
- Robust error handling and clear user feedback

---

## Installation

### Prerequisites

- C++17 compatible compiler (gcc >= 7, clang, MSVC)  
- CMake 3.10+  
- libcurl development libraries  
- `nlohmann/json` header-only library  
- `cpp-httplib` (for HTTP server support)  

### Build Steps

```bash
git clone https://github.com/yourusername/GetPK.git
cd GetPK
mkdir build && cd build
cmake ..
make
````

The compiled executable will be located at `build/GetPK`.

---

## Usage

### Command-line Interface (CLI)

Basic usage with secret key:

```bash
./GetPK --sk=sk_test_yoursecretkey
```

With verification and JSON output:

```bash
./GetPK --sk=sk_test_yoursecretkey --verify=true --json-output
```

Silent mode (no console output):

```bash
./GetPK --sk=sk_test_yoursecretkey --no-output
```

### HTTP Server Mode

Start the server on port 8080:

```bash
./GetPK --server
```

Send a POST request to `/run` with JSON payload:

```json
{
  "sk": "sk_test_yoursecretkey",
  "verify": true
}
```

Response:

```json
{
    "input": {
        "secret_key": "sk_test_yoursecretkey",
        "verify": true
    },
    "checkout_url": "https://checkout.stripe.com/...",
    "publishable_key": "pk_test_xyz",
    "publishable_key_validation": "success",
    "runtime_seconds": 1
}
```

---

## Command-Line Arguments

| Argument        | Description                                      | Default |
| --------------- | ------------------------------------------------ | ------- |
| `--sk=`         | Stripe Secret Key (required)                     | (none)  |
| `--verify=`     | Validate publishable key (`true` or `false`)     | `true`  |
| `--json-output` | Output results in JSON format                    | `false` |
| `--no-output`   | Suppress console output (implies no JSON)        | `false` |
| `--server`      | Run in HTTP server mode, listens on 0.0.0.0:8080 | `false` |

---

## Output Formats

### Human-readable (default)

* Color-coded output with clear labels
* Error messages in red
* Success messages in green

### JSON (`--json-output`)

* Pretty-printed JSON
* Useful for scripting and automation

---

## Examples

### Example 1: Basic CLI usage

```bash
./GetPK --sk=sk_test_4eC39HqLyjWDarjtT1zdp7dc
```

Output:

```
[Input] Secret Key: sk_test_4eC39HqLyjWDarjtT1zdp7dc
[Input] Verify: true

[Output] Checkout URL: https://checkout.stripe.com/...
[Output] Publishable Key: pk_test_TYooMQauvdEDq54NiTphI7jx
[Output] Publishable Key Validation: Success (Valid Publishable Key)

[Info] Program finished in 2s
```

### Example 2: Using JSON output

```bash
./GetPK --sk=sk_test_4eC39HqLyjWDarjtT1zdp7dc --json-output
```

Output:

```json
{
    "input": {
        "secret_key": "sk_test_4eC39HqLyjWDarjtT1zdp7dc",
        "verify": true
    },
    "checkout_url": "https://checkout.stripe.com/...",
    "publishable_key": "pk_test_TYooMQauvdEDq54NiTphI7jx",
    "publishable_key_validation": "success",
    "runtime_seconds": 2
}
```

### Example 3: Running as HTTP server

```bash
./GetPK --server
```

Then POST JSON data to `http://localhost:8080/run`.

---

## Development & Build

### Dependencies

* **libcurl**: Handles HTTP requests to Stripe
* **nlohmann/json**: JSON parsing and serialization
* **cpp-httplib**: Lightweight HTTP server implementation (optional)

### Build

Run these commands:

```bash
mkdir build && cd build
cmake ..
make
```

You can add additional compiler flags or link libraries in `CMakeLists.txt`.

---

## Project Structure

```
GetPK/
├── CMakeLists.txt
├── include/
│   ├── Stripe.hpp
│   ├── Algorithm.hpp
│   └── nlohmann/json.hpp
├── src/
│   ├── main.cpp
│   ├── Stripe.cpp
│   └── Algorithm.cpp
├── README.md
└── LICENSE
```

---

## Contributing

Contributions are welcome! Please open issues or submit pull requests for:

* Bug fixes
* Feature requests
* Documentation improvements

Before contributing, please ensure:

* Code compiles without warnings
* Follow existing coding style
* Include relevant tests if applicable

---

## Contact

Created and maintained by [Junaid433](https://github.com/Junaid433).
Feel free to reach out for questions or collaboration.

```
```
