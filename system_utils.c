#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include "student_management.h"

// 시스템 초기화
void initialize_system() {
    printf("\n=== 시스템 초기화 ===\n");
    printf("이 작업은 모든 데이터를 삭제합니다.\n");
    printf("정말로 초기화하시겠습니까? (y/n): ");
    
    char confirm;
    scanf("%c", &confirm);
    getchar();
    
    if (confirm != 'y' && confirm != 'Y') {
        printf("초기화가 취소되었습니다.\n");
        return;
    }
    
    // Student 폴더 삭제
    remove_directory_contents("Student");
    rmdir("Student");
    
    // Grade 폴더 삭제
    remove_directory_contents("Grade");
    rmdir("Grade");
    
    // 전역 변수 초기화
    student_count = 0;
    subject_count = 0;
    
    // 디렉토리 재생성
    mkdir("Student", 0755);
    mkdir("Grade", 0755);
    
    // index.dat 파일 생성
    FILE *index_file = fopen("Student/index.dat", "w");
    if (index_file != NULL) {
        fclose(index_file);
        chmod("Student/index.dat", 0644);
    }
    
    printf("시스템이 초기화되었습니다.\n");
}

// 디렉토리 내용 삭제
void remove_directory_contents(const char* dir_path) {
    DIR *dir = opendir(dir_path);
    if (dir == NULL) {
        return;
    }
    
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }
        
        char file_path[256];
        sprintf(file_path, "%s/%s", dir_path, entry->d_name);
        
        struct stat stat_buf;
        if (stat(file_path, &stat_buf) == 0) {
            if (S_ISDIR(stat_buf.st_mode)) {
                // 디렉토리인 경우 재귀적으로 삭제
                remove_directory_contents(file_path);
                rmdir(file_path);
            } else {
                // 파일인 경우 삭제
                remove(file_path);
            }
        }
    }
    closedir(dir);
}

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