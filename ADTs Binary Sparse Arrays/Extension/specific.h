#pragma once

typedef struct bsa_data bsa_data;

struct bsa
{
  bsa_data* first;
  bsa_data* last;
};

struct bsa_data
{
  int index;
  int data;
  bsa_data* prev;
  bsa_data* next;
};

void test_addition(int* a, int* b);
void exit_if_null(void* ptr);
bsa_data* bsa_get_data(bsa* b, int indx);
