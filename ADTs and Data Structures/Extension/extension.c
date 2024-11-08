#include "../bsa.h"
#include "specific.h"

void test(void)
{
  bsa* b = bsa_init();
  assert(b->first == NULL);
  assert(b->last == NULL);

  assert(bsa_get_data(NULL, 1) == NULL);
  assert(bsa_get_data(b, -1) == NULL);
  assert(bsa_get_data(b, 1) == NULL);

  assert(bsa_set(NULL, 0, 1) == false);
  assert(bsa_set(b, -1, 1) == false);
  assert(bsa_set(b, 1, 1) == true);

  bsa_data* b_data = bsa_get_data(b, 1);
  assert(b_data->data == 1);

  assert(bsa_get(NULL, 1) == NULL);
  assert(bsa_get(b, -1) == NULL);
  assert(bsa_get(b, 2) == NULL);
  assert(*bsa_get(b, 1) == 1);

  assert(bsa_delete(NULL, 1) == false);
  assert(bsa_delete(b, -1) == false);
  assert(bsa_delete(b, 2) == false);
  assert(*bsa_get(b, 1) == 1);
  assert(bsa_delete(b, 1) == true);
  assert(bsa_get(b, 1) == NULL);

  assert(bsa_set(b, 1, 1) == true);
  assert(bsa_maxindex(b) == 1);
  assert(bsa_set(b, 5, 1) == true);
  assert(bsa_maxindex(b) == 5);
  assert(bsa_delete(b, 1) == true);
  assert(bsa_delete(b, 5) == true);
  assert(bsa_maxindex(b) == -1);

  assert(bsa_set(b, 4, 1) == true);
  assert(bsa_set(b, 5, 1) == true);
  int num1 = 1;
  int num2 = 2;
  test_addition(&num1, &num2);
  assert(num1 == 3);
  bsa_foreach(test_addition, b, &num1);
  assert(*bsa_get(b, 4) == 4);
  assert(*bsa_get(b, 5) == 4);

  assert(bsa_free(b) == true);
  b = NULL;
  assert(bsa_free(b) == false);
}


bsa* bsa_init(void)
{
  bsa* b = (bsa*) calloc(1, sizeof(bsa));
  exit_if_null((void*) b);
  return b;
}


bool bsa_set(bsa* b, int indx, int d)
{
  if(b == NULL || indx < 0)
  {
    return false;
  }

  bsa_data* b_data;
  if(b->first == NULL)
  {
    b->first = (bsa_data*) calloc(1, sizeof(bsa_data));
    exit_if_null((void*) b->first);
    b->last = b->first;
    b_data = b->first;
  }
  else
  {
    b_data = b->first;
    while(b_data != NULL && b_data->index < indx)
    {
      b_data = b_data->next;
    }
    if(b_data == NULL)
    {
      b->last->next = (bsa_data*) calloc(1, sizeof(bsa_data));
      exit_if_null((void*) b->last->next);
      b->last = b->last->next;
      b_data = b->last;
    }
    else if(b_data->index > indx)
    {
      bsa_data* b_tempdata= (bsa_data*) calloc(1, sizeof(bsa_data));
      exit_if_null((void*) b_tempdata);
      b_tempdata->prev = b_data->prev;
      b_tempdata->next = b_data;
      b_data = b_tempdata;
      b_data->prev->next = b_data;
      b_data->next->prev = b_data;
    }
  }

  b_data->index = indx;
  b_data->data = d;
  return true;
}


int* bsa_get(bsa* b, int indx)
{
  bsa_data* b_data = bsa_get_data(b, indx);
  if(b_data == NULL)
  {
    return NULL;
  }
  
  return &b_data->data;
}


bool bsa_delete(bsa* b, int indx)
{
  bsa_data* b_data = bsa_get_data(b, indx);
  if(b_data == NULL)
  {
    return false;
  }

  if(b_data->prev == NULL)
  {
    b->first = b_data->next;
  }
  else
  {
    b_data->prev->next = b_data->next;
  }
  if(b_data->next == NULL)
  {
    b->last = b_data->prev;
  }
  else
  {
    b_data->next->prev = b_data->prev;
  }

  free(b_data);

  return true;
}


int bsa_maxindex(bsa* b)
{
  if(b == NULL || b->last == NULL)
  {
    return -1;
  }

  return b->last->index;
}


bool bsa_free(bsa* b)
{
  if(b == NULL)
  {
    return false;
  }

  bsa_data* b_data = b->first;
  bsa_data* b_tempdata;
  while(b_data != NULL)
  {
    b_tempdata = b_data;
    b_data = b_data->next;
    free(b_tempdata);
  }

  free(b);
  return true;
}


void bsa_foreach(void (*func)(int* p, int* n), bsa* b, int* acc)
{
  if(b == NULL)
  {
    return;
  }

  bsa_data* b_data = b->first;
  while(b_data != NULL)
  {
    (*func)(&b_data->data, acc);
    b_data = b_data->next;
  }
}


void test_addition(int* a, int* b)
{
  *a = *a + *b;
}


void exit_if_null(void* ptr)
{
  if(ptr==NULL)
  {
    exit(EXIT_FAILURE);
  }
}


bsa_data* bsa_get_data(bsa* b, int indx)
{
  if(b == NULL || b->first == NULL ||indx < 0)
  {
    return NULL;
  }

  bsa_data* b_data = b->first;
  while(b_data->index != indx)
  {
    b_data = b_data->next;
    if(b_data == NULL)
    {
      return NULL;
    }
  }
  
  return b_data;
}
