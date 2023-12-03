CFLAGS=-Wall -Werror -Wextra
CC=gcc

s21_cat:
	$(CC) s21_cat.c $(CFLAGS) -o s21_cat

rebuild:
	rm -rf s21_cat
	$(CC) s21_cat.c $(CFLAGS) -o s21_cat

clean:
	rm -rf s21_cat

style:
	clang-format -style=Google -n *.c
	clang-format -style=Google -i *.c

.PHONY: s21_cat