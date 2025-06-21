#include "student_management.h"

// 성적 관리 메뉴
void grade_management_menu() {
    int choice;
    do {
        printf("\n=== 성적 관리 ===\n");
        printf("1. 교과목 추가\n");
        printf("2. 교과목 제거\n");
        printf("3. 교과목 목록\n");
        printf("4. 성적 입력\n");
        printf("5. 성적 수정\n");
        printf("0. 메인 메뉴로 돌아가기\n");
        printf("선택: ");
        scanf("%d", &choice);
        getchar(); // 버퍼 정리
        
        switch(choice) {
            case 1:
                add_subject();
                break;
            case 2:
                remove_subject();
                break;
            case 3:
                list_subjects();
                break;
            case 4:
                input_grades();
                break;
            case 5:
                modify_grades();
                break;
            case 0:
                printf("메인 메뉴로 돌아갑니다.\n");
                break;
            default:
                printf("잘못된 선택입니다.\n");
        }
    } while (choice != 0);
} 