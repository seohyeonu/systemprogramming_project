#ifndef STUDENT_MANAGEMENT_H
#define STUDENT_MANAGEMENT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <dirent.h>
#include <pwd.h>

#define MAX_NAME 50
#define MAX_DEPT 30
#define MAX_STATUS 20
#define MAX_SUBJECT 50
#define MAX_STUDENTS 1000
#define MAX_SUBJECTS 20

// 학생 정보 구조체
typedef struct {
    int number;
    char student_id[20];
    char name[MAX_NAME];
    char birth_date[15];
    char department[MAX_DEPT];
    char status[MAX_STATUS];
    time_t last_modified;
} Student;

// 교과목 정보 구조체
typedef struct {
    char name[MAX_SUBJECT];
    char department[MAX_DEPT];
    time_t created_time;
} Subject;

// 성적 정보 구조체
typedef struct {
    char student_id[20];
    int score;
    time_t timestamp;
} Grade;

// 교과목별 성적 구조체
typedef struct {
    char subject_name[MAX_SUBJECT];
    Grade grades[MAX_STUDENTS];
    int grade_count;
} SubjectGrade;

// 전역 변수 선언
extern Student students[MAX_STUDENTS];
extern int student_count;
extern Subject subjects[MAX_SUBJECTS];
extern int subject_count;

// 권한 확인 함수
int is_admin();
int is_professor();
int is_student();

// 학생 정보 관리 함수
void student_management_menu();
void add_student();
void modify_student();
void delete_student();
void list_students();

// 성적 관리 함수
void grade_management_menu();
void add_subject();
void remove_subject();
void list_subjects();
void input_grades();
void modify_grades();
void modify_grade_in_file(const char* filename, const char* student_id, int new_score);
void load_subjects();

// 성적 열람 함수
void grade_view_menu();
void view_my_grades();
void create_student_grade_file(const char* student_id);
void create_all_student_grade_files();

// 시스템 유틸리티 함수
void initialize_system();
void remove_directory_contents(const char* dir_path);
void set_file_permissions();
void check_system_status();

// 데이터 관리 함수
void load_index_data();
void save_index_data();
void create_directories();

// 기타 유틸리티 함수
void clear_screen();
void check_user_permission();

#endif // STUDENT_MANAGEMENT_H 