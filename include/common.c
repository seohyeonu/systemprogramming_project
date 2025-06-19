#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>

#include "common.h"
#include "professor_file_io.h"  // for trim_whitespace

// Role-based UID lists for access control
static const uid_t PROFESSOR_UIDS[] = { /* 교수 UID 목록을 여기에 입력하세요 */ };
static const size_t NUM_PROFESSORS = sizeof(PROFESSOR_UIDS) / sizeof(PROFESSOR_UIDS[0]);
static const uid_t STUDENT_UIDS[] = {501};
static const size_t NUM_STUDENTS = sizeof(STUDENT_UIDS) / sizeof(STUDENT_UIDS[0]);

// Checks whether the given uid is in the professor list
int is_professor_uid(uid_t uid) {
    for (size_t i = 0; i < NUM_PROFESSORS; ++i) {
        if (PROFESSOR_UIDS[i] == uid) return 1;
    }
    return 0;
}

// Checks whether the given uid is in the student list
int is_student_uid(uid_t uid) {
    for (size_t i = 0; i < NUM_STUDENTS; ++i) {
        if (STUDENT_UIDS[i] == uid) return 1;
    }
    return 0;
}

// Enforces that only professors can proceed
void require_professor() {
    uid_t uid = getuid();
    if (!is_professor_uid(uid)) {
        fprintf(stderr, "권한 거부: 교수만 사용할 수 있습니다.\n");
        exit(EXIT_FAILURE);
    }
}

// Enforces that only students can proceed
void require_student() {
    uid_t uid = getuid();
    if (!is_student_uid(uid)) {
        fprintf(stderr, "권한 거부: 학생만 사용할 수 있습니다.\n");
        exit(EXIT_FAILURE);
    }
}

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