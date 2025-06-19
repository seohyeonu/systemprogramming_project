#include <sys/types.h>  // uid_t 정의
#include <unistd.h>     // getuid(), 기타 POSIX 함수
#ifndef COMMON_H
#define COMMON_H

// 오류 발생 시 메시지를 출력하고 프로그램 종료
void error_exit(const char* message);

// 로그 메시지를 현재 시간과 함께 출력
void log_message(const char* message);
int parse_comma_separated(const char *input, char output[][64], int max_count);
void require_professor();
void require_student();
int is_professor_uid(uid_t uid);
int is_student_uid(uid_t uid);


#endif // COMMON_H
