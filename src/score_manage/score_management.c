#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <errno.h>

#include "../../include/common.h"            // parse_comma_separated
#include "../../include/file_io.h"           // open_file, close_file
#include "../../include/timestamp.h"         // get_current_timestamp
#include "../../include/professor_file_io.h" // create_grade_file, delete_grade_file

#define PROFESSOR_DIR "../../data/professors"
#define STUDENT_DIR   "../../data/Student"
#define GRADE_DIR     "../../data/Grade"
#define MAX_SUBJECTS  10
#define MAX_NAME_LEN  64
#define MAX_STUDENTS 1000

void input_scores_for_subject(const char *subject, const char *prof_dept);
void modify_score_for_subject(const char *subject);

int main() {
    require_professor();
    // Ensure necessary directories exist
    struct stat st = {0};
    if (stat(PROFESSOR_DIR, &st) == -1) mkdir(PROFESSOR_DIR, 0755);
    if (stat(GRADE_DIR, &st) == -1) mkdir(GRADE_DIR, 0755);

    // Setup or load professor profile
    char professor_id[MAX_NAME_LEN];
    snprintf(professor_id, sizeof(professor_id), "uid_%d", getuid());
    setup_professor_profile(professor_id);

    // Load department and subject list
    char profile_path[128];
    snprintf(profile_path, sizeof(profile_path), "%s/%s.dat", PROFESSOR_DIR, professor_id);
    FILE *pf = open_file(profile_path, "r");
    if (!pf) return 1;

    char line[256];
    char department[MAX_NAME_LEN] = "";
    char subjects[MAX_SUBJECTS][MAX_NAME_LEN];
    int subject_count = 0;

    while (fgets(line, sizeof(line), pf)) {
        if (strncmp(line, "학과:", strlen("학과:")) == 0) {
            char *p = line + strlen("학과:");
            trim_whitespace(p);
            strncpy(department, p, sizeof(department)-1);
            department[sizeof(department)-1] = '\0';
        } else if (strncmp(line, "교과목:", strlen("교과목:")) == 0) {
            char *p = line + strlen("교과목:");
            trim_whitespace(p);
            subject_count = parse_comma_separated(p, subjects, MAX_SUBJECTS);
        }
    }
    close_file(pf);

    // Main menu loop
    int choice;
    while (1) {
        printf("\n[성적 관리 시스템] 학과: %s\n", department);
        for (int i = 0; i < subject_count; i++) {
            printf("  %d. %s\n", i+1, subjects[i]);
        }
        printf("  %d. 과목 추가\n", subject_count+1);
        printf("  %d. 과목 삭제\n", subject_count+2);
        printf("  0. 종료\n");
        printf("선택: ");
        if (scanf("%d", &choice) != 1) {
            fprintf(stderr, "입력 오류\n");
            break;
        }
        if (choice == 0) break;
        else if (choice == subject_count+1) {
            add_subject();
            // Reload subjects from profile file
            {
                FILE *pf2 = open_file(profile_path, "r");
                char line2[256];
                while (fgets(line2, sizeof(line2), pf2)) {
                    if (strncmp(line2, "교과목:", strlen("교과목:")) == 0) {
                        char *p = line2 + strlen("교과목:");
                        trim_whitespace(p);
                        subject_count = parse_comma_separated(p, subjects, MAX_SUBJECTS);
                        break;
                    }
                }
                close_file(pf2);
            }
        }
        else if (choice == subject_count+2) {
            delete_subject();
            // Reload subjects from profile file
            {
                FILE *pf2 = open_file(profile_path, "r");
                char line2[256];
                while (fgets(line2, sizeof(line2), pf2)) {
                    if (strncmp(line2, "교과목:", strlen("교과목:")) == 0) {
                        char *p = line2 + strlen("교과목:");
                        trim_whitespace(p);
                        subject_count = parse_comma_separated(p, subjects, MAX_SUBJECTS);
                        break;
                    }
                }
                close_file(pf2);
            }
        }
        else if (choice >= 1 && choice <= subject_count) {
            const char *subject = subjects[choice-1];
            int action;
            printf("'%s' 교과목 선택됨. 1) 성적 입력  2) 성적 수정\n선택: ", subject);
            if (scanf("%d", &action) != 1) continue;
            if (action == 1) input_scores_for_subject(subject, department);
            else if (action == 2) modify_score_for_subject(subject);
            else printf("잘못된 선택입니다.\n");
        }
        else {
            printf("잘못된 선택입니다.\n");
        }
    }

    return 0;
}

void input_scores_for_subject(const char *subject, const char *prof_dept) {
    // Open or create grade file
    char grade_path[128];
    snprintf(grade_path, sizeof(grade_path), "%s/%s.dat", GRADE_DIR, subject);
    FILE *gfp = open_file(grade_path, "r+");
    if (!gfp) return;

    // Read existing student IDs
    int existing_ids[MAX_STUDENTS], exist_count=0;
    char buf[256];
    while (fgets(buf, sizeof(buf), gfp)) {
        int sid;
        if (sscanf(buf, "%d", &sid) == 1) existing_ids[exist_count++] = sid;
    }

    // Iterate over student files
    DIR *d = opendir(STUDENT_DIR);
    if (!d) {
        perror("학생 디렉터리 열기 실패");
        close_file(gfp);
        return;
    }
    struct dirent *entry;
    while ((entry = readdir(d)) != NULL) {
        if (!strstr(entry->d_name, ".dat")) continue;
        char student_path[128];
        snprintf(student_path, sizeof(student_path), "%s/%s", STUDENT_DIR, entry->d_name);
        FILE *sfp = open_file(student_path, "r");
        if (!sfp) continue;

        char idstr[MAX_NAME_LEN]="", dept[MAX_NAME_LEN]="", line2[256];
        while (fgets(line2, sizeof(line2), sfp)) {
            if (strncmp(line2,"학번:",strlen("학번:"))==0) {
                sscanf(line2+strlen("학번:"), "%s", idstr);
            } else if (strncmp(line2,"학과:",strlen("학과:"))==0) {
                char *p = line2+strlen("학과:");
                trim_whitespace(p);
                strncpy(dept,p,sizeof(dept)-1);
                dept[sizeof(dept)-1]='\0';
            }
        }
        close_file(sfp);
        if (strcmp(dept, prof_dept)!=0) continue;

        int sid = atoi(idstr);
        int has_grade = 0;
        for (int i=0;i<exist_count;i++){
            if (existing_ids[i]==sid){ has_grade=1; break; }
        }
        if (has_grade) continue;

        int score;
        printf("학생 %s 성적 입력: ", idstr);
        if (scanf("%d", &score)!=1) continue;
        // write new grade entry
        char ts_buf[64];
        get_current_timestamp(ts_buf, sizeof(ts_buf));
        fprintf(gfp, "%d %d %s\n", sid, score, ts_buf);
    }
    closedir(d);
    close_file(gfp);
}

void modify_score_for_subject(const char *subject) {
    char grade_path[128];
    snprintf(grade_path, sizeof(grade_path), "%s/%s.dat", GRADE_DIR, subject);

    // Load all grade entries
    FILE *gfp = open_file(grade_path, "r");
    if (!gfp) return;
    char **lines = NULL;
    size_t count=0;
    char buf[256];
    while (fgets(buf, sizeof(buf), gfp)) {
        lines = realloc(lines, sizeof(char*)*(count+1));
        lines[count] = strdup(buf);
        count++;
    }
    close_file(gfp);

    int target_id, new_score;
    printf("수정할 학번 입력: ");
    if (scanf("%d", &target_id)!=1) {
        for(size_t i=0;i<count;i++) free(lines[i]);
        free(lines);
        return;
    }

    // Rewrite file with updated score
    gfp = open_file(grade_path, "w");
    for (size_t i=0;i<count;i++) {
        int sid, score;
        char old_ts[MAX_NAME_LEN];
        if (sscanf(lines[i],"%d %d %s",&sid,&score,old_ts)==3 && sid==target_id) {
            printf("새 성적 입력: ");
            if (scanf("%d",&new_score)==1) {
                char ts_buf[64];
                get_current_timestamp(ts_buf,sizeof(ts_buf));
                fprintf(gfp,"%d %d %s\n",sid,new_score,ts_buf);
            }
        } else {
            fputs(lines[i],gfp);
        }
        free(lines[i]);
    }
    free(lines);
    close_file(gfp);
}