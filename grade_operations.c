#include "student_management.h"

// 성적 수정
void modify_grades() {
    printf("\n=== 성적 수정 ===\n");
    
    if (subject_count == 0) {
        printf("등록된 교과목이 없습니다.\n");
        return;
    }
    
    printf("교과목 목록:\n");
    for (int i = 0; i < subject_count; i++) {
        printf("%d. %s (%s)\n", i + 1, subjects[i].name, subjects[i].department);
    }
    
    int choice;
    printf("성적을 수정할 교과목 번호: ");
    scanf("%d", &choice);
    getchar();
    
    if (choice < 1 || choice > subject_count) {
        printf("잘못된 선택입니다.\n");
        return;
    }
    
    choice--; // 배열 인덱스로 변환
    
    char filename[100];
    sprintf(filename, "Grade/%s.dat", subjects[choice].name);
    
    // 현재 성적 표시
    printf("\n현재 성적:\n");
    FILE *grade_file = fopen(filename, "r");
    if (grade_file != NULL) {
        char line[256];
        while (fgets(line, sizeof(line), grade_file)) {
            if (line[0] != '#') { // 주석이 아닌 경우
                char student_id[20];
                int score;
                time_t timestamp;
                if (sscanf(line, "%s %d %ld", student_id, &score, &timestamp) == 3) {
                    char time_str[20];
                    struct tm *tm_info = localtime(&timestamp);
                    strftime(time_str, 20, "%Y-%m-%d %H:%M", tm_info);
                    printf("%s - %d점 (입력시간: %s)\n", student_id, score, time_str);
                }
            }
        }
        fclose(grade_file);
    }
    
    // 성적 수정
    char student_id[20];
    printf("\n수정할 학생의 학번: ");
    scanf("%s", student_id);
    getchar();
    
    int new_score;
    printf("새로운 점수: ");
    scanf("%d", &new_score);
    getchar();
    
    if (new_score < 0 || new_score > 100) {
        printf("점수는 0-100 사이여야 합니다.\n");
        return;
    }
    
    // 성적 수정 실행
    modify_grade_in_file(filename, student_id, new_score);
} 