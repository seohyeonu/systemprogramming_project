#include "student_management.h"

// 파일에서 성적 수정 실행
void modify_grade_in_file(const char* filename, const char* student_id, int new_score) {
    // 임시 파일에 복사하면서 수정
    char temp_filename[100];
    sprintf(temp_filename, "%s.tmp", filename);
    
    FILE *temp_file = fopen(temp_filename, "w");
    FILE *original_file = fopen(filename, "r");
    
    if (temp_file != NULL && original_file != NULL) {
        char line[256];
        int found = 0;
        
        while (fgets(line, sizeof(line), original_file)) {
            if (line[0] == '#') {
                fputs(line, temp_file); // 주석은 그대로 복사
            } else {
                char current_id[20];
                int current_score;
                time_t current_timestamp;
                
                if (sscanf(line, "%s %d %ld", current_id, &current_score, &current_timestamp) == 3) {
                    if (strcmp(current_id, student_id) == 0) {
                        // 해당 학생의 성적을 새로운 점수로 수정
                        time_t now = time(NULL);
                        fprintf(temp_file, "%s %d %ld\n", student_id, new_score, now);
                        found = 1;
                        printf("성적이 수정되었습니다: %s - %d점\n", student_id, new_score);
                    } else {
                        fputs(line, temp_file); // 다른 학생은 그대로 복사
                    }
                } else {
                    fputs(line, temp_file); // 파싱 실패한 라인도 그대로 복사
                }
            }
        }
        
        if (!found) {
            printf("해당 학번의 성적을 찾을 수 없습니다.\n");
        }
        
        fclose(temp_file);
        fclose(original_file);
        
        // 임시 파일을 원본 파일로 교체
        remove(filename);
        rename(temp_filename, filename);
    }
} 