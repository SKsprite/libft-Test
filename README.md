# libft unit tests

Unit tests for the 34 libft functions in Part 1 and Part 2.

- **Part 1** (`isalpha` … `strdup`): each `ft_*` function is called with
  the same inputs as the real function from libc (or from libbsd for
  `strlcpy`, `strlcat` and `strnstr`). The tests then assert that both
  give the same return value and leave the same bytes in memory.
- **Part 2** (`substr` … `putnbr_fd`): libc has no equivalent, so the
  expected results are written out in the tests by hand, based on the
  subject. `itoa` and `putnbr_fd` are checked against `snprintf("%d")`.
  The `*_fd` functions write to a temporary file, which the test reads
  back byte by byte.

## Requirements

- `cc` (clang or gcc) and `make`
- **libbsd**: the tests link with `-lbsd` to get the real `strlcpy`,
  `strlcat` and `strnstr`. Without it, install it with
  `sudo apt install libbsd-dev`.
- `valgrind` (optional), for `make valgrind`

## Layout

```
Milestone0/
├── libft/              <- your library (must build libft.a with `make`)
└── libft Test/
    ├── Makefile
    ├── run_tests.sh    <- builds and runs tests one function at a time
    ├── test.h          <- assertion macros (CHECK, CHECK_INT, CHECK_PTR, ...)
    ├── test_utils.c    <- runner: fork per case, timeout, crash detection
    └── tests/
        ├── test_ft_atoi.c
        ├── test_ft_bzero.c
        └── ... one file per libft function
```

The tests expect the library in `../libft`. If yours is somewhere else,
pass `LIBFT_DIR`:

```sh
make LIBFT_DIR=/path/to/libft
```

## Usage

Run these from inside `libft Test/`:

| Command | What it does |
|---|---|
| `make` / `make test` | Builds `libft.a` (runs `make` in `../libft`), then builds and runs every test |
| `make part1` | Tests only Part 1, the libc functions (`isalpha` … `strdup`) |
| `make part2` | Tests only Part 2, the additional functions (`substr` … `putnbr_fd`) |
| `make strlen` or `make ft_strlen` | Tests one function only |
| `./run_tests.sh atoi memmove` | Tests several functions you pick |
| `make valgrind` | Runs everything under valgrind to find invalid reads/writes and leaks |
| `make valgrind_part1` / `make valgrind_part2` | The same valgrind run, for one part only |
| `make build` | Only compiles the test binaries (into `bin/`) |
| `make clean` | Deletes `bin/` |

You can run any binary on its own after it is built, e.g. `./bin/test_ft_strlcat`.

## Reading the output

```
ft_strchr
  [OK] found at start, middle, end
  [OK] not found -> NULL
  [KO] c = '\0' returns pointer to terminator
      -> tests/test_ft_strchr.c:11: s="Hello" c=0: expected "", got NULL
  ...
ft_strchr: 5/7 passed
```

- **[OK]**: the case passed.
- **[KO]**: the case failed. The lines starting with `->` give the test file
  and line, the input, the expected result and what your function returned.
  Non-printable bytes appear as `\xNN`.
- **[KO] ... -> SEGFAULT / BUS ERROR / ABORT**: your function crashed.
  ABORT usually means a bad `free` or heap corruption.
- **[KO] ... -> TIMEOUT**: the case ran for more than 2 seconds, which usually
  means an infinite loop. Change the limit with `TEST_TIMEOUT=5 make`.
- **[BUILD FAIL]**: the test could not be compiled or linked against your
  `libft.a`. The compiler/linker errors are shown underneath. A common
  cause is a function that is missing from `libft.a` or has the wrong
  name, e.g. `memmove` instead of `ft_memmove`.

Every test case runs in its own child process. A crash or infinite loop
fails only that case, and the remaining tests still run. The summary at the
end lists which functions passed, which failed and which did not build.
`make` exits non-zero if anything failed.

## How results are compared

- **`is*` functions**: libc may return any non-zero value for "true" (e.g. 1024).
  So only true/false is compared, not the exact number.
- **`strncmp` / `memcmp`**: the standard only defines the **sign** of the
  result, so only the sign is compared.
- **Functions that write memory** (`memset`, `bzero`, `memcpy`, `memmove`,
  `strlcpy`, `strlcat`): the destination is pre-filled with `'X'`, and the
  **whole buffer** is compared against libc's result. Writing past the end
  or forgetting the `'\0'` therefore shows up.
- **Functions that return pointers** (`strchr`, `strrchr`, `memchr`,
  `strnstr`): the returned pointer must be the **exact same address** libc
  returns, not just point at the same text.

## Edge cases covered

### Part 1

| Function | Edge cases |
|---|---|
| `isalpha` `isdigit` `isalnum` `isprint` | every value from EOF (-1) to 255, the characters on each side of every range (`@ [ \` {`, `/ :`), `\t\n\v\f\r` are not printable, out-of-range ints (`'a'+256`, `INT_MIN`, `INT_MAX`) |
| `isascii` | -1..255, 127/128, negative and huge ints |
| `toupper` `tolower` | -1..255, characters on each side of each range, values that must come back unchanged |
| `strlen` | `""`, single char, embedded `'\0'`, high bytes, 100 000 chars |
| `memset` | `n = 0`, destination full of `'\0'` (must not stop at NUL), `c` converted to unsigned char (`'A'+256`, `-1`), 1 MiB |
| `bzero` | `n = 0`, embedded `'\0'`, first byte already `'\0'`, 1 MiB |
| `memcpy` | `n = 0`, copies past `'\0'`, high bytes, return value, `NULL, NULL, 0`, struct, 1 MiB |
| `memmove` | overlap both ways (dst > src must copy backwards), `dst == src`, `n = 0`, `NULL, NULL, 0`, 1 MiB overlapping |
| `strlcpy` | `size = 0` (writes nothing), `size = 1`, `size == len`, `size == len+1`, truncation, empty src, return value is always `strlen(src)` |
| `strlcat` | `size = 0`, `size < strlen(dst)`, `size == strlen(dst)` and `+1`, exact fit, empty dst/src, dst with no `'\0'` within `size` |
| `strchr` `strrchr` | `c = '\0'` returns the terminator, not found, empty string, match only at `s[0]`, first vs last occurrence, `c` converted to `char` (`256`, `-1`, `255`), every `c` from -1 to 256 |
| `strncmp` | `n = 0`, difference after `n`, `n` larger than the strings (must stop at `'\0'`), `n = SIZE_MAX`, prefix strings, unsigned compare (`'\200'` vs `'\0'`) |
| `memchr` | `n = 0`, finding `'\0'`, searching past `'\0'`, `c = -1` finds `0xff`, every byte value |
| `memcmp` | `n = 0`, does not stop at `'\0'`, unsigned compare, difference only in the last byte, int arrays |
| `strnstr` | empty `little` returns `big`, empty `big`, `len = 0`, `len` cuts the match short, `len` larger than `big` (incl. `SIZE_MAX`), partial match before the real one (`"aaab"`/`"aab"`) |
| `atoi` | all 6 whitespace chars, `+`/`-`, double signs (`--42`, `+-42`), sign separated by a space, junk after the number, no digits, leading zeros, `INT_MAX`, `INT_MIN` |
| `calloc` | memory really zeroed (even when malloc reuses a dirty chunk), `0` arguments can be passed to `free()`, `nmemb * size` overflow returns `NULL`, 1 MiB, many small blocks |
| `strdup` | `""`, new pointer (not the original), independent copy, embedded `'\0'`, high bytes, 100 000 chars, lengths 0..63 |

### Part 2

| Function | Edge cases |
|---|---|
| `substr` | `len = 0`, `len` past the end (clipped), `len = SIZE_MAX` (must not malloc that much), `start == strlen(s)`, `start > strlen(s)` incl. `UINT_MAX` returns `""` (not NULL), empty `s`, result is a separate copy |
| `strjoin` | empty `s1`, empty `s2`, both empty returns `""`, same pointer for both, inputs not modified, high bytes, 100 000 chars |
| `strtrim` | multi-char set in any order, middle chars kept, trim only start/only end, everything trimmed returns `""` (not NULL), empty set, empty `s1`, `" \n\t"` set, one char left |
| `split` | repeated delimiters (no empty words), leading/trailing delimiters, no delimiter, empty string returns `{NULL}` (not NULL), only delimiters, `c = '\0'`, other/non-ASCII delimiters, words are separate allocations, 1000 words. Every result is freed the way the subject describes |
| `itoa` | `0`, -9..9, powers of ten ±1, `INT_MAX`, `INT_MIN`, a sweep of -10000..10000 and near the limits, new string on every call |
| `strmapi` | `f` gets index 0, 1, 2… once per char in order, uses the index, empty string (f never called, returns `""`), original not modified, 100 000 chars |
| `striteri` | modifies in place, `f` gets `&s[i]` (not a copy) and the right index, empty string, stops at `'\0'` |
| `putchar_fd` | every byte 0..255 (incl. `'\0'`) is exactly 1 byte, calls append, `fd = -1` doesn't crash |
| `putstr_fd` | `""` writes nothing, no newline added, stops at `'\0'`, high bytes, 100 000 chars, `fd = -1` |
| `putendl_fd` | `""` still writes `"\n"`, a string already ending in `'\n'` gets another, several lines, `fd = -1` |
| `putnbr_fd` | `0`, negatives, powers of ten, `INT_MAX`, `INT_MIN`, a sweep of -2000..2000, calls append, `fd = -1` |

Behaviour the subject or the C standard leaves undefined is **not** tested:
`NULL` passed to string functions, and `atoi` overflow beyond `int`.

## Tips

- Some memory bugs don't crash and won't fail a normal run, e.g. `strdup`
  allocating one byte too few. `make valgrind` catches those: any valgrind
  error turns that case into a KO, and the `Invalid write` / `Invalid read`
  report is printed above it.
- Each test is a small, readable C file. To check a case of your own,
  write another `static void` function in `tests/test_ft_<name>.c` and
  register it in `main()` with `run_case("description", fn)`.
