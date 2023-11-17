#include "s21_grep.h"

int main(int argc, char** argv) {
  Flags flags = {0};
  char* mass_pattern[100];
  char* mass_files[100];
  char* file_pattern[100];
  int indx_file_pattern = 0;
  int indx_mass_files = 0;
  int indx_pattern = 0;
  int files = 0;
  read_flags(argc, argv, &flags, mass_pattern, &indx_pattern, mass_files,
             &indx_mass_files);
  int indx_file = optind;
  if (flags.f) {
    for (int i = 0; i < indx_mass_files; i++) {
      FILE* ptrF = fopen(mass_files[i], "rt");
      char* line_file = NULL;
      size_t len = 0;
      int read;
      while ((read = getline(&line_file, &len, ptrF)) != -1) {
        file_pattern[indx_file_pattern] = line_file;
        printf("file_pattern: %s\nindx_file_pattern: %d\n",
               file_pattern[indx_file_pattern], indx_file_pattern);
        // //проверка
        indx_file_pattern++;
        // call (argc, argv, files, indx_file, flags, indx_pattern,
        // mass_pattern);
      }
      /*call(argc, argv, files, indx_file, flags, indx_pattern, mass_pattern,
       indx_file_pattern, file_pattern);*/
      for (int i = 0; i < indx_file_pattern; i++) {
        printf("index i: %d\n", i);
        printf("file_pattern call: %s\n", file_pattern[i]);
      }
    }
  }
  // printf("indx_file_pattern: %d\n", indx_file_pattern); //проверка
  // printf("indx_file: %d\n", indx_file); //проверка
  call(argc, argv, files, indx_file, flags, indx_pattern, mass_pattern,
       indx_file_pattern, file_pattern);
  /*if (flags.e) {
    if (indx_file + 1 < argc) files = 1;
    for (int j = indx_file; j < argc; j++) {
      for (int i = 0; i < indx_pattern; i++) {
        // printf("\nmass_pattern %d: %s\n", i, mass_pattern[i]); //проверка
        print_flag_file(flags, argv[j], mass_pattern[i], files);
      }
    }
  } else {
    //тест
    indx_file += 1;
    if (indx_file + 1 < argc) files = 1;
    for (int j = indx_file; j < argc; j++) {
      print_flag_file(flags, argv[j], argv[j - 1], files);
    }
    //тест окончен*/

  /*for (int i = 1; i < argc - 1; i++) {
    if (argv[i][0] != '-' && indx_pattern == 0) {
      indx_pattern = i;
      if (indx_pattern + 2 != argc) files = 1;
    } else if (indx_pattern == 0)
      continue;
    print_flag_file(flags, argv[i + 1], argv[indx_pattern], files);
    // printf("number of call\n"); //проверка
  }*/
  //}
  return 0;
}

void call(int argc, char** argv, int files, int indx_file, Flags flags,
          int indx_pattern, char* mass_pattern[100], int indx_file_pattern,
          char* file_pattern[100]) {
  if (flags.e) {
    if (indx_file + 1 < argc) files = 1;
    for (int j = indx_file; j < argc; j++) {
      for (int i = 0; i < indx_pattern; i++) {
        // printf("\nmass_pattern %d: %s\n", i, mass_pattern[i]); //проверка
        print_flag_file(flags, argv[j], mass_pattern[i], files);
      }
    }
  } else if (!flags.f) {
    indx_file += 1;
    if (indx_file + 1 < argc) files = 1;
    for (int j = indx_file; j < argc; j++) {
      print_flag_file(flags, argv[j], argv[j - 1], files);
    }
  }
  if (flags.f) {
    if (indx_file + 1 < argc) files = 1;
    for (int j = indx_file; j < argc; j++) {
      for (int i = 0; i < indx_file_pattern; i++) {
        printf("index i: %d\n", i);
        print_flag_file(flags, argv[j], file_pattern[i], files);
        printf("file_pattern call: %s\n", file_pattern[i]);
      }
    }
  }
}

void print_flag_file(Flags flags, char* fname, char* pattern, int files) {
  //тут начало эксперимента
  regex_t regex;
  int reti;
  int cflags = 0;
  regmatch_t matchptr[1];
  const size_t nmatch = 1;
  //эксперимент окончен
  FILE* ptrF = fopen(fname, "rt");
  char* line_file = NULL;
  size_t len = 0;
  int read;
  // printf("name of file: %s\n", fname); //проверка
  if (ptrF != NULL) {
    int str_num = 1;
    int match_count = 0;
    while ((read = getline(&line_file, &len, ptrF)) != -1) {
      /*printf("Получена строка длиной %d :\n", read);
      printf("%s", line_file);*/
      if (flags.i) cflags = REG_ICASE;
      reti = regcomp(&regex, pattern, cflags);
      reti = regexec(&regex, line_file, nmatch, matchptr, 0);
      if (flags.l) {
        printf("%s%s%s\n", PUR, fname, RESET);
        break;
      }
      if ((!reti && !flags.v) || (flags.v && reti == REG_NOMATCH)) {
        for (int i = 0; i < read; i++) {
          match_count++;
          if (flags.c) break;
          if (files && i == 0) {
            if (!flags.h) printf("%s%s%s%s:%s", PUR, fname, RESET, BLUE, RESET);
          }
          if (flags.n && i == 0)
            printf("%s%d%s%s:%s", GREEN, str_num, RESET, BLUE,
                   RESET);  //так стало (флаг n)
          if (matchptr[0].rm_so == i && !flags.v) {
            while (i < matchptr[0].rm_eo) {
              printf("%s%c%s", RED, line_file[i], RESET);
              if ((i == read - 1 && line_file[i] != '\n') ||
                  (flags.o && i == matchptr[0].rm_eo - 1))
                printf("\n");
              i++;
            }
          }
          if (!flags.o) printf("%c", line_file[i]);
          if (i == read - 1 && line_file[i] != '\n') printf("\n");
        }
      }
      str_num++;
    }
    if (flags.c)
      printf("%s%s%s%s:%s%d\n", PUR, fname, RESET, BLUE, RESET, match_count);
    fclose(ptrF);
  } else if (!flags.s)
    printf("%s: No such file or directory\n", fname);
}

void read_flags(int argc, char** argv, Flags* flags, char* mass_pattern[100],
                int* indx_pattern, char* mass_files[100],
                int* indx_mass_files) {
  int current_flag = getopt_long(argc, argv, "e:ivclnhsof:", NULL, NULL);
  while (current_flag != -1) {
    switch (current_flag) {
      case 'e':
        flags->e = 1;
        // printf("\nindx_pattern: %d\n", *indx_pattern); //проверка
        mass_pattern[*indx_pattern] = optarg;
        *indx_pattern = *indx_pattern + 1;
        // printf("\noptarg: %s\n", optarg); //проверка
        break;
      case 'i':
        flags->i = 1;
        break;
      case 'v':
        flags->v = 1;
        break;
      case 'c':
        flags->c = 1;
        break;
      case 'l':
        flags->l = 1;
        break;
      case 'n':
        flags->n = 1;
        break;
      case 'h':
        flags->h = 1;
        break;
      case 's':
        flags->s = 1;
        break;
      case 'o':
        flags->o = 1;
        break;
      case 'f':
        flags->f = 1;
        mass_files[*indx_mass_files] = optarg;
        *indx_mass_files = *indx_mass_files + 1;
        break;
    }
    current_flag = getopt_long(argc, argv, "e:ivclnhsof:", NULL, NULL);
  }
}