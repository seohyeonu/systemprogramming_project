#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <errno.h>
#include <unistd.h>  // UID 사용
#include "file_io.h"
#include "timestamp.h"
#include "common.h"
#include <stdlib.h>

#define PROFESSOR_DIR "./data/professors"
#define GRADE_DIR "./data/Grade"

// Creates a grade file for the given subject if it doesn't exist.
static void create_grade_file(const char *subject) {
    char path[256];
    snprintf(path, sizeof(path), GRADE_DIR "/%s.dat", subject);
    if (access(path, F_OK) != 0) {
        FILE *fp = open_file(path, "w");
        fprintf(fp, "# 성적 파일: %s\n", subject);
        close_file(fp);
    }
}

// Deletes the grade file for the given subject if it exists.
static void delete_grade_file(const char *subject) {
    char path[256];
    snprintf(path, sizeof(path), GRADE_DIR "/%s.dat", subject);
    if (access(path, F_OK) == 0) {
        if (remove(path) == 0) {
            printf("교과목 [%s] 성적 파일 삭제 완료.\n", subject);
        } else {
            perror("성적 파일 삭제 실패");
        }
    } else {
        printf("성적 파일이 존재하지 않습니다: %s\n", subject);
    }
}

void trim_whitespace(char* str) {
    char* start = str;
    while (*start == ' ') start++;

    char* end = start + strlen(start) - 1;
    while (end > start && (*end == ' ' || *end == '\n')) *end-- = '\0';

    if (start != str) memmove(str, start, strlen(start) + 1);
}

void setup_professor_profile(const char* professor_id) {
    char prof_dir[] = "./data/professors";
    struct stat st = {0};
    // Ensure professor directory exists
    if (stat(prof_dir, &st) == -1) {
        mkdir(prof_dir, 0777);
    }
    // Construct profile path
    char prof_path[128];
    snprintf(prof_path, sizeof(prof_path), "%s/%s.dat", prof_dir, professor_id);

    // If profile already exists, skip setup
    if (access(prof_path, F_OK) == 0) {
        return;
    }

    // First-time setup: prompt for department and subjects
    char department[64];
    char subjects[256];
    printf("최초 실행: 학과를 입력하세요: ");
    fgets(department, sizeof(department), stdin);
    trim_whitespace(department);

    printf("담당 교과목을 입력하세요 (쉼표로 구분): ");
    fgets(subjects, sizeof(subjects), stdin);
    trim_whitespace(subjects);

    // Write profile file
    FILE *fp = open_file(prof_path, "w");
    fprintf(fp, "교수ID: %s\n", professor_id);
    fprintf(fp, "학과: %s\n", department);
    fprintf(fp, "교과목: %s\n", subjects);
    close_file(fp);

    // Ensure Grade directory exists
    char grade_dir[] = "./data/Grade";
    if (stat(grade_dir, &st) == -1) {
        mkdir(grade_dir, 0777);
    }

    // Create empty grade files for each subject
    {
        char *rest = subjects;
        char *token;
        while ((token = strsep(&rest, ",")) != NULL) {
            trim_whitespace(token);
            create_grade_file(token);
        }
    }
}


void add_subject() {
    char subject[32];
    printf("추가할 교과목 이름: ");
    scanf("%31s", subject);

    char path[256];
    snprintf(path, sizeof(path), GRADE_DIR "/%s.dat", subject);
    if (file_exists(path)) {
        printf("이미 존재하는 교과목입니다.\n");
        return;
    }
    create_grade_file(subject);

    // Update professor profile file
    char prof_id[64];
    snprintf(prof_id, sizeof(prof_id), "uid_%d", getuid());
    char prof_path[256];
    snprintf(prof_path, sizeof(prof_path), PROFESSOR_DIR "/%s.dat", prof_id);

    // Read all lines
    FILE *fp = open_file(prof_path, "r");
    char *lines[16];
    int line_count = 0;
    char buf[256];
    while (fgets(buf, sizeof(buf), fp)) {
        lines[line_count++] = strdup(buf);
    }
    close_file(fp);

    // Modify the "교과목:" line by appending the new subject
    for (int i = 0; i < line_count; i++) {
        if (strncmp(lines[i], "교과목:", strlen("교과목:")) == 0) {
            char *p = lines[i] + strlen("교과목:");
            trim_whitespace(p);
            char new_line[256];
            snprintf(new_line, sizeof(new_line), "교과목: %s, %s\n", p, subject);
            free(lines[i]);
            lines[i] = strdup(new_line);
            break;
        }
    }

    // Write back all lines
    fp = open_file(prof_path, "w");
    for (int i = 0; i < line_count; i++) {
        fputs(lines[i], fp);
        free(lines[i]);
    }
    close_file(fp);

    printf("교과목 [%s]이 생성되었습니다.\n", subject);
}

void delete_subject() {
    char subject[32];
    printf("삭제할 교과목 이름: ");
    scanf("%31s", subject);

    delete_grade_file(subject);

    // Update professor profile file
    char prof_id[64];
    snprintf(prof_id, sizeof(prof_id), "uid_%d", getuid());
    char prof_path[256];
    snprintf(prof_path, sizeof(prof_path), PROFESSOR_DIR "/%s.dat", prof_id);

    // Read all lines
    FILE *fp = open_file(prof_path, "r");
    char *lines[16];
    int line_count = 0;
    char buf[256];
    while (fgets(buf, sizeof(buf), fp)) {
        lines[line_count++] = strdup(buf);
    }
    close_file(fp);

    // Modify the "교과목:" line by removing the deleted subject
    for (int i = 0; i < line_count; i++) {
        if (strncmp(lines[i], "교과목:", strlen("교과목:")) == 0) {
            char *p = lines[i] + strlen("교과목:");
            trim_whitespace(p);
            char new_subjects[256] = "";
            char *rest = p;
            char *token;
            int first = 1;
            while ((token = strsep(&rest, ",")) != NULL) {
                trim_whitespace(token);
                if (strcmp(token, subject) == 0) continue;
                if (!first) strcat(new_subjects, ", ");
                strcat(new_subjects, token);
                first = 0;
            }
            char new_line[256];
            snprintf(new_line, sizeof(new_line), "교과목: %s\n", new_subjects);
            free(lines[i]);
            lines[i] = strdup(new_line);
            break;
        }
    }

    // Write back all lines
    fp = open_file(prof_path, "w");
    for (int i = 0; i < line_count; i++) {
        fputs(lines[i], fp);
        free(lines[i]);
    }
    close_file(fp);

    printf("교과목 [%s]이 삭제되었습니다.\n", subject);
}