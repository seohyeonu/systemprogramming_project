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

// 사용자 권한 확인
void check_user_permission() {
    uid_t uid = getuid();
    struct passwd *pw = getpwuid(uid);
    
    printf("현재 사용자: %s (UID: %d)\n", pw->pw_name, uid);
    
    if (uid == 0) {
        printf("관리자 권한으로 실행 중\n");
    } else if (uid >= 1000 && uid <= 2000) {
        printf("교수 권한으로 실행 중\n");
    } else {
        printf("학생 권한으로 실행 중\n");
    }
}

// 관리자 권한 확인
int is_admin() {
    return getuid() == 0;
}

// 교수 권한 확인
int is_professor() {
    uid_t uid = getuid();
    return (uid >= 1000 && uid <= 2000);
}

// 학생 권한 확인
int is_student() {
    uid_t uid = getuid();
    return (uid > 2000);
}

// 디렉토리 생성
void create_directories() {
    mkdir("Student", 0755);
    mkdir("Grade", 0755);
    
    // index.dat 파일이 없으면 생성
    FILE *index_file = fopen("Student/index.dat", "r");
    if (index_file == NULL) {
        index_file = fopen("Student/index.dat", "w");
        if (index_file != NULL) {
            fclose(index_file);
            printf("Student/index.dat 파일이 생성되었습니다.\n");
        }
    } else {
        fclose(index_file);
    }
}

// 인덱스 데이터 로드
void load_index_data() {
    FILE *index_file = fopen("Student/index.dat", "r");
    if (index_file == NULL) {
        printf("index.dat 파일을 찾을 수 없습니다.\n");
        return;
    }
    
    student_count = 0;
    while (fscanf(index_file, "%d %s %ld", 
                  &students[student_count].number,
                  students[student_count].student_id,
                  &students[student_count].last_modified) == 3) {
        
        // 개별 학생 파일에서 상세 정보 로드
        char filename[100];
        sprintf(filename, "Student/%s.dat", students[student_count].student_id);
        FILE *student_file = fopen(filename, "r");
        if (student_file != NULL) {
            char temp[100]; // 임시 변수 추가
            fscanf(student_file, "%s %s %s %s %s",
                   students[student_count].name,
                   students[student_count].birth_date,
                   students[student_count].department,
                   students[student_count].status,
                   temp); // 마지막 필드용 임시 변수
            fclose(student_file);
        }
        student_count++;
    }
    fclose(index_file);
    printf("학생 데이터 로드 완료: %d명\n", student_count);
}

// 인덱스 데이터 저장
void save_index_data() {
    FILE *index_file = fopen("Student/index.dat", "w");
    if (index_file == NULL) {
        printf("index.dat 파일을 열 수 없습니다.\n");
        return;
    }
    
    for (int i = 0; i < student_count; i++) {
        fprintf(index_file, "%d %s %ld\n",
                students[i].number,
                students[i].student_id,
                students[i].last_modified);
    }
    fclose(index_file);
}

// 화면 클리어
void clear_screen() {
    system("clear");
} 