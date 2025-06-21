CC = gcc
CFLAGS = -std=c99 -Wall -Wextra -Iinclude

# Default target: build all executables
all: score_manage show_score student_info

# Score management program (professor only)
score_manage: src/score_manage/score_management.c \
	include/common.c \
	include/file_io.c \
	include/timestamp.c \
	include/professor_file_io.c
	$(CC) $(CFLAGS) $^ -o $@

# Score viewing program (student only)
show_score: src/score_view/show_score.c \
	include/common.c \
	include/file_io.c \
	include/timestamp.c \
	include/professor_file_io.c
	$(CC) $(CFLAGS) $^ -o $@

# Student info management program (student only)
student_info: src/student_info/student_info_management.c \
	include/common.c \
	include/file_io.c \
	include/timestamp.c \
	include/professor_file_io.c \
	include/index.c \
	include/student_file_io.c
	$(CC) $(CFLAGS) $^ -o $@

# Clean generated binaries
.PHONY: all clean
clean:
	rm -f score_manage show_score student_info

