# Lab 22: Strava Activity Filter

CS210 Lesson 22: Command-Line Arguments and the Outside Environment

## What you're building

A command-line program that filters a Strava-style activity log. Given a CSV of activities (date, type, distance, duration, heart rate), your program selects entries matching the cadet's filters and prints them.

Usage:

```
./strava_filter <logfile>                            # print all activities
./strava_filter <logfile> <type>                     # filter by activity type
./strava_filter <logfile> <type> <min_distance>      # filter by type AND distance
```

Examples:

```
$ ./strava_filter data/activities.csv
2026-04-01  run  3.20km  18min  HR 145
2026-04-02  run  5.10km  29min  HR 148
... (and so on for every row) ...
30 activities matched.

$ ./strava_filter data/activities.csv run 10.0
2026-04-11  run  10.50km  58min  HR 153
2026-04-19  run  12.10km  68min  HR 154
2026-04-30  run  15.20km  82min  HR 155
3 activities matched.
```

If `STRAVA_QUIET` is set in the environment, suppress the summary line:

```
$ STRAVA_QUIET=1 ./strava_filter data/activities.csv run 10.0
2026-04-11  run  10.50km  58min  HR 153
2026-04-19  run  12.10km  68min  HR 154
2026-04-30  run  15.20km  82min  HR 155
```

## Files

- `strava_filter.h`: provided, **do not modify**. Defines the structs and the function prototypes.
- `main.c`: provided, **do not modify**. Calls your three functions in the right order.
- `strava_filter.c`: **you implement** the three functions: `parse_args`, `load_log`, `apply_filter`.
- `Makefile`: written by you. `make` to build, `make clean` to clean.
- `data/activities.csv`: sample data for local testing, one activity per line with no header row. Other CSVs in the same format also work.

## How to work the lab

1. Open `strava_filter.h` and read the comment above each function. Every behavior is documented there. In particular, the return values:
   - `parse_args` returns 0 when the arguments are usable and a non-zero value on a usage error (wrong number of arguments, or a minimum distance that is not a valid non-negative number).
   - `load_log` returns the number of activities it loaded. If the file cannot be opened, it should return -1 instead — that is how `main.c` knows to print the open-failure message. If the file holds more rows than the caller has room for, stop at the caller's limit and return that count; running out of room is not an error.
   - `apply_filter` returns the number of entries it printed.
2. Open `main.c` and see how the three functions are wired together.
3. Open `strava_filter.c` and implement the three functions in order: `parse_args`, then `load_log`, then `apply_filter`. The TODOs in the file point at the spec.
4. Build with `make`. Compile errors should be zero before you run.
5. Run the self-tests below.

## Check before you submit

Run these six commands and check the output against what is listed. The autograder runs the same six situations, so if these look right you are in good shape:

1. `./strava_filter`: prints `usage: ...` to stderr; exit code is non-zero.
2. `./strava_filter data/activities.csv`: prints all 30 rows plus the summary line; exit code 0.
3. `./strava_filter data/activities.csv run`: prints 15 rows of type `run` plus summary.
4. `./strava_filter data/activities.csv run 10.0`: prints 3 rows: dates 04-11, 04-19, 04-30.
5. `./strava_filter no_such_file.csv`: prints the open-failure message; exit code is non-zero.
6. `STRAVA_QUIET=1 ./strava_filter data/activities.csv run 10.0`: prints 3 rows, no summary.

## The Makefile (you write it)

Worth **0.20 of this lab**. Since Lesson 7 you have known how to write a Makefile, and from Lesson 9 on you write one for every lab. The stub lists what the project contains; you write the file. Open `Makefile` and fill it in before you start on the C.

The autograder checks that your Makefile:

- builds an executable named `strava_filter`
- compiles each `.c` to its own `.o` and links them
- puts `-Wall` and `-Werror` in `CFLAGS`
- has a `clean` rule that removes `strava_filter` and the `.o` files
- does nothing on a second `make` when nothing has changed

Put `-g` in `CFLAGS` too. It is not graded, but it is what lets `gdb` and `valgrind` show you file names and line numbers instead of `???`.

Recipe lines must start with a real tab character, not spaces. If `make` says `missing separator`, that is what happened. The Lesson 7 lab and the Makefile walkthrough on the Lesson 7 page are the reference.

If your C does not compile, this check is not counted against you: the build points here are about your Makefile, not your code.

## Your tests, with assert

`test_lab22.c` has the Lesson 18 shape: no `main`, one static function per test, and one public `run_all_tests()` (prototype in the provided `tests.h`) that calls every test in order. The provided block at the top of `main` calls it when the program is built with `-D RUNTESTS`, which is what the provided `make run_tests` rule does; leave that block, `tests.h`, and the rule as they are, and keep `test_lab22.c` out of your object list. Nothing is written for you this time; the header comment in the file lists the cases worth covering. You need **at least 4 `assert` calls, on `parse_args` and `apply_filter`**, one static function per case, named for what it checks, with a comment naming the edge case, and every test listed in `run_all_tests()` or it never runs. `make run_tests && ./run_tests` runs them; `make test` does too. The six commands above check the whole program; your tests check the functions.

The three rules from Lesson 18 still apply: work the expected value out by hand first; a test that fails against correct code is itself the bug (the autograder runs your file against the instructor's `strava_filter.c`); a failing assert skips everything below it, including your frees.

Graded as 0.2 of the lab: 0.1 for a test file that exercises the named functions (comments stripped) and 0.1 for those tests holding against the reference module. The second half is not scored until the first passes.

<!-- visible-tests:begin (generated) -->
## Visible tests

A subset of the checks the autograder runs is shipped in `tests/visible/`. Run them any time with

```
make test
```

(`make test` just runs `bash run_local.sh`, so either works. The script builds with your Makefile, falling back to a direct gcc build while your Makefile is unfinished, so you can always check your C.)

The autograder runs more cases than these. Passing the visible tests is necessary, not sufficient.

`make test` also compiles and runs your `test_lab22.c` and reports whether every assert held. The autograder grades that file separately (see "Your tests, with assert" above).
<!-- visible-tests:end -->

## Grading

This lab is worth 2 points, autograded through Gradescope. Point breakdown:

- Documentation statement (see below): 0.10
- Builds cleanly with `make` (no warnings, no errors): 0.15
- Functional correctness across 8 test cases: 1.245
- Discipline, measured (memory and resource habits): 0.105. The autograder runs one full filter pass under valgrind; memory errors or leaks cost these points. Close every file you open.
- Your Makefile builds the project (you write it): 0.20
- Your tests, with assert (you write them; see above): 0.20

### Documentation statement (5% of this lab)

The comment header of `strava_filter.c` must contain a documentation statement: a line beginning with `Documentation:` that lists ALL outside help you received on this lab. That covers GenAI tools (documented per the course GenAI policy), internet sources such as Stack Overflow, and help from any person (classmates, friends, EI with your instructor). If you received no outside help, write exactly:

    Documentation: None

Keep the statement on the same line as `Documentation:`. Leaving the template placeholder in place scores zero for this component.

## A few rules to keep you out of trouble

- Errors and usage messages go to **stderr**, not stdout. Use `fprintf(stderr, ...)`.
- Return **`EXIT_FAILURE`** (or any non-zero value) on failure, not 0.
- Always check `argc` **before** indexing `argv[N]`.
- `getenv` returns NULL when the variable is not set. Check before dereferencing.
- Close every file you open (`fclose` whatever you `fopen`'d).
- Compile flags are `-Wall -Werror`. Warnings break the build.
- Do not modify `strava_filter.h` or `main.c`.
