#include "s21_cat.h"

int main(int argc, char** argv) {
  Flags flags = {0, 0, 0, 0, 0, 0};
  read_flags(argc, argv, &flags);
  for (int i = 1; i < argc; i++) {
    if (argv[i][0] != '-') {
      print_flag_file(flags, argv[i]);
    }
  }
  return 0;
}

void print_flag_file(Flags flags, char* fname) {
  FILE* ptrF = fopen(fname, "rt");
  int c, empty = 1, start_str = 1, line = 1;
  if (ptrF != NULL) {
    while ((c = fgetc(ptrF)) != EOF) {
      if (c == '\n')
        empty += 1;
      else
        empty = 0;
      if (flags.squeeze_blank) {
        if (empty > 2) continue;
      }
      if (flags.tab) {
        if (c == '\t') {
          printf("^");
          c += 64;
        }
      }
      if (flags.print_non_printable) {
        if ((c >= 0 && c <= 31 && c != '\n' && c != '\t') || c == 127) {
          printf("^");
          c += 64;
        }
      }
      if (flags.number_nonblanc) {
        if (start_str && c != '\n') {
          printf("%6d\t", line);
          line += 1;
        }
      }
      if (flags.number_string && !flags.number_nonblanc) {
        if (start_str) {
          printf("%6d\t", line);
          line += 1;
        }
      }
      if (flags.symbol_end) {
        if (c == '\n') printf("%c", '$');
      }
      if (c != '\n')
        start_str = 0;
      else
        start_str = 1;
      printf("%c", c);
    }
    fclose(ptrF);
  } else
    fprintf(stderr, "cat: %s: No such file or directory\n", fname);
}

void read_flags(int argc, char** argv, Flags* flags) {
  struct option long_opts[] = {{"number-nonblank", 0, 0, 'b'},
                               {"number", 0, 0, 'n'},
                               {"squeeze-blank", 0, 0, 's'},
                               {0, 0, 0, 0}};
  int current_flag = getopt_long(argc, argv, "bevEnstT", long_opts, NULL);
  while (current_flag != -1) {
    switch (current_flag) {
      case 'b':
        flags->number_nonblanc = 1;
        break;
      case 'e':
        flags->symbol_end = 1;
        flags->print_non_printable = 1;
        break;
      case 'v':
        flags->print_non_printable = 1;
        break;
      case 'E':
        flags->symbol_end = 1;
        break;
      case 'n':
        flags->number_string = 1;
        break;
      case 's':
        flags->squeeze_blank = 1;
        break;
      case 't':
        flags->tab = 1;
        flags->print_non_printable = 1;
        break;
      case 'T':
        flags->tab = 1;
        break;
    }
    current_flag = getopt_long(argc, argv, "bevEnstT", long_opts, NULL);
  }
}
