#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <dirent.h>
#include "student_management.h"

// 성적 관리 메뉴
void grade_management_menu() {
    int choice;
    do {
        printf("\n=== 성적 관리 ===\n");
        printf("1. 교과목 추가\n");
        printf("2. 교과목 제거\n");
        printf("3. 교과목 목록\n");
        printf("4. 성적 입력\n");
        printf("5. 성적 수정\n");
        printf("0. 메인 메뉴로 돌아가기\n");
        printf("선택: ");
        scanf("%d", &choice);
        getchar(); // 버퍼 정리
        
        switch(choice) {
            case 1:
                add_subject();
                break;
            case 2:
                remove_subject();
                break;
            case 3:
                list_subjects();
                break;
            case 4:
                input_grades();
                break;
            case 5:
                modify_grades();
                break;
            case 0:
                printf("메인 메뉴로 돌아갑니다.\n");
                break;
            default:
                printf("잘못된 선택입니다.\n");
        }
    } while (choice != 0);
}

// 교과목 목록 로드
void load_subjects() {
    subject_count = 0;
    DIR *dir = opendir("Grade");
    if (dir == NULL) {
        return;
    }
    
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (strstr(entry->d_name, ".dat") != NULL) {
            char subject_name[MAX_SUBJECT];
            strcpy(subject_name, entry->d_name);
            subject_name[strlen(subject_name) - 4] = '\0'; // .dat 제거
            
            strcpy(subjects[subject_count].name, subject_name);
            subjects[subject_count].created_time = time(NULL);
            subject_count++;
        }
    }
    closedir(dir);
}

// 교과목 추가
void add_subject() {
    if (subject_count >= MAX_SUBJECTS) {
        printf("최대 교과목 수에 도달했습니다.\n");
        return;
    }
    
    printf("\n=== 교과목 추가 ===\n");
    
    char subject_name[MAX_SUBJECT];
    char department[MAX_DEPT];
    
    printf("교과목명: ");
    scanf("%s", subject_name);
    getchar();
    
    printf("학과: ");
    scanf("%s", department);
    getchar();
    
    // 중복 확인
    for (int i = 0; i < subject_count; i++) {
        if (strcmp(subjects[i].name, subject_name) == 0) {
            printf("이미 존재하는 교과목입니다.\n");
            return;
        }
    }
    
    // 교과목 정보 저장
    strcpy(subjects[subject_count].name, subject_name);
    strcpy(subjects[subject_count].department, department);
    subjects[subject_count].created_time = time(NULL);
    
    // 교과목 파일 생성
    char filename[100];
    sprintf(filename, "Grade/%s.dat", subject_name);
    FILE *subject_file = fopen(filename, "w");
    if (subject_file != NULL) {
        fprintf(subject_file, "# %s 교과목 성적 파일\n", subject_name);
        fprintf(subject_file, "# 학과: %s\n", department);
        fprintf(subject_file, "# 생성일: %s\n", ctime(&subjects[subject_count].created_time));
        fclose(subject_file);
        
        // 파일 권한 설정
        chmod(filename, 0644);
    }
    
    subject_count++;
    printf("교과목 '%s'이(가) 추가되었습니다.\n", subject_name);
}

// 교과목 제거
void remove_subject() {
    printf("\n=== 교과목 제거 ===\n");
    
    if (subject_count == 0) {
        printf("등록된 교과목이 없습니다.\n");
        return;
    }
    
    printf("등록된 교과목:\n");
    for (int i = 0; i < subject_count; i++) {
        printf("%d. %s (%s)\n", i + 1, subjects[i].name, subjects[i].department);
    }
    
    int choice;
    printf("제거할 교과목 번호: ");
    scanf("%d", &choice);
    getchar();
    
    if (choice < 1 || choice > subject_count) {
        printf("잘못된 선택입니다.\n");
        return;
    }
    
    choice--; // 배열 인덱스로 변환
    
    printf("교과목 '%s'을(를) 제거하시겠습니까? (y/n): ", subjects[choice].name);
    char confirm;
    scanf("%c", &confirm);
    getchar();
    
    if (confirm == 'y' || confirm == 'Y') {
        // 파일 삭제
        char filename[100];
        sprintf(filename, "Grade/%s.dat", subjects[choice].name);
        remove(filename);
        
        // 배열에서 제거
        for (int i = choice; i < subject_count - 1; i++) {
            subjects[i] = subjects[i + 1];
        }
        subject_count--;
        
        printf("교과목이 제거되었습니다.\n");
    } else {
        printf("제거가 취소되었습니다.\n");
    }
}

// 교과목 목록
void list_subjects() {
    printf("\n=== 교과목 목록 ===\n");
    
    if (subject_count == 0) {
        printf("등록된 교과목이 없습니다.\n");
        return;
    }
    
    for (int i = 0; i < subject_count; i++) {
        char time_str[20];
        struct tm *tm_info = localtime(&subjects[i].created_time);
        strftime(time_str, 20, "%Y-%m-%d", tm_info);
        
        printf("%d. %s (%s) - 생성일: %s\n", 
               i + 1, subjects[i].name, subjects[i].department, time_str);
    }
}

// 성적 입력
void input_grades() {
    printf("\n=== 성적 입력 ===\n");
    
    if (subject_count == 0) {
        printf("등록된 교과목이 없습니다.\n");
        return;
    }
    
    printf("교과목 목록:\n");
    for (int i = 0; i < subject_count; i++) {
        printf("%d. %s (%s)\n", i + 1, subjects[i].name, subjects[i].department);
    }
    
    int choice;
    printf("성적을 입력할 교과목 번호: ");
    scanf("%d", &choice);
    getchar();
    
    if (choice < 1 || choice > subject_count) {
        printf("잘못된 선택입니다.\n");
        return;
    }
    
    choice--; // 배열 인덱스로 변환
    
    // 해당 학과 학생들 찾기
    printf("\n%s 학과 학생 목록:\n", subjects[choice].department);
    int enrolled_students[MAX_STUDENTS];
    int enrolled_count = 0;
    
    for (int i = 0; i < student_count; i++) {
        if (strcmp(students[i].department, subjects[choice].department) == 0) {
            enrolled_students[enrolled_count] = i;
            printf("%d. %s - %s\n", enrolled_count + 1, 
                   students[i].student_id, students[i].name);
            enrolled_count++;
        }
    }
    
    if (enrolled_count == 0) {
        printf("해당 학과의 학생이 없습니다.\n");
        return;
    }
    
    // 성적 입력
    printf("\n성적을 입력하세요 (학번 - 점수 형식, 종료하려면 'end' 입력):\n");
    char input[100];
    
    while (1) {
        printf("> ");
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = 0;
        
        if (strcmp(input, "end") == 0) {
            break;
        }
        
        char student_id[20];
        int score;
        
        if (sscanf(input, "%s - %d", student_id, &score) == 2) {
            if (score < 0 || score > 100) {
                printf("점수는 0-100 사이여야 합니다.\n");
                continue;
            }
            
            // 성적 파일에 저장
            char filename[100];
            sprintf(filename, "Grade/%s.dat", subjects[choice].name);
            FILE *grade_file = fopen(filename, "a");
            if (grade_file != NULL) {
                time_t now = time(NULL);
                fprintf(grade_file, "%s %d %ld\n", student_id, score, now);
                fclose(grade_file);
                printf("성적이 저장되었습니다: %s - %d점\n", student_id, score);
            }
        } else {
            printf("잘못된 형식입니다. (학번 - 점수)\n");
        }
    }
}

// 성적 수정
void modify_grades() {
    printf("\n=== 성적 수정 ===\n");
    
    if (subject_count == 0) {
        printf("등록된 교과목이 없습니다.\n");
        return;
    }
    
    printf("교과목 목록:\n");
    for (int i = 0; i < subject_count; i++) {
        printf("%d. %s (%s)\n", i + 1, subjects[i].name, subjects[i].department);
    }
    
    int choice;
    printf("성적을 수정할 교과목 번호: ");
    scanf("%d", &choice);
    getchar();
    
    if (choice < 1 || choice > subject_count) {
        printf("잘못된 선택입니다.\n");
        return;
    }
    
    choice--; // 배열 인덱스로 변환
    
    char filename[100];
    sprintf(filename, "Grade/%s.dat", subjects[choice].name);
    
    // 현재 성적 표시
    printf("\n현재 성적:\n");
    FILE *grade_file = fopen(filename, "r");
    if (grade_file != NULL) {
        char line[256];
        while (fgets(line, sizeof(line), grade_file)) {
            if (line[0] != '#') { // 주석이 아닌 경우
                char student_id[20];
                int score;
                time_t timestamp;
                if (sscanf(line, "%s %d %ld", student_id, &score, &timestamp) == 3) {
                    char time_str[20];
                    struct tm *tm_info = localtime(&timestamp);
                    strftime(time_str, 20, "%Y-%m-%d %H:%M", tm_info);
                    printf("%s - %d점 (입력시간: %s)\n", student_id, score, time_str);
                }
            }
        }
        fclose(grade_file);
    }
    
    // 성적 수정
    char student_id[20];
    printf("\n수정할 학생의 학번: ");
    scanf("%s", student_id);
    getchar();
    
    int new_score;
    printf("새로운 점수: ");
    scanf("%d", &new_score);
    getchar();
    
    if (new_score < 0 || new_score > 100) {
        printf("점수는 0-100 사이여야 합니다.\n");
        return;
    }
    
    // 임시 파일에 복사하면서 수정
    char temp_filename[100];
    sprintf(temp_filename, "Grade/%s.tmp", subjects[choice].name);
    
    FILE *temp_file = fopen(temp_filename, "w");
    FILE *original_file = fopen(filename, "r");
    
    if (temp_file != NULL && original_file != NULL) {
        char line[256];
        int found = 0;
        
        while (fgets(line, sizeof(line), original_file)) {
            if (line[0] == '#') {
                fputs(line, temp_file); // 주석은 그대로 복사
            } else {
                char current_id[20];
                int current_score;
                time_t current_timestamp;
                
                if (sscanf(line, "%s %d %ld", current_id, &current_score, &current_timestamp) == 3) {
                    if (strcmp(current_id, student_id) == 0) {
                        // 해당 학생의 성적을 새로운 점수로 수정
                        time_t now = time(NULL);
                        fprintf(temp_file, "%s %d %ld\n", student_id, new_score, now);
                        found = 1;
                        printf("성적이 수정되었습니다: %s - %d점\n", student_id, new_score);
                    } else {
                        fputs(line, temp_file); // 다른 학생은 그대로 복사
                    }
                } else {
                    fputs(line, temp_file); // 파싱 실패한 라인도 그대로 복사
                }
            }
        }
        
        if (!found) {
            printf("해당 학번의 성적을 찾을 수 없습니다.\n");
        }
        
        fclose(temp_file);
        fclose(original_file);
        
        // 임시 파일을 원본 파일로 교체
        remove(filename);
        rename(temp_filename, filename);
    }
} 