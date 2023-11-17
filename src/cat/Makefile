CC = gcc
CFLAG = -std=c11 -Wall -Wextra -Werror
TARGET = s21_cat

build: clean s21_cat

s21_cat: s21_cat.o
	$(CC) $(CFLAG) s21_cat.o -o $(TARGET)

s21_cat.o: s21_cat.c
	$(CC) $(CFLAG) -c s21_cat.c -o s21_cat.o

clean:
	rm -rf *.o $(TARGET)