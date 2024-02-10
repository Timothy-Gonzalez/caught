<div align="center">

# Caught

[![Version](https://img.shields.io/github/v/tag/Timothy-Gonzalez/caught?style=for-the-badge&label=Version&color=yellow&cacheSeconds=300)](https://github.com/Timothy-Gonzalez/caught)
[![Build Status](https://img.shields.io/github/actions/workflow/status/Timothy-Gonzalez/caught/ci.yaml?branch=main&style=for-the-badge&cacheSeconds=300)](https://github.com/Timothy-Gonzalez/caught/actions/workflows/ci.yaml?query=branch%3Amain)
[![Docs](https://img.shields.io/badge/Docs-Caught-blue?style=for-the-badge&cacheSeconds=300)](docs/getting-started.md)
[![License](https://img.shields.io/badge/License-MIT-blue?style=for-the-badge&color=%23dc2bff&cacheSeconds=300)](LICENSE.txt)

A lightweight & simple C testing library

</div>

# Features

- No setup - just #include
- Easy to use syntax
- Beautiful colored output that just works
- Easily expand for custom types

# Example test

```c
int factorial(n)
{
    return n <= 1 ? 1 : factorial(n - 1) * n;
}

TEST("factorial")
{
    EXPECT_INT(factorial(1), ==, 1);
    EXPECT_INT(factorial(2), ==, 2);
    EXPECT_INT(factorial(3), ==, 6);
    EXPECT_INT(factorial(4), ==, 24);
    EXPECT_INT(factorial(5), ==, 120);
}
```

# Installation

1. Download the `caught.h` and `caught.c` files from the [latest release](https://github.com/Timothy-Gonzalez/caught/releases/latest)
2. In your test files, simply `#include "caught.h"`
3. Modify your compiler to compile your tests & `caught.c`
4. See [Usage](#usage) for how to begin!

> **Note:** your code cannot include a `main` function as Caught needs to define it.

<details>
<summary>One file solution</summary>
If you only want to have one test file, you can put the contents of `caught.c` at the end of `caught.h`
and just include that instead, without modifying your compiler to compile `caught.c`.

This works because there cannot be duplicate definitions of `main`.
</details>

<details>
<summary>From source</summary>
If you want, you can always download the full source and use that directly.

However, you will need to make sure you include the actual path to `caught.h` (`#include "path/to/caught.h"`) and
update your compiler to compile all of Caught's files.
</details>


# Usage

Please see the following docs:
- [Getting Started](docs/getting-started.md)
- [API Reference](docs/api-reference.md)
- [Custom Assertions](docs/custom-assertions.md)
