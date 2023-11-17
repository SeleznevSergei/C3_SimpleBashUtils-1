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
      indx_file_pattern = 0;
      FILE* ptrF = fopen(mass_files[i], "rt");
      char* line_file = NULL;
      size_t len = 0;
      int read;
      if (ptrF != NULL) {
        while ((read = getline(&line_file, &len, ptrF)) != -1) {
          file_pattern[indx_file_pattern] = line_file;
          line_file = NULL;
          indx_file_pattern++;
        }
        call(argc, argv, files, indx_file, flags, indx_pattern, mass_pattern,
             indx_file_pattern, file_pattern);
        for (int i = 0; i < indx_file_pattern; i++) {
          free(line_file);
        }
        fclose(ptrF);
      } else
        fprintf(stderr, "grep: %s: No such file or directory\n", mass_files[i]);
    }
  } else
    call(argc, argv, files, indx_file, flags, indx_pattern, mass_pattern,
         indx_file_pattern, file_pattern);
  return 0;
}

void call(int argc, char** argv, int files, int indx_file, Flags flags,
          int indx_pattern, char* mass_pattern[100], int indx_file_pattern,
          char* file_pattern[100]) {
  if (flags.e) {
    if (indx_file + 1 < argc) files = 1;
    for (int j = indx_file; j < argc; j++) {
      for (int i = 0; i < indx_pattern; i++) {
        print_flag_file(flags, argv[j], mass_pattern[i], files);
      }
    }
  } else if (!flags.f) {
    if (indx_file + 2 < argc) files = 1;
    for (int j = indx_file + 1; j < argc; j++) {
      print_flag_file(flags, argv[j], argv[indx_file], files);
    }
  }
  if (flags.f) {
    if (indx_file + 1 < argc) files = 1;
    for (int j = indx_file; j < argc; j++) {
      for (int i = 0; i < indx_file_pattern; i++) {
        print_flag_file(flags, argv[j], file_pattern[i], files);
        free(file_pattern[i]);
      }
    }
  }
}

void print_flag_file(Flags flags, char* fname, char* pattern, int files) {
  regex_t regex;
  int reti;
  int cflags = 0;
  regmatch_t matchptr[1] = {0};
  const size_t nmatch = 1;
  FILE* ptrF = fopen(fname, "rt");
  char* line_file = NULL;
  size_t len = 0;
  int read;
  if (ptrF != NULL) {
    int str_num = 1;
    int match_count = 0;
    while ((read = getline(&line_file, &len, ptrF)) != -1) {
      if (flags.i) cflags = REG_ICASE;
      reti = regcomp(&regex, pattern, cflags);
      reti = regexec(&regex, line_file, nmatch, matchptr, 0);
      if ((!reti && !flags.v) || (flags.v && reti == REG_NOMATCH)) {
        if (flags.l) {
          printf("%s\n", fname);
          break;
        }
        for (int i = 0; i < read; i++) {
          match_count++;
          if (flags.c) break;
          if (files && i == 0) {
            if (!flags.h) printf("%s:", fname);
          }
          if (flags.n && i == 0) printf("%d:", str_num);
          if (matchptr[0].rm_so == i && !flags.v) {
            while (i < matchptr[0].rm_eo) {
              printf("%c", line_file[i]);
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
      regfree(&regex);
      str_num++;
    }
    free(line_file);
    if (flags.c) {
      if (files)
        printf("%s:%d\n", fname, match_count);
      else
        printf("%d\n", match_count);
    }
    fclose(ptrF);
  } else if (!flags.s)
    fprintf(stderr, "grep: %s: No such file or directory\n", fname);
}

void read_flags(int argc, char** argv, Flags* flags, char* mass_pattern[100],
                int* indx_pattern, char* mass_files[100],
                int* indx_mass_files) {
  int current_flag = getopt_long(argc, argv, "e:ivclnhsof:", NULL, NULL);
  while (current_flag != -1) {
    switch (current_flag) {
      case 'e':
        flags->e = 1;
        mass_pattern[*indx_pattern] = optarg;
        *indx_pattern = *indx_pattern + 1;
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