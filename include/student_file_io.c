#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "file_io.h"

#define STUDENT_FOLDER "./data/Student/"
#define MAX_LINE 256

bool create_student_file(const char* student_id, const char* name, const char* birth, const char* department, const char* status) {
    char path[128];
    snprintf(path, sizeof(path), "%s%s.dat", STUDENT_FOLDER, student_id);

    FILE* fp = open_file(path, "w");
    if (fp == NULL) return false;

    fprintf(fp, "학번: %s\n", student_id);
    fprintf(fp, "이름: %s\n", name);
    fprintf(fp, "생년월일: %s\n", birth);
    fprintf(fp, "학과: %s\n", department);
    fprintf(fp, "학적 상태: %s\n", status);

    close_file(fp);
    return true;
}

bool update_student_status(const char* student_id, const char* new_status) {
    char path[128];
    snprintf(path, sizeof(path), "%s%s.dat", STUDENT_FOLDER, student_id);

    FILE* read_fp = open_file(path, "r");
    FILE* temp_fp = fopen("./data/Student/temp_student.dat", "w");
    if (read_fp == NULL || temp_fp == NULL) {
        perror("Error opening student file");
        exit(EXIT_FAILURE);
    }

    char line[MAX_LINE];
    while (fgets(line, sizeof(line), read_fp)) {
        if (strncmp(line, "학적 상태:", 14) == 0) {
            fprintf(temp_fp, "학적 상태: %s\n", new_status);
        } else {
            fprintf(temp_fp, "%s", line);
        }
    }

    close_file(read_fp);
    close_file(temp_fp);
    remove(path);
    rename("./data/Student/temp_student.dat", path);
    return true;
}

bool update_student_info(const char* student_id, const char* new_name, const char* new_birth, const char* new_department, const char* new_status) {
    char path[128];
    snprintf(path, sizeof(path), "%s%s.dat", STUDENT_FOLDER, student_id);

    FILE* read_fp = open_file(path, "r");
    FILE* temp_fp = fopen("./data/Student/temp_student.dat", "w");
    if (read_fp == NULL || temp_fp == NULL) {
        perror("Error opening student file");
        exit(EXIT_FAILURE);
    }

    char line[MAX_LINE];
    while (fgets(line, sizeof(line), read_fp)) {
        if (strncmp(line, "이름:", 7) == 0) {
            fprintf(temp_fp, "이름: %s\n", new_name);
        } else if (strncmp(line, "생년월일:", 13) == 0) {
            fprintf(temp_fp, "생년월일: %s\n", new_birth);
        } else if (strncmp(line, "학과:", 7) == 0) {
            fprintf(temp_fp, "학과: %s\n", new_department);
        } else if (strncmp(line, "학적 상태:", 14) == 0) {
            fprintf(temp_fp, "학적 상태: %s\n", new_status);
        } else {
            fprintf(temp_fp, "%s", line);
        }
    }

    close_file(read_fp);
    close_file(temp_fp);
    remove(path);
    rename("./data/Student/temp_student.dat", path);
    return true;
}
// Deletes a student's data file based on their student ID
bool delete_student_file(const char* student_id) {
    char path[128];
    snprintf(path, sizeof(path), "%s%s.dat", STUDENT_FOLDER, student_id);
    return remove(path) == 0;
}

