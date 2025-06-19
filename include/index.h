#ifndef INDEX_H
#define INDEX_H

#include <stdbool.h>

bool add_index_entry(const char* student_id, const char* timestamp);
bool index_has_student(const char* student_id);
bool delete_index_entry(const char* student_id);
bool update_index_timestamp(const char* student_id, const char* new_timestamp);

#endif // INDEX_H