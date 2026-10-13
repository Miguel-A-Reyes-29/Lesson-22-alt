
/*
 * test_lab22.c -- Assert tests for the Lesson 22 lab.
 *
 * Tests parse_args and apply_filter using hand-calculated expectations.
 */

#define _POSIX_C_SOURCE 200809L

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "strava_filter.h"
#include "tests.h"

/* Edge case: only the required logfile argument is provided. */
static void test_parse_args_defaults(void) {
    char *argv[] = {"prog", "data/activities.csv"};
    filter_config_t cfg;

    unsetenv("STRAVA_QUIET");

    assert(parse_args(2, argv, &cfg) == 0);
    assert(strcmp(cfg.logfile, "data/activities.csv") == 0);
    assert(strcmp(cfg.filter_type, "") == 0);
    assert(cfg.min_distance == 0.0);
    assert(cfg.quiet == 0);
}

/* Edge case: both optional arguments are provided. */
static void test_parse_args_with_filters(void) {
    char *argv[] = {
        "prog", "data/activities.csv", "run", "5.5"
    };
    filter_config_t cfg;

    unsetenv("STRAVA_QUIET");

    assert(parse_args(4, argv, &cfg) == 0);
    assert(strcmp(cfg.logfile, "data/activities.csv") == 0);
    assert(strcmp(cfg.filter_type, "run") == 0);
    assert(cfg.min_distance == 5.5);
}

/* Edge case: the required logfile argument is missing. */
static void test_parse_args_missing_logfile(void) {
    char *argv[] = {"prog"};
    filter_config_t cfg;

    assert(parse_args(1, argv, &cfg) != 0);
}

/* Edge case: the minimum distance contains non-numeric text. */
static void test_parse_args_invalid_distance(void) {
    char *argv[] = {
        "prog", "data/activities.csv", "run", "abc"
    };
    filter_config_t cfg;

    assert(parse_args(4, argv, &cfg) != 0);
}

/* Edge case: a negative minimum distance must be rejected. */
static void test_parse_args_negative_distance(void) {
    char *argv[] = {
        "prog", "data/activities.csv", "run", "-1.0"
    };
    filter_config_t cfg;

    assert(parse_args(4, argv, &cfg) != 0);
}

/* Edge case: STRAVA_QUIET is set, even to an empty value. */
static void test_parse_args_quiet_environment(void) {
    char *argv[] = {"prog", "data/activities.csv"};
    filter_config_t cfg;

    assert(setenv("STRAVA_QUIET", "", 1) == 0);
    assert(parse_args(2, argv, &cfg) == 0);
    assert(cfg.quiet == 1);

    assert(unsetenv("STRAVA_QUIET") == 0);
}

/* Edge case: only runs meeting the minimum distance should match. */
static void test_apply_filter_matching_entries(void) {
    activity_t entries[3] = {
        {"2026-04-12", "run", 5.20, 28, 148},
        {"2026-04-13", "ride", 12.50, 40, 135},
        {"2026-04-14", "run", 3.00, 18, 155}
    };

    filter_config_t cfg;
    cfg.logfile = "data/activities.csv";
    snprintf(cfg.filter_type, sizeof(cfg.filter_type), "%s", "run");
    cfg.min_distance = 5.0;
    cfg.quiet = 1;

    assert(apply_filter(entries, 3, &cfg) == 1);
}

/* Edge case: an empty type filter accepts every qualifying activity. */
static void test_apply_filter_empty_type(void) {
    activity_t entries[3] = {
        {"2026-04-12", "run", 5.20, 28, 148},
        {"2026-04-13", "ride", 12.50, 40, 135},
        {"2026-04-14", "run", 3.00, 18, 155}
    };

    filter_config_t cfg;
    cfg.logfile = "data/activities.csv";
    cfg.filter_type[0] = '\0';
    cfg.min_distance = 5.0;
    cfg.quiet = 1;

    assert(apply_filter(entries, 3, &cfg) == 2);
}

/* Edge case: zero entries should produce a count of zero. */
static void test_apply_filter_empty_array(void) {
    filter_config_t cfg;
    cfg.logfile = "data/activities.csv";
    cfg.filter_type[0] = '\0';
    cfg.min_distance = 0.0;
    cfg.quiet = 1;

    assert(apply_filter(NULL, 0, &cfg) == 0);
}

/* Run every test. */
void run_all_tests(void) {
    test_parse_args_defaults();
    test_parse_args_with_filters();
    test_parse_args_missing_logfile();
    test_parse_args_invalid_distance();
    test_parse_args_negative_distance();
    test_parse_args_quiet_environment();
    test_apply_filter_matching_entries();
    test_apply_filter_empty_type();
    test_apply_filter_empty_array();

    printf("all tests passed\n");
}
