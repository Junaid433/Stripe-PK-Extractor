# Stripe-PK-Extractor

Stripe-PK-Extractor is a C++ command-line utility and web server for securely retrieving Stripe publishable keys from secret keys. It supports verification of keys, JSON output formatting, and a built-in HTTP server endpoint for easy integration.

---

## Features

- Retrieve Stripe checkout session URLs using secret keys.
- Extract and verify Stripe publishable keys.
- Optional JSON formatted output.
- Built-in web server with HTTP POST endpoint for key processing.
- Supports flags to toggle verification and output verbosity.

---

## Requirements

- C++17 compatible compiler (e.g., GCC, Clang)
- libcurl
- pthread
- [cpp-httplib](https://github.com/yhirose/cpp-httplib)
- [nlohmann/json](https://github.com/nlohmann/json)

---

## Build Instructions

```bash
mkdir build
cd build
cmake ..
make
```

---

## Usage

### Command-line options:

```
--sk=<secret_key>         Stripe secret key (required)
--verify=<true|false>     Whether to verify the publishable key (default: true)
--no-output               Suppress normal output
--json-output             Output result as JSON
--server                  Start HTTP server on port 8080
```

### Examples

Run with secret key and verify (default):

```bash
./GetPK --sk=sk_test_abc123 --verify=true
```

Run with JSON output:

```bash
./GetPK --sk=sk_test_abc123 --json-output
```

Start server mode:

```bash
./GetPK --server
```

---

## HTTP Server API

- **Endpoint:** `POST /run`
- **Content-Type:** `application/json`
- **Request JSON:**

```json
{
  "sk": "sk_test_abc123",
  "verify": true
}
```

- **Response JSON:**

```json
{
  "input": {
    "secret_key": "sk_test_abc123",
    "verify": true
  },
  "checkout_url": "https://checkout.stripe.com/...",
  "publishable_key": "pk_test_...",
  "publishable_key_validation": "success",
  "runtime_seconds": 2
}
```

---

## Integration with Python

You can call GetPK executable from Python using subprocess and parse JSON output:

```python
import subprocess
import json

def get_publishable_key(secret_key, verify=True):
    cmd = ['./GetPK', f'--sk={secret_key}', f'--verify={"true" if verify else "false"}', '--json-output']
    result = subprocess.run(cmd, capture_output=True, text=True)
    if result.returncode != 0:
        raise RuntimeError(f"Error: {result.stderr}")
    data = json.loads(result.stdout)
    return data

data = get_publishable_key("sk_test_abc123")
print(data.get("publishable_key"))
```

---

## Author

Junaid Rahman — GitHub: [Junaid433](https://github.com/Junaid433)

---

