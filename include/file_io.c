#include <stdio.h>
#include <stdlib.h> // exit 함수 사용을 위해 추가
#include <stdbool.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>

#define PROFESSOR_DIR "./data/professors"

FILE* open_file(const char* path, const char* mode) {
    FILE* fp = fopen(path, mode);
    if (fp == NULL) {
        // If the file is opened in read mode and doesn't exist, try to create it
        if (mode[0] == 'r') {
            fp = fopen(path, "w+");
            if (fp == NULL) {
                perror("Error creating file");
                exit(EXIT_FAILURE);
            }
        } else {
            perror("Error opening file");
            exit(EXIT_FAILURE);
        }
    }
    return fp;
}

bool file_exists(const char* path) {
    struct stat buffer;
    return (stat(path, &buffer) == 0);
}

void close_file(FILE* fp) {
    if (fp != NULL) {
        if (fclose(fp) != 0) {
            perror("Error closing file");
        }
    }
}

void ensure_student_directory_exists() {
    const char* path = "./data/Student";
    struct stat st;
    if (stat(path, &st) == -1) {
        if (mkdir("./data", 0777) == -1 && errno != EEXIST) {
            perror("Failed to create ./data directory");
            exit(EXIT_FAILURE);
        }
        if (mkdir(path, 0777) == -1) {
            perror("Failed to create ./data/Student directory");
            exit(EXIT_FAILURE);
        }
    }
}

void ensure_grade_directory_exists() {
    const char* path = "./data/Grade";
    struct stat st;
    if (stat(path, &st) == -1) {
        if (mkdir("./data", 0777) == -1 && errno != EEXIST) {
            perror("Failed to create ./data");
        }
        if (mkdir(path, 0777) == -1) {
            perror("Failed to create ./data/Grade");
        }
    }
}

void ensure_professor_directory_exists() {
    struct stat st;
    if (stat(PROFESSOR_DIR, &st) == -1) {
        if (mkdir("./data", 0777) == -1 && errno != EEXIST) {
            perror("Failed to create data directory");
            exit(EXIT_FAILURE);
        }
        if (mkdir(PROFESSOR_DIR, 0777) == -1) {
            perror("Failed to create professors directory");
            exit(EXIT_FAILURE);
        }
    }
}