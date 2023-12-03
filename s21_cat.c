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

int flag_detect(char *argv[], char *flag, int argc) {
  int found = 0;
  for (int i = 0; (i < argc) && !found; ++i) {
    if (strcmp(flag, argv[i]) == 0) {
      found = 2;
    } else {
      for (int j = 1;
           (j < (int)strlen(argv[i])) && (argv[i][0] == '-') && !found; ++j) {
        if ((argv[i][j] == *flag) && ((*flag == 'E') || (*flag == 'T'))) {
          found = 2;
        } else if (argv[i][j] == *flag) {
          found = 1;
        }
      }
    }
  }
  return found;
}

void NumberNonblank(FILE *file, int b, int e, int n, int s, int t, int v) {
  char now[2] = {0}, last[2] = {0};
  int count = 1, combo_n = -1;
  while (fgets(now, 2, file)) {
    if (now[0] == '\n') {
      ++combo_n;
    } else {
      combo_n = -1;
    }
    if ((combo_n < 2) || !s) {
      if ((b || n) && ((last[0] == '\n') || (count == 1))) {
        if (!b || (now[0] != '\n')) {
          printf("%6d\t", count);
          ++count;
        } else if (e) {
          printf("      \t");
        }
      }
      if (e && (now[0] == '\n')) {
        printf("$");
      }
      if ((now[0] == '\t') && t) {
        printf("^");
        now[0] += 64;
      } else if (((now[0] < 32)) && (now[0] != '\n') && (now[0] != '\t') && v) {
        printf("^");
        now[0] += 64;
      } else if ((now[0] > 126) && (now[0] != '\n') && (now[0] != '\t') && v) {
        printf("^");
        now[0] -= 64;
      }
      // } else if ((now[0] >= 32) || (now[0] <= 126) || (now[0] >= 192) ||
      // (now[0] == '\n')) {
      // }
      printf("%c", now[0]);
    }
    last[0] = now[0];
  }
}

void check_flags(int argc, char *argv[], int *condition) {
  for (int i = 1; (i < argc) && *condition; ++i) {
    for (int j = 1; j < (int)strlen(argv[i]) && (argv[i][0] == '-'); ++j) {
      if ((argv[i][j] != 'b') && (argv[i][j] != 'e') && (argv[i][j] != 'n') &&
          (argv[i][j] != 's') && (argv[i][j] != 't') && (argv[i][j] != 'E') &&
          (argv[i][j] != 'T') && (strcmp(argv[i], "--number-nonblank")) &&
          (strcmp(argv[i], "--number")) &&
          (strcmp(argv[i], "--squeeze-blank"))) {
        *condition = 0;
        if (argv[i][1] == '-') {
          fprintf(
              stderr,
              "cat: illegal option -- %s\nusage: cat [-belnstuv] [file ...]",
              argv[i]);
        } else {
          fprintf(
              stderr,
              "cat: illegal option -- %c\nusage: cat [-belnstuv] [file ...]",
              argv[i][j]);
        }
      }
    }
  }
}

void clean_files(FILE **all_files) {
  for (int i = 0; *(all_files + i) != NULL; ++i) {
    fclose(all_files[i]);
  }
  free(all_files);
}

FILE **append_file(FILE **all_files, char *file, int count) {
  if (all_files == NULL) {
    all_files = (FILE **)malloc(sizeof(FILE *) * 2);
    all_files[1] = NULL;
  } else if (count > 0) {
    all_files = (FILE **)realloc(all_files, sizeof(FILE *) * (count + 1));
    all_files[count] = NULL;
  }
  if (all_files != NULL) {
    // all_files[count - 1] = (FILE*)malloc(sizeof(FILE));
    all_files[count - 1] = fopen(file, "r");
    if (all_files[count - 1] == NULL) {
      clean_files(all_files);
      all_files = NULL;
    }
  }
  return all_files;
}

FILE **find_files(int argc, char *argv[], int *file_index) {
  FILE *file = NULL;
  FILE **all_files = NULL;
  int count = 0;
  for (int i = 1; (i < argc); ++i) {
    printf("%d\n", i);
    if ((argv[i] != NULL) && ((int)strlen(argv[i]) > 0)) {
      file = fopen(argv[i], "r");
    }
    if (file != NULL) {
      *file_index = i;
      ++count;
      all_files = append_file(all_files, argv[i], count);
    } else if ((file == NULL) && (argv[i][0] != '-')) {
      fprintf(stderr, "cat: %s: No such file or directory\n", argv[i]);
    }
  }
  if (file != NULL) {
    fclose(file);
  }
  return all_files;
}

int main(int argc, char *argv[]) {
  FILE **files = NULL;
  int condition = 1, file_index = 0, b = 0, e = 0, n = 0, s = 0, t = 0, v = 0;
  files = find_files(argc, argv, &file_index);
  if (files == NULL) {
    condition = 0;
  }

  if (argc < 2) {
    fprintf(stderr, "No such file or directory");
    condition = 0;
  }
  check_flags(file_index, argv, &condition);
  if (condition) {
    b = max(flag_detect(argv, "b", file_index),
            flag_detect(argv, "--number-nonblank", file_index));
    e = max(flag_detect(argv, "e", file_index),
            flag_detect(argv, "E", file_index));
    n = max(flag_detect(argv, "n", file_index),
            flag_detect(argv, "--number", file_index));
    s = max(flag_detect(argv, "s", file_index),
            flag_detect(argv, "--squeeze-blank", file_index));
    t = max(flag_detect(argv, "t", file_index),
            flag_detect(argv, "T", file_index));
    if ((e == 1) || (t == 1)) {
      v = 1;
    } else {
      v = 0;
    }
    // printf("b - %d\n", b);
    // printf("e - %d\n", e);
    // printf("n - %d\n", n);
    // printf("s - %d\n", s);
    // printf("t - %d\n", t);
    // printf("v - %d\n", v);
    for (int i = 0; *(files + i) != NULL; ++i) {
      NumberNonblank(*(files + i), b, e, n, s, t, v);
    }
    // printf("\n");
  }
  if (files != NULL) {
    clean_files(files);
  }
}