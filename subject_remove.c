#include "student_management.h"

// 교과목 제거
void remove_subject() {
    printf("\n=== 교과목 제거 ===\n");
    
    if (subject_count == 0) {
        printf("등록된 교과목이 없습니다.\n");
        return;
    }
    
    printf("등록된 교과목:\n");
    for (int i = 0; i < subject_count; i++) {
        printf("%d. %s (%s)\n", i + 1, subjects[i].name, subjects[i].department);
    }
    
    int choice;
    printf("제거할 교과목 번호: ");
    scanf("%d", &choice);
    getchar();
    
    if (choice < 1 || choice > subject_count) {
        printf("잘못된 선택입니다.\n");
        return;
    }
    
    choice--; // 배열 인덱스로 변환
    
    printf("교과목 '%s'을(를) 제거하시겠습니까? (y/n): ", subjects[choice].name);
    char confirm;
    scanf("%c", &confirm);
    getchar();
    
    if (confirm == 'y' || confirm == 'Y') {
        // 파일 삭제
        char filename[100];
        sprintf(filename, "Grade/%s.dat", subjects[choice].name);
        remove(filename);
        
        // 배열에서 제거
        for (int i = choice; i < subject_count - 1; i++) {
            subjects[i] = subjects[i + 1];
        }
        subject_count--;
        
        printf("교과목이 제거되었습니다.\n");
    } else {
        printf("제거가 취소되었습니다.\n");
    }
} 