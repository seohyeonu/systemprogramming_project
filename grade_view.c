#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <pwd.h>
#include <dirent.h>
#include "student_management.h"

// 성적 열람 메뉴
void grade_view_menu() {
    if (is_student()) {
        view_my_grades();
    } else {
        printf("학생만 성적을 열람할 수 있습니다.\n");
    }
}

// 내 성적 조회 (학생용)
void view_my_grades() {
    uid_t uid = getuid();
    struct passwd *pw = getpwuid(uid);
    
    // UID를 학번으로 변환 (예: UID 20213348 -> 학번 20213348)
    char my_student_id[20];
    sprintf(my_student_id, "%d", uid);
    
    // 내 학생 정보 찾기
    int my_index = -1;
    for (int i = 0; i < student_count; i++) {
        if (strcmp(students[i].student_id, my_student_id) == 0) {
            my_index = i;
            break;
        }
    }
    
    if (my_index == -1) {
        printf("학생 정보를 찾을 수 없습니다.\n");
        return;
    }
    
    printf("\n=== 내 성적 조회 ===\n");
    printf("학번: %s\n", students[my_index].student_id);
    printf("이름: %s\n", students[my_index].name);
    printf("학과: %s\n", students[my_index].department);
    printf("학적 상태: %s\n", students[my_index].status);
    printf("\n");
    
    // 교과목 목록 로드
    load_subjects();
    
    if (subject_count == 0) {
        printf("등록된 교과목이 없습니다.\n");
        return;
    }
    
    printf("=== 성적 현황 ===\n");
    int has_grades = 0;
    
    for (int i = 0; i < subject_count; i++) {
        // 해당 학과의 교과목인지 확인
        if (strcmp(subjects[i].department, students[my_index].department) == 0) {
            char filename[100];
            sprintf(filename, "Grade/%s.dat", subjects[i].name);
            
            FILE *grade_file = fopen(filename, "r");
            if (grade_file != NULL) {
                char line[256];
                while (fgets(line, sizeof(line), grade_file)) {
                    if (line[0] != '#') { // 주석이 아닌 경우
                        char student_id[20];
                        int score;
                        time_t timestamp;
                        
                        if (sscanf(line, "%s %d %ld", student_id, &score, &timestamp) == 3) {
                            if (strcmp(student_id, my_student_id) == 0) {
                                char time_str[20];
                                struct tm *tm_info = localtime(&timestamp);
                                strftime(time_str, 20, "%Y-%m-%d %H:%M", tm_info);
                                
                                printf("%s: %d점 (입력시간: %s)\n", 
                                       subjects[i].name, score, time_str);
                                has_grades = 1;
                                break;
                            }
                        }
                    }
                }
                fclose(grade_file);
            }
        }
    }
    
    if (!has_grades) {
        printf("아직 입력된 성적이 없습니다.\n");
    }
}

// 성적 파일 생성 (학생 배포용)
void create_student_grade_file(const char* student_id) {
    // 해당 학생 정보 찾기
    int student_index = -1;
    for (int i = 0; i < student_count; i++) {
        if (strcmp(students[i].student_id, student_id) == 0) {
            student_index = i;
            break;
        }
    }
    
    if (student_index == -1) {
        printf("학생을 찾을 수 없습니다.\n");
        return;
    }
    
    // 학생별 성적 파일 생성
    char filename[100];
    sprintf(filename, "Grade/student_%s_grade.txt", student_id);
    
    FILE *output_file = fopen(filename, "w");
    if (output_file == NULL) {
        printf("파일을 생성할 수 없습니다.\n");
        return;
    }
    
    // 학생 정보 출력
    fprintf(output_file, "=== 학생 성적표 ===\n");
    fprintf(output_file, "학번: %s\n", students[student_index].student_id);
    fprintf(output_file, "이름: %s\n", students[student_index].name);
    fprintf(output_file, "학과: %s\n", students[student_index].department);
    fprintf(output_file, "학적 상태: %s\n", students[student_index].status);
    fprintf(output_file, "생성일: %s\n", ctime(&students[student_index].last_modified));
    fprintf(output_file, "\n");
    
    // 교과목 목록 로드
    load_subjects();
    
    fprintf(output_file, "=== 성적 현황 ===\n");
    int has_grades = 0;
    int total_score = 0;
    int subject_count_with_grades = 0;
    
    for (int i = 0; i < subject_count; i++) {
        // 해당 학과의 교과목인지 확인
        if (strcmp(subjects[i].department, students[student_index].department) == 0) {
            char grade_filename[100];
            sprintf(grade_filename, "Grade/%s.dat", subjects[i].name);
            
            FILE *grade_file = fopen(grade_filename, "r");
            if (grade_file != NULL) {
                char line[256];
                while (fgets(line, sizeof(line), grade_file)) {
                    if (line[0] != '#') { // 주석이 아닌 경우
                        char current_student_id[20];
                        int score;
                        time_t timestamp;
                        
                        if (sscanf(line, "%s %d %ld", current_student_id, &score, &timestamp) == 3) {
                            if (strcmp(current_student_id, student_id) == 0) {
                                char time_str[20];
                                struct tm *tm_info = localtime(&timestamp);
                                strftime(time_str, 20, "%Y-%m-%d %H:%M", tm_info);
                                
                                fprintf(output_file, "%s: %d점 (입력시간: %s)\n", 
                                       subjects[i].name, score, time_str);
                                
                                total_score += score;
                                subject_count_with_grades++;
                                has_grades = 1;
                                break;
                            }
                        }
                    }
                }
                fclose(grade_file);
            }
        }
    }
    
    if (has_grades) {
        double average = (double)total_score / subject_count_with_grades;
        fprintf(output_file, "\n=== 성적 요약 ===\n");
        fprintf(output_file, "총점: %d점\n", total_score);
        fprintf(output_file, "평균: %.2f점\n", average);
        fprintf(output_file, "수강 과목 수: %d개\n", subject_count_with_grades);
    } else {
        fprintf(output_file, "아직 입력된 성적이 없습니다.\n");
    }
    
    fclose(output_file);
    
    // 파일 권한 설정 (읽기 전용)
    chmod(filename, 0444);
    
    printf("학생 성적 파일이 생성되었습니다: %s\n", filename);
}

// 모든 학생의 성적 파일 생성 (관리자용)
void create_all_student_grade_files() {
    printf("\n=== 모든 학생 성적 파일 생성 ===\n");
    
    for (int i = 0; i < student_count; i++) {
        create_student_grade_file(students[i].student_id);
    }
    
    printf("모든 학생의 성적 파일이 생성되었습니다.\n");
} 