#include "student_management.h"

// 디렉토리 생성
void create_directories() {
    mkdir("Student", 0755);
    mkdir("Grade", 0755);
    
    // index.dat 파일이 없으면 생성
    FILE *index_file = fopen("Student/index.dat", "r");
    if (index_file == NULL) {
        index_file = fopen("Student/index.dat", "w");
        if (index_file != NULL) {
            fclose(index_file);
            printf("Student/index.dat 파일이 생성되었습니다.\n");
        }
    } else {
        fclose(index_file);
    }
}

// 인덱스 데이터 로드
void load_index_data() {
    FILE *index_file = fopen("Student/index.dat", "r");
    if (index_file == NULL) {
        printf("index.dat 파일을 찾을 수 없습니다.\n");
        return;
    }
    
    student_count = 0;
    while (fscanf(index_file, "%d %s %ld", 
                  &students[student_count].number,
                  students[student_count].student_id,
                  &students[student_count].last_modified) == 3) {
        
        // 개별 학생 파일에서 상세 정보 로드
        char filename[100];
        sprintf(filename, "Student/%s.dat", students[student_count].student_id);
        FILE *student_file = fopen(filename, "r");
        if (student_file != NULL) {
            char temp[100];
            fscanf(student_file, "%s %s %s %s %s",
                   students[student_count].name,
                   students[student_count].birth_date,
                   students[student_count].department,
                   students[student_count].status,
                   temp);
            fclose(student_file);
        }
        student_count++;
    }
    fclose(index_file);
    printf("학생 데이터 로드 완료: %d명\n", student_count);
}

// 인덱스 데이터 저장
void save_index_data() {
    FILE *index_file = fopen("Student/index.dat", "w");
    if (index_file == NULL) {
        printf("index.dat 파일을 열 수 없습니다.\n");
        return;
    }
    
    for (int i = 0; i < student_count; i++) {
        fprintf(index_file, "%d %s %ld\n",
                students[i].number,
                students[i].student_id,
                students[i].last_modified);
    }
    fclose(index_file);
}

// 화면 클리어
void clear_screen() {
    system("clear");
} 