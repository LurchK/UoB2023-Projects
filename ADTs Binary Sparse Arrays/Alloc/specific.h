#pragma once

#define ELEMSTRLEN 200

struct bsa
{
  int* data_rows[BSA_ROWS];
  bool* in_use_rows[BSA_ROWS];
};

int indx2row(int num);
void test_addition(int* a, int* b);
void exit_if_null(void* ptr);
