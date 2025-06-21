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