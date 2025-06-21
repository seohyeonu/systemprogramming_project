CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -O2
TARGET = student_management
SOURCES = main.c permission.c data_manager.c student_management.c student_operations.c student_modify.c student_delete.c grade_management.c subject_management.c subject_operations.c subject_remove.c grade_input.c grade_operations.c grade_modify.c grade_view.c system_utils.c system_status.c
OBJECTS = $(SOURCES:.c=.o)
HEADERS = student_management.h

.PHONY: all clean install uninstall

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)
	rm -rf Student Grade

install: $(TARGET)
	@echo "관리자 권한으로 설치합니다..."
	sudo cp $(TARGET) /usr/local/bin/
	sudo chown root:root /usr/local/bin/$(TARGET)
	sudo chmod 755 /usr/local/bin/$(TARGET)
	@echo "설치 완료: /usr/local/bin/$(TARGET)"

uninstall:
	@echo "프로그램을 제거합니다..."
	sudo rm -f /usr/local/bin/$(TARGET)
	@echo "제거 완료"

# 테스트용 타겟
test: $(TARGET)
	@echo "테스트 실행 중..."
	@echo "관리자 권한으로 실행:"
	sudo ./$(TARGET)
	@echo "교수 권한으로 실행 (UID 1000):"
	sudo -u \#1000 ./$(TARGET)
	@echo "학생 권한으로 실행 (UID 20213348):"
	sudo -u \#20213348 ./$(TARGET)

# 개발용 타겟
dev: CFLAGS += -g -DDEBUG
dev: $(TARGET)

# 릴리즈용 타겟
release: CFLAGS += -DNDEBUG
release: $(TARGET)

help:
	@echo "사용 가능한 타겟:"
	@echo "  all       - 프로그램 컴파일 (기본)"
	@echo "  clean     - 컴파일된 파일들 정리"
	@echo "  install   - 시스템에 설치 (관리자 권한 필요)"
	@echo "  uninstall - 시스템에서 제거"
	@echo "  test      - 다양한 권한으로 테스트 실행"
	@echo "  dev       - 디버그 모드로 컴파일"
	@echo "  release   - 릴리즈 모드로 컴파일"
	@echo "  help      - 이 도움말 표시" 