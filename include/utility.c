#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "student_info.h"  // IndexEntry, ID_LEN 정의 필요

#define INDEX_PATH "data/Student/index.dat"

/**
 * index.dat 파일에서 주어진 학번(student_id)이 존재하는지 확인합니다.
 * @param student_id 검색할 학번 문자열
 * @return 존재하면 true, 없으면 false
 */
bool index_has_student(const char *student_id) {
    FILE *fp = fopen(INDEX_PATH, "rb");
    if (!fp) {
        perror("fopen");
        return false;
    }

    IndexEntry entry;
    while (fread(&entry, sizeof(IndexEntry), 1, fp) == 1) {
        if (strncmp(entry.student_id, student_id, ID_LEN) == 0) {
            fclose(fp);
            return true;
        }
    }

    fclose(fp);
    return false;
}
