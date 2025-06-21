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