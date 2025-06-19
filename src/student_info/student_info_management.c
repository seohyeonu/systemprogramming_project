#include <stdio.h>
#include <string.h>
#include "../../include/file_io.h"
#include "../../include/index.h"
#include "../../include/timestamp.h"
#include "../../include/student_file_io.h"

void add_student() {
    char id[16], name[32], birth[16], dept[32], status[16], timestamp[32];
    printf("학번: "); scanf("%s", id);

    if (index_has_student(id)) {
        printf("이미 등록된 학번입니다.\n");
        return;
    }

    printf("이름: "); scanf("%s", name);
    printf("생년월일(YYYY-MM-DD): "); scanf("%s", birth);
    printf("학과: "); scanf("%s", dept);
    printf("학적 상태(재학/휴학/졸업): "); scanf("%s", status);

    get_current_timestamp(timestamp, sizeof(timestamp));
    create_student_file(id, name, birth, dept, status);
    add_index_entry(id, timestamp);
    printf("학생 정보가 등록되었습니다.\n");
}

void modify_student() {
    char id[16], name[32], birth[16], dept[32], status[16], timestamp[32];
    printf("수정할 학생의 학번: "); scanf("%s", id);

    if (!index_has_student(id)) {
        printf("해당 학번의 학생 정보가 존재하지 않습니다.\n");
        return;
    }

    printf("새 이름: "); scanf("%s", name);
    printf("새 생년월일(YYYY-MM-DD): "); scanf("%s", birth);
    printf("새 학과: "); scanf("%s", dept);
    printf("새 학적 상태: "); scanf("%s", status);

    get_current_timestamp(timestamp, sizeof(timestamp));
    update_student_info(id, name, birth, dept, status);
    update_index_timestamp(id, timestamp);
    printf("학생 정보가 수정되었습니다.\n");
}

void delete_student() {
    char id[16];
    printf("삭제할 학생의 학번: "); scanf("%s", id);

    if (!index_has_student(id)) {
        printf("해당 학번의 학생 정보가 존재하지 않습니다.\n");
        return;
    }

    delete_index_entry(id);

    if (delete_student_file(id)) {
        printf("학생 정보 파일이 삭제되었습니다.\n");
    } else {
        perror("학생 정보 파일 삭제 실패");
    }

    printf("학생 인덱스에서 정보가 삭제되었습니다.\n");
}

int main() {
    int choice;

    ensure_student_directory_exists(); // Ensure directory exists before any operation

    while (1) {
        printf("\n[학생 정보 관리]\n");
        printf("1. 학생 추가\n");
        printf("2. 학생 정보 수정\n");
        printf("3. 학생 삭제\n");
        printf("0. 종료\n");
        printf("선택: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: add_student(); break;
            case 2: modify_student(); break;
            case 3: delete_student(); break;
            case 0: return 0;
            default: printf("잘못된 선택입니다.\n");
        }
    }

    return 0;
}
