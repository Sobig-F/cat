#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int max(int a, int b) {
    if (a > b) {
        return a;
    } else {
        return b;
    }
}

int flag_detect(char **flags, char *flag, int argc) {
    int result = 0;
    for (int i = 0; (i < argc) && (result == 0); ++i) {
        if (strcmp(*(flags + i), flag) == 0) {
            result = 1;
        }
    }
    return result;
}

void NumberNonblank(FILE *file, int b, int e, int n, int s, int t) {
    char now[2], last[2];
    int count = 1;
    while (fgets(now, 2, file)) {
        if ((b || n) && ((last[0] == '\n') || (count == 1))) {
            if ((b == 0) || (now[0] != '\n')) {
                printf("     %d  ", count);
                ++count;
            }
        }
        if (e && (now[0] == '\n')) {
            printf("$");
        }
        if ((now[0] != '\n') || (last[0] != '\n') || (s == 0)) {
            if ((now[0] == '\t') && t) {
                printf("^|");
            } else {
                printf("%c", now[0]);
            }
        }
        last[0] = now[0];
    }
}

int main(int argc, char *argv[]) {
    FILE *file = fopen(argv[argc - 1], "r");
    int condition = 1, b, e, n, s, t;
    if (argc < 3) {
        printf("no arguments");
        condition = 0;
    }
    if ((condition) && (file == NULL)) {
        printf("file not found");
        condition = 0;
    }
    if (condition) {
        b = max(flag_detect(argv, "-b", argc), flag_detect(argv, "--number-nonblank", argc));
        e = max(flag_detect(argv, "-e", argc), flag_detect(argv, "-E", argc));
        n = max(flag_detect(argv, "-n", argc), flag_detect(argv, "--number", argc));
        s = max(flag_detect(argv, "-s", argc), flag_detect(argv, "--squeeze-blank", argc));
        t = max(flag_detect(argv, "-t", argc), flag_detect(argv, "-T", argc));
        NumberNonblank(file, b, e, n, s, t);
    }
}
