#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>

#define DATA_DIR       "data/Student"
#define INDEX_PATH     DATA_DIR "/index.dat"
#define ID_LEN         16

typedef struct {
    int    number;
    char   student_id[ID_LEN];
    time_t last_modified;
} IndexEntry;

//─ 유틸리티 ──────────────────────────────────────────────────────────────────
// 디렉토리/파일 존재 검사
int  exists(const char *path) {
    struct stat st;
    return (stat(path, &st) == 0);
}
// 디렉토리 생성
void make_dir(const char *path) {
    if (mkdir(path, 0755) != 0) {
        perror("mkdir");
        exit(1);
    }
}
// 파일 생성 (빈 파일)
void make_file(const char *path) {
    FILE *f = fopen(path, "ab");
    if (!f) { perror("fopen"); exit(1); }
    fclose(f);
}

//─ 초기화 ──────────────────────────────────────────────────────────────────
void init_storage() {
    if (!exists(DATA_DIR)) {
        printf("Student 폴더가 없습니다. 생성합니다...\n");
        make_dir(DATA_DIR);
    }
    if (!exists(INDEX_PATH)) {
        printf("index.dat 파일이 없습니다. 생성합니다...\n");
        make_file(INDEX_PATH);
    }
    // 읽기/쓰기 권한 확인
    if (access(INDEX_PATH, R_OK|W_OK) != 0) {
        fprintf(stderr, "index.dat에 대한 권한이 없습니다.\n");
        exit(1);
    }
}

//─ 인덱스 입출력 ──────────────────────────────────────────────────────────────
IndexEntry *load_index(int *out_n) {
    FILE *f = fopen(INDEX_PATH, "rb");
    if (!f) { *out_n = 0; return NULL; }
    fseek(f, 0, SEEK_END);
    long sz = ftell(f);
    rewind(f);
    int n = sz / sizeof(IndexEntry);
    IndexEntry *arr = malloc(n * sizeof *arr);
    fread(arr, sizeof *arr, n, f);
    fclose(f);
    *out_n = n;
    return arr;
}

void save_index(IndexEntry *arr, int n) {
    FILE *f = fopen(INDEX_PATH, "wb");
    fwrite(arr, sizeof *arr, n, f);
    fclose(f);
}

//─ 학생 추가 모드 ─────────────────────────────────────────────────────────────
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

//─ 학생 수정 모드 ─────────────────────────────────────────────────────────────
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

//─ 메인 메뉴 ─────────────────────────────────────────────────────────────────
int main() {
    init_storage();

    int choice;
    do {
        printf("\n[학생 정보 관리]\n");
        printf("1) 추가  2) 수정  0) 종료\n");
        printf("선택> "); scanf("%d", &choice);
        getchar();

        switch (choice) {
        case 1: add_student();    break;
        case 2: modify_student(); break;
        case 0: puts("프로그램을 종료합니다."); break;
        default: puts("잘못된 선택입니다.");
        }
    } while (choice != 0);

    return 0;
}
