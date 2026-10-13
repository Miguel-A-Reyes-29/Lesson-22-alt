/* ============================================================
 * tests.h  --  The one public function of your test file.
 *
 * CS210 (labs 18 and up). Provided; do not modify. The program's main
 * includes this so that, when it is built with -D RUNTESTS (make
 * run_tests), main can hand control to your tests and stop. Your test
 * functions stay static inside the test file: nothing else needs them,
 * so nothing else can see them.
 * ============================================================ */

#ifndef TESTS_H
#define TESTS_H

/**
 * @brief runs every test in your test file, in order, and prints a line per test
 * @return nothing; a failing assert aborts the program before this returns
 */
void run_all_tests(void);

#endif /* TESTS_H */
