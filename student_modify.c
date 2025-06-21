#include "student_management.h"

// 학생 정보 수정
void modify_student() {
    char student_id[20];
    printf("\n=== 학생 정보 수정 ===\n");
    printf("수정할 학생의 학번: ");
    scanf("%s", student_id);
    getchar();
    
    int found = -1;
    for (int i = 0; i < student_count; i++) {
        if (strcmp(students[i].student_id, student_id) == 0) {
            found = i;
            break;
        }
    }
    
    if (found == -1) {
        printf("해당 학번의 학생을 찾을 수 없습니다.\n");
        return;
    }
    
    printf("\n현재 정보:\n");
    printf("이름: %s\n", students[found].name);
    printf("생년월일: %s\n", students[found].birth_date);
    printf("학과: %s\n", students[found].department);
    printf("학적 상태: %s\n", students[found].status);
    
    printf("\n새로운 정보를 입력하세요 (변경하지 않으려면 엔터):\n");
    
    char temp[100];
    
    // 이름 수정
    printf("이름 (%s): ", students[found].name);
    fgets(temp, sizeof(temp), stdin);
    temp[strcspn(temp, "\n")] = 0;
    if (strlen(temp) > 0) {
        strcpy(students[found].name, temp);
    }
    
    // 생년월일 수정
    printf("생년월일 (%s): ", students[found].birth_date);
    fgets(temp, sizeof(temp), stdin);
    temp[strcspn(temp, "\n")] = 0;
    if (strlen(temp) > 0) {
        strcpy(students[found].birth_date, temp);
    }
    
    // 학과 수정
    printf("학과 (%s): ", students[found].department);
    fgets(temp, sizeof(temp), stdin);
    temp[strcspn(temp, "\n")] = 0;
    if (strlen(temp) > 0) {
        strcpy(students[found].department, temp);
    }
    
    // 학적 상태 수정
    printf("학적 상태 (%s): ", students[found].status);
    fgets(temp, sizeof(temp), stdin);
    temp[strcspn(temp, "\n")] = 0;
    if (strlen(temp) > 0) {
        strcpy(students[found].status, temp);
    }
    
    // 수정 시간 업데이트
    students[found].last_modified = time(NULL);
    
    // 개별 학생 파일 업데이트
    char filename[100];
    sprintf(filename, "Student/%s.dat", students[found].student_id);
    FILE *student_file = fopen(filename, "w");
    if (student_file != NULL) {
        fprintf(student_file, "%s %s %s %s %s\n",
                students[found].name,
                students[found].birth_date,
                students[found].department,
                students[found].status,
                "");
        fclose(student_file);
    }
    
    // 인덱스 파일 업데이트
    save_index_data();
    
    printf("학생 정보가 성공적으로 수정되었습니다.\n");
} 