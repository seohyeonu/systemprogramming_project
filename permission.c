#include "student_management.h"

// 사용자 권한 확인
void check_user_permission() {
    uid_t uid = getuid();
    struct passwd *pw = getpwuid(uid);
    
    printf("현재 사용자: %s (UID: %d)\n", pw->pw_name, uid);
    
    if (uid == 0) {
        printf("관리자 권한으로 실행 중\n");
    } else if (uid >= 1000 && uid <= 1002) {
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
    return (uid >= 1000 && uid <= 1002);
}

// 학생 권한 확인
int is_student() {
    uid_t uid = getuid();
    return (uid > 1002);
} 