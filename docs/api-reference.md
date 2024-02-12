# Other Resources

- [Getting Started](./getting-started.md)
- [API Reference](#api-reference)
- [Custom Assertions](./custom-assertions.md)

# API Reference

- [Operators](#operators)
- [Expect Assertions](#expect-assertions)
- [Basic Assertions](#basic-assertions)
- [Pointer Assertions](#pointer-assertions)
- [Array Assertions](#array-assertions)
- [Exit & Signal Assertions](#exit--signal-assertions)
- [Mocking](#mocking)

## Operators

### `==`, `!=`, `>`, `<`, `>=`, `<=`

The same as the standrd C operators:
- `==`
  - Requires actual to be equal to expected.
- `!=`
  - Requires actual to **not** be equal to expected.
- `>`
  - Requires actual to be greater than to expected.
- `<`
  - Requires actual to be less than to expected.
- `>=`
  - Requires actual to be greater than or equal to expected.
- `<=`
  - Requires actual to be less than or equal to expected.

### `in` and `not in`

Only defined for strings and array elements.

- `in`
  - Requires actual to be in expected.
- `not in`
  - **Inverse of above**: requires actual to **not** be in expected.

Examples:
- `EXPECT_STR("abc", in, "abcdef")`
- `EXPECT_INT_ARRAY_ELEMENT(1, in, {1, 2, 3})`

### `match` and `not match`

Only defined for strings.

- `match`
  - Requires actual to match expected.
- `not match`
  - **Inverse of above**: requires actual to **not** match expected.

Examples:
- `EXPECT_STR("abc", match, "abc")`
- `EXPECT_STR("a a char", match, "a $c char")`
- `EXPECT_STR("a 123 number", match, "a $i number")`
- `EXPECT_STR("abc", not match, "def")`
- See [sample tests](../tests/str.c) for more

Match specifiers are specified with `$`:
- `$i`, `$d` = match a integer (0-9)
- `$c` = match a single character
- `$f` = match a float (0-9, a single . allowed)
- `$a` = match alpha-chars (a-z and A-Z)
- `$w` = match a word (matches anything non-whitespace)
- `$s` = match a string (at least 1 character)
- `$$` = a literal $
- Anything else = literal match
- **Note:** Having two greedy operators like $s without a literal match between them (`$s$s`) will fail

## Expect Assertions

### `EXPECT(expr)`

Equivalent to `EXPECT_BOOL(expr, ==, true)`.
A good drop in replacement for `assert`.

For example: `EXPECT(1 + 2 == 2)`. [See sample tests](../tests/expect.c).

### `EXPECT_NOT(expr)`

The inverse of [`EXPECT`](#expectexpr).
Equivalent to `EXPECT_BOOL(expr, ==, false)`.

For example: `EXPECT_NOT(6 * 9 != 42)`. [See sample tests](../tests/expect.c).

## Basic Assertions

### `EXPECT_PTR(actual, op, expected)`

Compare two ptrs' addresses.

For example: `EXPECT_PTR((void*) 0x3, !=, (void*) 0x2)`. [See sample tests](../tests/ptr.c).

### `EXPECT_BOOL(actual, op, expected)`

Compare two bools.

For example: `EXPECT_BOOL(true, !=, false)`. [See sample tests](../tests/bool.c).

### `EXPECT_CHAR(actual, op, expected)`

Compare two chars.

For example: `EXPECT_CHAR('a', !=, 'b')`. [See sample tests](../tests/char.c).

### `EXPECT_INT(actual, op, expected)`

Compare two ints.

For example: `EXPECT_INT(4, >, 3)`. [See sample tests](../tests/int.c).

### `EXPECT_STR(actual, op, expected)`

Compare two strings.

For example: `EXPECT_STRING("abc", !=, "def")`. [See sample tests](../tests/str.c).

## Pointer Assertions

### `EXPECT_PTR_PTR(actual, op, expected)`

First, checks for `NULL`, then dereferences and compares two ptrs' addresses.

[See sample tests](../tests/ptr.c).

### `EXPECT_BOOL_PTR(actual, op, expected)`

First, checks for `NULL`, then dereferences and compares two bools.

[See sample tests](../tests/bool.c).

### `EXPECT_CHAR_PTR(actual, op, expected)`

First, checks for `NULL`, then dereferences and compares two chars.

[See sample tests](../tests/char.c).

### `EXPECT_INT_PTR(actual, op, expected)`

First, checks for `NULL`, then dereferences and compares two ints.

[See sample tests](../tests/int.c).

### `EXPECT_STR_PTR(actual, op, expected)`

First, checks for `NULL`, then dereferences and compares two strings.

[See sample tests](../tests/str.c).

## Array Assertions

### `EXPECT_PTR_ARRAY(actual, op, expected, length)`

Compare two arrays of ptrs' addresses.
Length of expected array is required.

Inequality operators like `>` are matched between each pair of elements.

For example: `EXPECT_PTR_ARRAY({(void*) 0x3, (void*) 0x2}, !=, {(void*) 0x4, (void*) 0x5}, 3)`. [See sample tests](../tests/ptr.c).

### `EXPECT_BOOL_ARRAY(actual, op, expected, length)`

Compare two arrays of bools.
Length of expected array is required.

Inequality operators like `>` are matched between each pair of elements.

For example: `EXPECT_BOOL_ARRAY({true, false}, !=, {false, false}, 2)`. [See sample tests](../tests/bool.c).

### `EXPECT_CHAR_ARRAY(actual, op, expected, length)`

Compare two arrays of chars.
Length of expected array is required.

Inequality operators like `>` are matched between each pair of elements.

If the arrays are `'\0'` terminated, a length of `-1` may be used.

For example: `EXPECT_CHAR_ARRAY({'a', 'w'}, !=, {'b', 'x'}, 2)`. [See sample tests](../tests/char.c).

### `EXPECT_INT_ARRAY(actual, op, expected, length)`

Compare two arrays of ints.
Length of expected array is required.

Inequality operators like `>` are matched between each pair of elements.

For example: `EXPECT_INT_ARRAY({1, 2}, !=, {4, 5}, 2)`. [See sample tests](../tests/int.c).

### `EXPECT_STR_ARRAY(actual, op, expected, length)`

Compare two arrays of strings.
Length of expected array is required.

If the arrays are `NULL` terminated, a length of `-1` may be used.

For example: `EXPECT_STRING_ARRAY({"abc", "def"}, !=, {"wow", "cool"}, 2)`. [See sample tests](../tests/str.c).

## Exit & Signal Assertions

### `EXPECT_EXIT(status, {code})`

Expects the code contained to exit with a certain status.

**Note:** internally uses fork to make this possible. If your system does not support fork, this will not work.

[See sample tests](../tests/exit.c).

### `EXPECT_SIGNAL(signal, {code})`

Expects the code contained to exit with a certain signal.

**Note:** internally uses fork to make this possible. If your system does not support fork, this will not work.

[See sample tests](../tests/signal.c).

## Mocking

### `MOCK_STDIN(char* input)`

Redirects reads from stdin to read from the input string instead.
Can be restored & read using [`RESTORE_STDIN()`](#restore_stdin).

[See sample tests](../tests/stdin.c).

### `RESTORE_STDIN()`

Undos stdin mock from [`MOCK_STDIN()`](#mock_stdinchar-input). Should be used AFTER stdin has been read.

[See sample tests](../tests/stdin.c).

### `MOCK_STDOUT()`

Redirects anything output to `stdout` (`printf`, `puts`, etc.) into a mock.
Can be restored & read using [`char* RESTORE_STDOUT()`](#char-restore_stdout).

[See sample tests](../tests/stdout.c).

### `char* RESTORE_STDOUT()`

Undos stdout mock from [`MOCK_STDOUT()`](#mock_stdout), and returns a string of everything redirected into the mock.

**Note:** the returned string is allocated and must be free'd.

[See sample tests](../tests/stdout.c).
