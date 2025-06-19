#include "student_info.h" 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>

void add_student() {
    int n;
    IndexEntry *idx = load_index(&n);

    // 1) 새 번호 결정
    int new_no = (n>0? idx[n-1].number+1 : 1);

    // 2) 정보 입력
    char sid[ID_LEN];
    printf("학번: ");  scanf("%15s", sid);
    getchar(); // 버퍼정리

    // TODO: 이름/생년월일/학과/학적 상태 입력 및 Student 구조체에 저장
    // 시간 기록
    time_t now = time(NULL);

    // 3) 학생 파일 저장
    char path[128];
    snprintf(path, sizeof path, "%s/%s.dat", DATA_DIR, sid);
    FILE *sf = fopen(path, "wb");
    // fwrite(&student, sizeof student, 1, sf);
    fclose(sf);

    // 4) index.dat 업데이트
    idx = realloc(idx, (n+1)*sizeof *idx);
    idx[n].number        = new_no;
    strncpy(idx[n].student_id, sid, ID_LEN);
    idx[n].last_modified = now;
    save_index(idx, n+1);
    free(idx);

    printf("학생[%s] 추가 완료 (번호 %d)\n", sid, new_no);
}