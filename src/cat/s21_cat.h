#ifndef SRC_CAT_S21_CAT_H_
#define SRC_CAT_S21_CAT_H_

#include <getopt.h>
#include <stdio.h>

typedef struct {
  int number_nonblanc;
  int symbol_end;
  int number_string;
  int squeeze_blank;
  int tab;
  int print_non_printable;
} Flags;

void read_flags(int argc, char** argv, Flags* flags);
void print_flag_file(Flags flags, char* fname);

#endif