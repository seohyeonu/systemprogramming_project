#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>

#define DATA_DIR       "../../data/Student"
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
