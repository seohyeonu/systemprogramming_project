#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "student_management.h"

// 학생 정보 관리 메뉴
void student_management_menu() {
    int choice;
    do {
        printf("\n=== 학생 정보 관리 ===\n");
        printf("1. 학생 정보 입력\n");
        printf("2. 학생 정보 수정\n");
        printf("3. 학생 정보 삭제\n");
        printf("4. 학생 목록 조회\n");
        printf("0. 메인 메뉴로 돌아가기\n");
        printf("선택: ");
        scanf("%d", &choice);
        getchar(); // 버퍼 정리
        
        switch(choice) {
            case 1:
                add_student();
                break;
            case 2:
                modify_student();
                break;
            case 3:
                delete_student();
                break;
            case 4:
                list_students();
                break;
            case 0:
                printf("메인 메뉴로 돌아갑니다.\n");
                break;
            default:
                printf("잘못된 선택입니다.\n");
        }
    } while (choice != 0);
}

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
        
        // 파일 권한 설정 (root 소유)
        chmod(filename, 0644);
    }
    
    // 인덱스 파일 업데이트
    save_index_data();
    
    printf("학생 정보가 성공적으로 추가되었습니다.\n");
    printf("할당된 번호: %d\n", assigned_number);
}

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

// 학생 정보 삭제
void delete_student() {
    char student_id[20];
    printf("\n=== 학생 정보 삭제 ===\n");
    printf("삭제할 학생의 학번: ");
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
    
    printf("학번 %s의 학생 정보를 삭제하시겠습니까? (y/n): ", student_id);
    char confirm;
    scanf("%c", &confirm);
    getchar();
    
    if (confirm == 'y' || confirm == 'Y') {
        // 개별 학생 파일 삭제
        char filename[100];
        sprintf(filename, "Student/%s.dat", student_id);
        remove(filename);
        
        // 배열에서 제거 (번호는 유지)
        for (int i = found; i < student_count - 1; i++) {
            students[i] = students[i + 1];
        }
        student_count--;
        
        // 인덱스 파일 업데이트
        save_index_data();
        
        printf("학생 정보가 삭제되었습니다. (번호 %d는 유지됩니다)\n", students[found].number);
    } else {
        printf("삭제가 취소되었습니다.\n");
    }
}

// 학생 목록 조회
void list_students() {
    printf("\n=== 학생 목록 ===\n");
    printf("%-5s %-12s %-10s %-15s %-15s %-10s %-20s\n",
           "번호", "학번", "이름", "생년월일", "학과", "상태", "최종 수정 시간");
    printf("----------------------------------------------------------------\n");
    
    for (int i = 0; i < student_count; i++) {
        char time_str[20];
        struct tm *tm_info = localtime(&students[i].last_modified);
        strftime(time_str, 20, "%Y-%m-%d %H:%M", tm_info);
        
        printf("%-5d %-12s %-10s %-15s %-15s %-10s %-20s\n",
               students[i].number,
               students[i].student_id,
               students[i].name,
               students[i].birth_date,
               students[i].department,
               students[i].status,
               time_str);
    }
    
    printf("\n총 %d명의 학생이 등록되어 있습니다.\n", student_count);
} 