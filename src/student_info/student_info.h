#include <time.h>
#define DATA_DIR       "../../data/Student"
#define INDEX_PATH     DATA_DIR "/index.dat"
#define ID_LEN         16


typedef struct {
    int    number;
    char   student_id[ID_LEN];
    time_t last_modified;
} IndexEntry;