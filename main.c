#include "student_management.h"

// 전역 변수 정의
Student students[MAX_STUDENTS];
int student_count = 0;
Subject subjects[MAX_SUBJECTS];
int subject_count = 0;

int main() {
    clear_screen();
    printf("=== 학생 정보 관리 시스템 ===\n");
    
    // 사용자 권한 확인
    check_user_permission();
    
    // 디렉토리 생성
    create_directories();
    
    // 인덱스 데이터 로드
    load_index_data();
    
    // 교과목 데이터 로드
    load_subjects();
    
    int choice;
    do {
        printf("\n=== 메인 메뉴 ===\n");
        printf("1. 학생 정보 관리\n");
        printf("2. 성적 관리\n");
        printf("3. 성적 열람\n");
        printf("4. 시스템 초기화\n");
        printf("5. 시스템 상태 확인\n");
        printf("0. 종료\n");
        printf("선택: ");
        scanf("%d", &choice);
        getchar(); // 버퍼 정리
        
        switch(choice) {
            case 1:
                if (is_admin()) {
                    student_management_menu();
                } else {
                    printf("관리자만 접근 가능합니다.\n");
                }
                break;
            case 2:
                if (is_admin() || is_professor()) {
                    grade_management_menu();
                } else {
                    printf("관리자 또는 교수만 접근 가능합니다.\n");
                }
                break;
            case 3:
                grade_view_menu();
                break;
            case 4:
                if (is_admin()) {
                    initialize_system();
                } else {
                    printf("관리자만 접근 가능합니다.\n");
                }
                break;
            case 5:
                check_system_status();
                break;
            case 0:
                printf("프로그램을 종료합니다.\n");
                break;
            default:
                printf("잘못된 선택입니다.\n");
        }
    } while (choice != 0);
    
    return 0;
} 