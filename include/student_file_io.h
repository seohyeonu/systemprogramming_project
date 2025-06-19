#ifndef STUDENT_FILE_IO_H
#define STUDENT_FILE_IO_H

#include <stdbool.h>

bool create_student_file(const char* student_id, const char* name, const char* birth, const char* department, const char* status);
bool update_student_status(const char* student_id, const char* new_status);
bool update_student_info(const char* student_id, const char* new_name, const char* new_birth, const char* new_department, const char* new_status);
bool delete_student_file(const char* student_id);

#endif // STUDENT_FILE_IO_H
