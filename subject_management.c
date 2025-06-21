#include "student_management.h"

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