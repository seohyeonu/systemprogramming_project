#include "student_info.h" 
void modify_student() {
    int n;
    IndexEntry *idx = load_index(&n);

    int no;
    printf("수정할 번호를 입력하세요: ");
    scanf("%d", &no);
    getchar();

    // 인덱스에서 번호 검색
    int i;
    for (i = 0; i < n; i++) {
        if (idx[i].number == no) break;
    }
    if (i == n) {
        printf("해당 번호의 학생이 없습니다.\n");
        free(idx);
        return;
    }

    // 해당 student_id 로 .dat 파일 로드
    char path[128];
    snprintf(path, sizeof path, "%s/%s.dat", DATA_DIR, idx[i].student_id);
    // FILE *sf = fopen(path, "rb+");
    // TODO: 파일에서 Student 구조체 읽어와 수정 → 덮어쓰기
    // fclose(sf);

    // 인덱스 최종 수정 시간 갱신
    idx[i].last_modified = time(NULL);
    save_index(idx, n);
    free(idx);

    printf("학생[%s] 정보 수정 완료\n", idx[i].student_id);
}