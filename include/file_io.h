#ifndef FILE_IO_H
#define FILE_IO_H

#include <stdio.h>
#include <stdbool.h>

// 파일 열기 (읽기 실패 시 새로 생성)
FILE* open_file(const char* path, const char* mode);

// 파일 존재 여부 확인
bool file_exists(const char* path);

// 파일 닫기
void close_file(FILE* fp);

// Student 폴더 생성 보장
void ensure_student_directory_exists();
// grade 폴더 생성 보장
void ensure_grade_directory_exists();
void ensure_professor_directory_exists();

#endif // FILE_IO_H