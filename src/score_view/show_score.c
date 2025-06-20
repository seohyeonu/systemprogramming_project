#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <errno.h>
#include <pwd.h>
#include "../../include/common.h"  

#define STUDENT_DIR   "./data/Student"
#define GRADE_DIR     "./data/Grade"
#define MAX_LINE_LEN  256
#define MAX_NAME_LEN  64

// Trim leading/trailing whitespace
static void trim(char *s) {
    char *p = s, *q = s + strlen(s) - 1;
    while (p <= q && (*p == ' ' || *p == '\n')) p++;
    while (q > p && (*q == ' ' || *q == '\n')) *q-- = '\0';
    if (p != s) memmove(s, p, q - p + 2);
}

static void show_student_view(uid_t uid) {
    // Determine student ID from login name
    struct passwd *pw = getpwuid(uid);
    if (!pw) {
        fprintf(stderr, "사용자 정보를 가져올 수 없습니다.\n");
        return;
    }
    const char *student_id = pw->pw_name;

    // Open student profile file: <STUDENT_DIR>/<student_id>.dat
    char student_path[256];
    snprintf(student_path, sizeof(student_path), STUDENT_DIR "/%s.dat", student_id);
    FILE *f = fopen(student_path, "r");
    if (!f) {
        perror("학생 정보 파일 열기 실패");
        return;
    }

    // Print personal info
    char line[MAX_LINE_LEN];
    printf("=== 개인 정보 ===\n");
    while (fgets(line, sizeof(line), f)) {
        printf("%s", line);
    }
    fclose(f);

    // Print grades only for this student
    printf("\n=== 나의 성적 ===\n");
    DIR *gd = opendir(GRADE_DIR);
    if (!gd) {
        perror("Grade dir");
        return;
    }
    struct dirent *e;
    while ((e = readdir(gd))) {
        if (!strstr(e->d_name, ".dat")) continue;
        char gpath[256];
        snprintf(gpath, sizeof(gpath), GRADE_DIR "/%s", e->d_name);
        FILE *gf = fopen(gpath, "r");
        if (!gf) continue;
        // Extract subject name
        char subject[64];
        strncpy(subject, e->d_name, strlen(e->d_name) - 4);
        subject[strlen(e->d_name) - 4] = '\0';

        // Scan for this student's record
        char rec[MAX_LINE_LEN];
        while (fgets(rec, sizeof(rec), gf)) {
            char rec_id[32];
            int score;
            char ts[64];
            if (sscanf(rec, "%31s %d %63s", rec_id, &score, ts) == 3 &&
                strcmp(rec_id, student_id) == 0) {
                printf("%s: %d (%s)\n", subject, score, ts);
            }
        }
        fclose(gf);
    }
    closedir(gd);
}

int main() {
    require_student();
    uid_t uid = getuid();
    show_student_view(uid);
    return 0;
}