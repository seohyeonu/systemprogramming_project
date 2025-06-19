#ifndef COMMON_H
#define COMMON_H

// 오류 발생 시 메시지를 출력하고 프로그램 종료
void error_exit(const char* message);

// 로그 메시지를 현재 시간과 함께 출력
void log_message(const char* message);
int parse_comma_separated(const char *input, char output[][64], int max_count);

#endif // COMMON_H
