#include "student_management.h"

// 학생 정보 입력
void add_student() {
    if (student_count >= MAX_STUDENTS) {
        printf("최대 학생 수에 도달했습니다.\n");
        return;
    }
    
    Student new_student;
    
    printf("\n=== 학생 정보 입력 ===\n");
    
    // 학번 입력
    printf("학번: ");
    scanf("%s", new_student.student_id);
    getchar();
    
    // 중복 학번 확인
    for (int i = 0; i < student_count; i++) {
        if (strcmp(students[i].student_id, new_student.student_id) == 0) {
            printf("이미 존재하는 학번입니다.\n");
            return;
        }
    }
    
    // 이름 입력
    printf("이름: ");
    scanf("%s", new_student.name);
    getchar();
    
    // 생년월일 입력
    printf("생년월일 (YYYY-MM-DD): ");
    scanf("%s", new_student.birth_date);
    getchar();
    
    // 학과 입력
    printf("학과: ");
    scanf("%s", new_student.department);
    getchar();
    
    // 학적 상태 입력
    printf("학적 상태 (재학/휴학/졸업): ");
    scanf("%s", new_student.status);
    getchar();
    
    // 번호 할당 (삭제된 번호 우선 사용)
    int assigned_number = -1;
    for (int i = 1; i <= student_count + 1; i++) {
        int found = 0;
        for (int j = 0; j < student_count; j++) {
            if (students[j].number == i) {
                found = 1;
                break;
            }
        }
        if (!found) {
            assigned_number = i;
            break;
        }
    }
    
    new_student.number = assigned_number;
    new_student.last_modified = time(NULL);
    
    // 학생 정보를 배열에 추가
    students[student_count] = new_student;
    student_count++;
    
    // 개별 학생 파일 생성
    char filename[100];
    sprintf(filename, "Student/%s.dat", new_student.student_id);
    FILE *student_file = fopen(filename, "w");
    if (student_file != NULL) {
        fprintf(student_file, "%s %s %s %s %s\n",
                new_student.name,
                new_student.birth_date,
                new_student.department,
                new_student.status,
                "");
        fclose(student_file);
        chmod(filename, 0644);
    }
    
    // 인덱스 파일 업데이트
    save_index_data();
    
    printf("학생 정보가 성공적으로 추가되었습니다.\n");
    printf("할당된 번호: %d\n", assigned_number);
} 