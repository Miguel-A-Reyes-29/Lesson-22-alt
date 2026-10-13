/*
 * main.c
 *
 * CS210 Lesson 22: Command-Line Arguments and the Outside Environment.
 * Provided. DO NOT MODIFY.
 *
 * This main drives the three functions you implement in
 * strava_filter.c. It does the I/O orchestration; you do the work.
 */

#include <stdio.h>
#include <stdlib.h>
#include "strava_filter.h"
#include "tests.h"

int main(int argc, char **argv) {
    /* Built with -D RUNTESTS (make run_tests)? Run your tests and stop.
     * Without the flag the preprocessor drops this block and the program
     * runs normally. Leave this block in place. */
#ifdef RUNTESTS
    run_all_tests();
    return 0;
#endif

    filter_config_t cfg;
    int rc = parse_args(argc, argv, &cfg);
    if (rc != 0) {
        return EXIT_FAILURE;
    }

    activity_t entries[MAX_ENTRIES];
    int n = load_log(cfg.logfile, entries, MAX_ENTRIES);
    if (n < 0) {
        fprintf(stderr, "error: could not open log file '%s'\n", cfg.logfile);
        return EXIT_FAILURE;
    }

    apply_filter(entries, n, &cfg);
    return EXIT_SUCCESS;
}
