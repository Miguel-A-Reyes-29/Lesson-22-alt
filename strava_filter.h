/*
 * strava_filter.h
 *
 * CS210 Lesson 22: Command-Line Arguments and the Outside Environment.
 * Provided header. DO NOT MODIFY.
 *
 * This header declares the three functions you will implement in
 * strava_filter.c. main.c is also provided and drives them.
 *
 * Each declaration carries a DFCS-standard function header
 * (@brief / @param / @return). Write headers in this same format
 * above every function you implement in strava_filter.c.
 */

#ifndef STRAVA_FILTER_H
#define STRAVA_FILTER_H

#define MAX_ENTRIES         200
#define MAX_ACTIVITY_TYPE   16
#define MAX_DATE_LEN        16

/*
 * One row of the activity log CSV.
 *
 * The CSV format is:
 *     date,activity_type,distance_km,duration_min,avg_hr
 * Example:
 *     2026-04-12,run,5.20,28,148
 */
typedef struct {
    char  date[MAX_DATE_LEN];
    char  activity_type[MAX_ACTIVITY_TYPE];
    double distance_km;
    int   duration_min;
    int   avg_hr;
} activity_t;

/*
 * The filter configuration produced by parse_args from the command line.
 *
 * filter_type is the empty string "" when no type filter is requested
 * (the cadet wants all activities, regardless of type).
 *
 * min_distance is 0.0 when no distance filter is requested.
 *
 * quiet is set from the STRAVA_QUIET environment variable. It is 1 if
 * STRAVA_QUIET is set to ANY value, and 0 otherwise.
 */
typedef struct {
    const char *logfile;
    char        filter_type[MAX_ACTIVITY_TYPE];
    double      min_distance;
    int         quiet;
} filter_config_t;

/**
* @brief validates the command line and fills in the filter configuration
* Usage required:
*     ./strava_filter <logfile> [activity_type] [min_distance]
* On success:
*   - cfg->logfile is set to argv[1].
*   - cfg->filter_type is set to argv[2] if present, or the empty string
*     "" if not. (Use strncpy with MAX_ACTIVITY_TYPE - 1 and
*     null-terminate; do not exceed the buffer.)
*   - cfg->min_distance is set to atof(argv[3]) if present, or 0.0.
*   - cfg->quiet is 1 if getenv("STRAVA_QUIET") returns non-NULL, else 0.
* On failure (too few or too many args, or argv[3] present but not a
* valid non-negative number), print a usage message to STDERR (not
* stdout!) of the form
*     usage: <argv[0]> <logfile> [activity_type] [min_distance]
* using argv[0], not a hard-coded program name.
* Implementation hint: keep parse_args focused on validation and
* config-filling. Do not open files or print activities here.
* @param argc argument count from main; must be at least 2 and at most 4
* @param argv argument vector from main
* @param cfg out: the configuration to populate
* @return 0 on success, non-zero on a usage error
*/
int parse_args(int argc, char **argv, filter_config_t *cfg);

/**
* @brief loads the activity log file into an array of activity_t
* Parses each line of the CSV (date,activity_type,distance_km,
* duration_min,avg_hr). Every line is one activity. If the file
* has more than max_entries rows, stop reading at max_entries and
* return max_entries (no error). Be sure to fclose the file before
* returning, including on success.
* @param path path to the activity log CSV
* @param entries out: array that receives the parsed activities
* @param max_entries capacity of the entries array
* @return the number of activities loaded (>= 0), or -1 if the file
*         cannot be opened
*/
int load_log(const char *path, activity_t *entries, int max_entries);

/**
* @brief prints every entry that matches the configured filters
* Match rule: an empty cfg->filter_type matches any activity_type;
* otherwise only entries whose activity_type equals cfg->filter_type
* (strcmp == 0) match. cfg->min_distance is a floor: entries with
* distance_km >= cfg->min_distance pass.
* Print each matching entry to STDOUT on its own line in this exact
* format (note the spaces and the decimal precision):
*     YYYY-MM-DD  <type>  <distance>km  <duration>min  HR <hr>
* distance_km prints with two decimal places. Examples:
*     2026-04-12  run  5.20km  28min  HR 148
*     2026-04-18  ride  22.30km  65min  HR 132
* After printing all matches, if cfg->quiet is 0, print a one-line
* summary to STDOUT: "<count> activities matched." If cfg->quiet is 1,
* do NOT print the summary line.
* @param entries array of activities to filter
* @param n number of activities in the array
* @param cfg the filter configuration to apply
* @return the number of entries printed
*/
int apply_filter(const activity_t *entries, int n, const filter_config_t *cfg);

#endif /* STRAVA_FILTER_H */
