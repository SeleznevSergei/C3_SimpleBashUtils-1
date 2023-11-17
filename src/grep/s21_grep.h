#ifndef SRC_GREP_S21_GREP_H_
#define SRC_GREP_S21_GREP_H_
#define _GNU_SOURCE

#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
  int e;
  int i;
  int v;
  int c;
  int l;
  int n;
  int h;
  int s;
  int o;
  int f;
} Flags;

void read_flags(int argc, char** argv, Flags* flags, char* mass_pattern[100],
                int* indx_pattern, char* mass_files[100], int* indx_mass_files);
void print_flag_file(Flags flags, char* fname, char* pattern, int files);
void call(int argc, char** argv, int files, int indx_file, Flags flags,
          int indx_pattern, char* mass_pattern[100], int indx_file_pattern,
          char* file_pattern[100]);

#endif