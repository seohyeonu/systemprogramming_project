#include "student_management.h"

// 성적 열람 메뉴
void grade_view_menu() {
    if (is_student()) {
        view_my_grades();
    } else {
        printf("학생만 성적을 열람할 수 있습니다.\n");
    }
}

// 내 성적 조회 (학생용)
void view_my_grades() {
    uid_t uid = getuid();
    
    // UID를 학번으로 변환 (예: UID 20213348 -> 학번 20213348)
    char my_student_id[20];
    sprintf(my_student_id, "%d", uid);
    
    // 내 학생 정보 찾기
    int my_index = -1;
    for (int i = 0; i < student_count; i++) {
        if (strcmp(students[i].student_id, my_student_id) == 0) {
            my_index = i;
            break;
        }
    }
    
    if (my_index == -1) {
        printf("학생 정보를 찾을 수 없습니다.\n");
        printf("현재 UID: %d, 찾는 학번: %s\n", uid, my_student_id);
        printf("등록된 학생 수: %d명\n", student_count);
        return;
    }
    
    printf("\n=== 내 성적 조회 ===\n");
    printf("학번: %s\n", students[my_index].student_id);
    printf("이름: %s\n", students[my_index].name);
    printf("학과: %s\n", students[my_index].department);
    printf("학적 상태: %s\n", students[my_index].status);
    printf("\n");
    
    // 교과목 목록 로드
    load_subjects();
    
    if (subject_count == 0) {
        printf("등록된 교과목이 없습니다.\n");
        return;
    }
    
    printf("=== 성적 현황 ===\n");
    int has_grades = 0;
    
    // 모든 교과목에서 성적을 찾아보기 (학과 제한 없이)
    for (int i = 0; i < subject_count; i++) {
        char filename[100];
        sprintf(filename, "Grade/%s.dat", subjects[i].name);
        
        FILE *grade_file = fopen(filename, "r");
        if (grade_file != NULL) {
            char line[256];
            while (fgets(line, sizeof(line), grade_file)) {
                if (line[0] != '#') { // 주석이 아닌 경우
                    char student_id[20];
                    int score;
                    time_t timestamp;
                    
                    if (sscanf(line, "%s %d %ld", student_id, &score, &timestamp) == 3) {
                        if (strcmp(student_id, my_student_id) == 0) {
                            char time_str[20];
                            struct tm *tm_info = localtime(&timestamp);
                            strftime(time_str, 20, "%Y-%m-%d %H:%M", tm_info);
                            
                            printf("%s: %d점 (입력시간: %s)\n", 
                                   subjects[i].name, score, time_str);
                            has_grades = 1;
                            break;
                        }
                    }
                }
            }
            fclose(grade_file);
        }
    }
    
    if (!has_grades) {
        printf("아직 입력된 성적이 없습니다.\n");
        printf("찾는 학번: %s\n", my_student_id);
        printf("등록된 교과목 수: %d개\n", subject_count);
    }
} 