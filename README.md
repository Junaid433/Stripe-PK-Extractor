# GetPK - Stripe Secret Key to Publishable Key Extractor & Validator

GetPK is a C++ tool designed to generate Stripe checkout sessions from secret keys, extract the associated publishable key, and optionally validate the publishable key by creating a test payment method. It supports both command-line usage and an HTTP server mode with JSON API responses.

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

