#include "student_management.h"

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
        chmod(filename, 0644);
    }
    
    subject_count++;
    printf("교과목 '%s'이(가) 추가되었습니다.\n", subject_name);
} 