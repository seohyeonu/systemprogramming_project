#include "student_management.h"

// 성적 입력
void input_grades() {
    printf("\n=== 성적 입력 ===\n");
    
    if (subject_count == 0) {
        printf("등록된 교과목이 없습니다.\n");
        return;
    }
    
    printf("교과목 목록:\n");
    for (int i = 0; i < subject_count; i++) {
        printf("%d. %s (%s)\n", i + 1, subjects[i].name, subjects[i].department);
    }
    
    int choice;
    printf("성적을 입력할 교과목 번호: ");
    scanf("%d", &choice);
    getchar();
    
    if (choice < 1 || choice > subject_count) {
        printf("잘못된 선택입니다.\n");
        return;
    }
    
    choice--; // 배열 인덱스로 변환
    
    // 해당 학과 학생들 찾기
    printf("\n%s 학과 학생 목록:\n", subjects[choice].department);
    int enrolled_students[MAX_STUDENTS];
    int enrolled_count = 0;
    
    for (int i = 0; i < student_count; i++) {
        if (strcmp(students[i].department, subjects[choice].department) == 0) {
            enrolled_students[enrolled_count] = i;
            printf("%d. %s - %s\n", enrolled_count + 1, 
                   students[i].student_id, students[i].name);
            enrolled_count++;
        }
    }
    
    if (enrolled_count == 0) {
        printf("해당 학과의 학생이 없습니다.\n");
        return;
    }
    
    // 성적 입력
    printf("\n성적을 입력하세요 (학번 - 점수 형식, 종료하려면 'end' 입력):\n");
    char input[100];
    
    while (1) {
        printf("> ");
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = 0;
        
        if (strcmp(input, "end") == 0) {
            break;
        }
        
        char student_id[20];
        int score;
        
        if (sscanf(input, "%s - %d", student_id, &score) == 2) {
            if (score < 0 || score > 100) {
                printf("점수는 0-100 사이여야 합니다.\n");
                continue;
            }
            
            // 성적 파일에 저장
            char filename[100];
            sprintf(filename, "Grade/%s.dat", subjects[choice].name);
            FILE *grade_file = fopen(filename, "a");
            if (grade_file != NULL) {
                time_t now = time(NULL);
                fprintf(grade_file, "%s %d %ld\n", student_id, score, now);
                fclose(grade_file);
                printf("성적이 저장되었습니다: %s - %d점\n", student_id, score);
            }
        } else {
            printf("잘못된 형식입니다. (학번 - 점수)\n");
        }
    }
} 