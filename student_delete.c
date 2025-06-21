#include "student_management.h"

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