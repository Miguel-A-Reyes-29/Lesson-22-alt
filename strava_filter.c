
/*
 * strava_filter.c
 *
 * CS210 Lesson 22: Command-Line Arguments and the Outside Environment.
 *
 * Lab 22: Strava Activity Filter
 *
 * Documentation:I originally coded most of this on a plane flight, but later ran out of time and into erros, SO I THEN USED CHATGPT TO FINISH THE CODING< IT DID THE REST OF THE ENTIRETY OF THE ASSIGNMENT HERE IS THE LINk: https://chatgpt.com/share/6acd90a4-3db8-83e8-8ea9-bba66a79737b, i still however have a personal understanding. Do not misinterpret chat did this work pretty much."
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <math.h>
#include "strava_filter.h"

int parse_args(int argc, char **argv, filter_config_t *cfg) {
    if (argc < 2 || argc > 4) {
        fprintf(stderr,
                "usage: %s <logfile> [type] [min_distance]\n",
                argv[0]);
        return 1;
    }

    cfg->logfile = argv[1];
    cfg->filter_type[0] = '\0';
    cfg->min_distance = 0.0;

    if (argc >= 3) {
        if (strlen(argv[2]) >= sizeof(cfg->filter_type)) {
            fprintf(stderr, "Error: activity type is too long.\n");
            return 1;
        }

        snprintf(cfg->filter_type, sizeof(cfg->filter_type), "%s",
                 argv[2]);
    }

    if (argc == 4) {
        char *endptr;
        double distance;

        errno = 0;
        distance = strtod(argv[3], &endptr);

        if (argv[3][0] == '\0' ||
            endptr == argv[3] ||
            *endptr != '\0' ||
            errno == ERANGE ||
            !isfinite(distance) ||
            distance < 0.0) {
            fprintf(stderr,
                    "usage: %s <logfile> [type] [min_distance]\n",
                    argv[0]);
            return 1;
        }

        cfg->min_distance = distance;
    }

    cfg->quiet = (getenv("STRAVA_QUIET") != NULL);

    return 0;
}

int load_log(const char *path, activity_t *entries, int max_entries) {
    FILE *file;
    char line[256];
    int count = 0;

    file = fopen(path, "r");
    if (file == NULL) {
        return -1;
    }

    while (count < max_entries &&
           fgets(line, sizeof(line), file) != NULL) {
        activity_t activity;

        if (sscanf(line, "%10[^,],%15[^,],%lf,%d,%d",
                   activity.date,
                   activity.activity_type,
                   &activity.distance_km,
                   &activity.duration_min,
                   &activity.avg_hr) == 5) {
            entries[count] = activity;
            count++;
        }
    }

    fclose(file);
    return count;
}

int apply_filter(const activity_t *entries, int n,
                 const filter_config_t *cfg) {
    int printed = 0;

    for (int i = 0; i < n; i++) {
        if (cfg->filter_type[0] != '\0' &&
            strcmp(entries[i].activity_type,
                   cfg->filter_type) != 0) {
            continue;
        }

        if (entries[i].distance_km < cfg->min_distance) {
            continue;
        }

        printf("%s  %s  %.2fkm  %dmin  HR %d\n",
               entries[i].date,
               entries[i].activity_type,
               entries[i].distance_km,
               entries[i].duration_min,
               entries[i].avg_hr);

        printed++;
    }

    if (!cfg->quiet) {
        printf("%d activities matched.\n", printed);
    }

    return printed;
}
