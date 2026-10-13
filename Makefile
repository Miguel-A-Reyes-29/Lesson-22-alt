# ============================================================
# CS210 Lesson 22 Lab - Makefile
#
# YOU WRITE THIS FILE.
#
# You learned how in Lesson 7. From Lesson 9 on, building the project is
# part of every lab. This file is worth 0.2 of this lab's 2 points,
# not enough to sink your grade. But the autograder does check that it
# works, and you cannot build, run, or debug your code without it.
#
# Recipe lines MUST start with a real TAB character. Spaces will not work,
# and many editors quietly turn tabs into spaces. If make says
# "missing separator", that is what happened.
# ============================================================
#
# WHAT THE AUTOGRADER CHECKS
#   1. The default target (the first one in the file) builds an
#      executable named `strava_filter`.
#   2. Compile each .c to its own .o, then link the .o files together:
#      main.o  strava_filter.o
#      One gcc command listing every .c at once does not count.
#   3. CFLAGS contains -Wall and -Werror.
#   4. A `clean` rule removes `strava_filter` and the .o files.
#   5. Running `make` twice in a row does nothing the second time,
#      because nothing changed.
#
# FILES IN THIS PROJECT
#   main.c           includes strava_filter.h, tests.h
#   strava_filter.c  includes strava_filter.h
#
# VALGRIND:
#   This lab allocates on the heap, and the autograder runs the program
#   under valgrind. If you want make to remember for you, add a target
#   of your own.
#
# NOT CHECKED, BUT DO IT ANYWAY
#   Put -g in CFLAGS alongside -Wall -Werror. It costs nothing and it is
#   what lets gdb and valgrind show you file names and line numbers
#   instead of ???. Every course example uses -Wall -Werror -g.
#
# Anything else is up to you. A `run` target that builds and runs the
# program is a common convenience; it is not required.
# ============================================================
CC=gcc
CFLAGS=-Wall -Werror -g
strava_filter:main.o strava_filter.o
	$(CC) main.o strava_filter.o -o strava_filter
main.o: main.c strava_filter.h tests.h
	$(CC) $(CFLAGS) -c main.c -o main.o
strava_filter.o:strava_filter.c
	$(CC) $(CFLAGS) -c strava_filter.c -o strava_filter.o

# TODO: write the whole file:
#   variables, the default target, the object-file rules, and clean.
#   The contract is above; the shape of every rule is in your
#   Lesson 7 lab and the Makefile walkthrough on the Lesson 7 page.

# ---- Your tests (PROVIDED - keep this; write your rules ABOVE it) ----
# `make run_tests` compiles every source, your test file included, in ONE
# gcc command with -D RUNTESTS, so main hands off to run_all_tests() (the
# Lesson 18 pattern). It does not reuse your .o files on purpose: those
# were compiled without the macro, and make cannot tell the difference.
# Have your clean rule remove run_tests too.
run_tests: main.c strava_filter.c test_lab22.c tests.h
	gcc -Wall -Werror -g -D RUNTESTS -o run_tests main.c strava_filter.c test_lab22.c
clean:
	-rm *.o strava_filter
# ---- Visible tests (PROVIDED - keep this; write your rules ABOVE it) ----
# `make test` runs the visible tests: run_local.sh builds your code,
# falling back to a direct gcc build while your Makefile is unfinished,
# so it works from your very first run.
test:
	bash run_local.sh
.PHONY: test
