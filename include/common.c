#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "common.h"
#include "professor_file_io.h"  // for trim_whitespace

void error_exit(const char* message) {
    perror(message);
    exit(EXIT_FAILURE);
}

void log_message(const char* message) {
    time_t now = time(NULL);
    struct tm* tm_info = localtime(&now);
    char time_buf[20];
    strftime(time_buf, sizeof(time_buf), "%Y-%m-%d %H:%M:%S", tm_info);

    printf("[%s] %s\n", time_buf, message);
}

// Parses a comma-separated list in `input` into `output` array (max `max_count` items).
// Returns the number of items parsed.
int parse_comma_separated(const char *input, char output[][64], int max_count) {
    char buf[512];
    strncpy(buf, input, sizeof(buf) - 1);
    buf[sizeof(buf) - 1] = '\0';

    int count = 0;
    char *rest = buf;
    char *token;
    while (count < max_count && (token = strsep(&rest, ",")) != NULL) {
        trim_whitespace(token);
        strncpy(output[count], token, 63);
        output[count][63] = '\0';
        count++;
    }
    return count;
}