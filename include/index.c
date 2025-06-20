#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include "file_io.h"

#define INDEX_PATH "./data/Student/index.dat"
#define MAX_LINE 256

typedef struct {
    int number;
    char student_id[16];
    char timestamp[32];
} IndexEntry;

bool add_index_entry(const char* student_id, const char* timestamp) {
    FILE* read_fp = open_file(INDEX_PATH, "r");
    FILE* temp_fp = fopen("./data/Student/index_temp.dat", "w");
    if (read_fp == NULL || temp_fp == NULL) {
        perror("Error accessing index file");
        exit(EXIT_FAILURE);
    }

    char line[MAX_LINE];
    int reused_number = -1;
    int max_number = 0;
    while (fgets(line, sizeof(line), read_fp)) {
        int num;
        char id[16], ts[32];
        if (sscanf(line, "%d %s %s", &num, id, ts) == 3) {
            if (strlen(id) == 1 && id[0] == '-') {
                if (reused_number == -1) reused_number = num;
                fprintf(temp_fp, "%d %s %s\n", num, student_id, timestamp);
            } else {
                fprintf(temp_fp, "%s", line);
            }
            if (num > max_number) max_number = num;
        } else {
            fprintf(temp_fp, "%s", line);
        }
    }

    if (reused_number == -1) {
        fprintf(temp_fp, "%d %s %s\n", max_number + 1, student_id, timestamp);
    }

    close_file(read_fp);
    close_file(temp_fp);
    remove(INDEX_PATH);
    rename("./data/Student/index_temp.dat", INDEX_PATH);
    return true;
}

bool index_has_student(const char* student_id) {
    FILE* fp = open_file(INDEX_PATH, "r");
    if (fp == NULL) return false;

    char line[MAX_LINE];
    while (fgets(line, sizeof(line), fp)) {
        char id[16];
        if (sscanf(line, "%*d %s", id) == 1) {
            if (strcmp(id, student_id) == 0) {
                close_file(fp);
                return true;
            }
        }
    }
    close_file(fp);
    return false;
}

bool delete_index_entry(const char* student_id) {
    FILE* read_fp = open_file(INDEX_PATH, "r");
    FILE* temp_fp = fopen("./data/Student/index_temp.dat", "w");
    if (read_fp == NULL || temp_fp == NULL) {
        perror("Error accessing index file");
        exit(EXIT_FAILURE);
    }

    char line[MAX_LINE];
    while (fgets(line, sizeof(line), read_fp)) {
        int num;
        char id[16], ts[32];
        if (sscanf(line, "%d %s %s", &num, id, ts) == 3) {
            if (strcmp(id, student_id) == 0) {
                fprintf(temp_fp, "%d - -\n", num);  // mark as deleted
            } else {
                fprintf(temp_fp, "%s", line);
            }
        } else {
            fprintf(temp_fp, "%s", line);
        }
    }

    close_file(read_fp);
    close_file(temp_fp);
    remove(INDEX_PATH);
    rename("./data/Student/index_temp.dat", INDEX_PATH);
    return true;
}

bool update_index_timestamp(const char* student_id, const char* new_timestamp) {
    FILE* read_fp = open_file(INDEX_PATH, "r");
    FILE* temp_fp = fopen("./data/Student/index_temp.dat", "w");
    if (read_fp == NULL || temp_fp == NULL) {
        perror("Error accessing index file");
        exit(EXIT_FAILURE);
    }

    char line[MAX_LINE];
    while (fgets(line, sizeof(line), read_fp)) {
        int num;
        char id[16], ts[32];
        if (sscanf(line, "%d %s %s", &num, id, ts) == 3) {
            if (strcmp(id, student_id) == 0) {
                fprintf(temp_fp, "%d %s %s\n", num, student_id, new_timestamp);
            } else {
                fprintf(temp_fp, "%s", line);
            }
        } else {
            fprintf(temp_fp, "%s", line);
        }
    }

    close_file(read_fp);
    close_file(temp_fp);
    remove(INDEX_PATH);
    rename("./data/Student/index_temp.dat", INDEX_PATH);
    return true;
}