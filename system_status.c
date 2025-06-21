#include "student_management.h"

// 파일 권한 설정
void set_file_permissions() {
    // Student 폴더 권한 설정
    chmod("Student", 0755);
    chmod("Student/index.dat", 0644);
    
    // Grade 폴더 권한 설정
    chmod("Grade", 0755);
    
    // 개별 학생 파일 권한 설정
    DIR *student_dir = opendir("Student");
    if (student_dir != NULL) {
        struct dirent *entry;
        while ((entry = readdir(student_dir)) != NULL) {
            if (strstr(entry->d_name, ".dat") != NULL && strcmp(entry->d_name, "index.dat") != 0) {
                char file_path[256];
                sprintf(file_path, "Student/%s", entry->d_name);
                chmod(file_path, 0644);
            }
        }
        closedir(student_dir);
    }
    
    // 교과목 파일 권한 설정
    DIR *grade_dir = opendir("Grade");
    if (grade_dir != NULL) {
        struct dirent *entry;
        while ((entry = readdir(grade_dir)) != NULL) {
            if (strstr(entry->d_name, ".dat") != NULL) {
                char file_path[256];
                sprintf(file_path, "Grade/%s", entry->d_name);
                chmod(file_path, 0644);
            }
        }
        closedir(grade_dir);
    }
}

// 시스템 상태 확인
void check_system_status() {
    printf("\n=== 시스템 상태 확인 ===\n");
    
    // Student 폴더 확인
    struct stat student_stat;
    if (stat("Student", &student_stat) == 0) {
        printf("Student 폴더: 존재함\n");
        
        // index.dat 파일 확인
        if (stat("Student/index.dat", &student_stat) == 0) {
            printf("index.dat 파일: 존재함\n");
        } else {
            printf("index.dat 파일: 없음\n");
        }
        
        // 학생 파일 개수 확인
        DIR *student_dir = opendir("Student");
        if (student_dir != NULL) {
            int file_count = 0;
            struct dirent *entry;
            while ((entry = readdir(student_dir)) != NULL) {
                if (strstr(entry->d_name, ".dat") != NULL && strcmp(entry->d_name, "index.dat") != 0) {
                    file_count++;
                }
            }
            closedir(student_dir);
            printf("학생 파일 개수: %d개\n", file_count);
        }
    } else {
        printf("Student 폴더: 없음\n");
    }
    
    // Grade 폴더 확인
    struct stat grade_stat;
    if (stat("Grade", &grade_stat) == 0) {
        printf("Grade 폴더: 존재함\n");
        
        // 교과목 파일 개수 확인
        DIR *grade_dir = opendir("Grade");
        if (grade_dir != NULL) {
            int file_count = 0;
            struct dirent *entry;
            while ((entry = readdir(grade_dir)) != NULL) {
                if (strstr(entry->d_name, ".dat") != NULL) {
                    file_count++;
                }
            }
            closedir(grade_dir);
            printf("교과목 파일 개수: %d개\n", file_count);
        }
    } else {
        printf("Grade 폴더: 없음\n");
    }
    
    printf("등록된 학생 수: %d명\n", student_count);
    printf("등록된 교과목 수: %d개\n", subject_count);
} 