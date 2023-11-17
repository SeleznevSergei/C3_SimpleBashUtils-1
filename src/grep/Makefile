CC = gcc
CFLAG = -std=c11 -Wall -Wextra -Werror
TARGET = s21_grep

build: clean s21_grep

s21_grep: s21_grep.o
	$(CC) $(CFLAG) s21_grep.o -o $(TARGET)

s21_grep.o: s21_grep.c
	$(CC) $(CFLAG) -c s21_grep.c -o s21_grep.o

clean:
	rm -rf *.o $(TARGET)