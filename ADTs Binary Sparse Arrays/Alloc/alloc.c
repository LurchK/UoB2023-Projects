#include "../bsa.h"
#include "specific.h"

void test(void)
{
  assert(indx2row(0) == 0);
  assert(indx2row(1) == 1);
  assert(indx2row(2) == 1);
  assert(indx2row(3) == 2);
  assert(indx2row(4) == 2);
  assert(indx2row(5) == 2);

  bsa* b = bsa_init();
  assert(b->data_rows[0] == NULL);
  assert(b->in_use_rows[0] == NULL);

  assert(bsa_set(NULL, 0, 1) == false);
  assert(bsa_set(b, -1, 1) == false);
  assert(bsa_set(b, 1U<<(BSA_ROWS), 1) == false);
  assert(bsa_set(b, 1, 1) == true);

  assert(bsa_get(NULL, 1) == NULL);
  assert(bsa_get(b, -1) == NULL);
  assert(bsa_get(b, 1U<<(BSA_ROWS)) == NULL);
  assert(bsa_get(b, 2) == NULL);
  assert(*bsa_get(b, 1) == 1);

  assert(bsa_delete(NULL, 1) == false);
  assert(bsa_delete(b, -1) == false);
  assert(bsa_delete(b, 1U<<(BSA_ROWS)) == false);
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

  char str[1000] = {0};
  assert(bsa_tostring(b, str) == true);
  assert(strcmp(str, "") == 0);
  assert(bsa_set(b, 4, 1) == true);
  assert(bsa_set(b, 5, 1) == true);
  assert(bsa_tostring(b, str) == true);
  assert(strcmp(str, "{}{}{[4]=1 [5]=1}") == 0);

  int num1 = 1;
  int num2 = 2;
  test_addition(&num1, &num2);
  assert(num1 == 3);
  bsa_foreach(test_addition, b, &num1);
  assert(bsa_tostring(b, str) == true);
  assert(strcmp(str, "{}{}{[4]=4 [5]=4}") == 0);

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

  int row = indx2row(indx);
  if(row >= BSA_ROWS)
  {
    return false;
  }

  if(b->data_rows[row] == NULL)
  {
    int num_cols = 1U<<row;
    b->data_rows[row] = (int*) calloc(num_cols, sizeof(int));
    exit_if_null((void*) b->data_rows[row]);
    b->in_use_rows[row] = (bool*) calloc(num_cols, sizeof(bool));
    exit_if_null((void*) b->in_use_rows[row]);
  }

  int col = indx-((1U<<row)-1);
  b->data_rows[row][col] = d;
  b->in_use_rows[row][col] = true;

  return true;
}


int* bsa_get(bsa* b, int indx)
{
  if(b == NULL || indx < 0)
  {
    return NULL;
  }

  int row = indx2row(indx);
  if(row >= BSA_ROWS)
  {
    return NULL;
  }

  if(b->in_use_rows[row] == NULL)
  {
    return NULL;
  }

  int col = indx-((1U<<row)-1);
  if(b->in_use_rows[row][col] == false)
  {
    return NULL;
  }

  return &b->data_rows[row][col];
}


bool bsa_delete(bsa* b, int indx)
{
  if(b == NULL || indx < 0)
  {
    return false;
  }

  int row = indx2row(indx);
  if(row >= BSA_ROWS)
  {
    return NULL;
  }

  if(b->in_use_rows[row] == NULL)
  {
    return NULL;
  }

  int col = indx-((1U<<row)-1);
  if(b->in_use_rows[row][col] == false)
  {
    return NULL;
  }

  b->in_use_rows[row][col] = false;

  int num_cols = 1U<<row;
  for(int col=0; col<num_cols; col++)
  {
    if(b->in_use_rows[row][col] == true)
    {
      return true;
    }
  }

  free(b->data_rows[row]);
  b->data_rows[row] = NULL;
  free(b->in_use_rows[row]);
  b->in_use_rows[row] = NULL;

  return true;
}


int bsa_maxindex(bsa* b)
{
  if(b == NULL)
  {
    return -1;
  }

  for(int row=BSA_ROWS-1; row>=0; row--)
  {
    if(b->in_use_rows[row] != NULL)
    {
      int num_cols = 1U<<row;
      for(int col=num_cols-1; col>=0; col--)
      {
        if(b->in_use_rows[row][col] == true)
        {
          return ((1U<<row)-1)+col;
        }
      }
    }
  }
  return -1;
}


bool bsa_free(bsa* b)
{
  if(b == NULL)
  {
    return false;
  }

  for(int row=0; row<BSA_ROWS; row++)
  {
    if(b->in_use_rows[row] != NULL)
    {
      free(b->data_rows[row]);
      free(b->in_use_rows[row]);
    }
  }
  free(b);
  return true;
}


bool bsa_tostring(bsa* b, char* str)
{
  if(b == NULL || str == NULL)
  {
    return false;
  }
  str[0] = 0;

  int maxindx = bsa_maxindex(b);
  if(maxindx < 0)
  {
    return true;
  }
  int maxrow = indx2row(maxindx);

  int indx, num_cols;
  char datastr[ELEMSTRLEN] = {0};
  for(int row=0; row<=maxrow; row++)
  {
    strcat(str, "{");
    if(b->in_use_rows[row] != NULL)
    {

      num_cols = 1U<<row;
      for(int col=0; col<num_cols; col++)
      {
        if(b->in_use_rows[row][col] == true)
        {
          indx = ((1U<<row)-1)+col;
          snprintf(datastr, ELEMSTRLEN, "[%i]=%i ", indx, b->data_rows[row][col]);
          strcat(str, datastr);
        }
      }
      str[strlen(str)-1] = 0;

    }
    strcat(str, "}");
  }
  return true;
}


void bsa_foreach(void (*func)(int* p, int* n), bsa* b, int* acc)
{
  if(b == NULL)
  {
    return;
  }

  int num_cols;
  for(int row=0; row<BSA_ROWS; row++)
  {
    if(b->in_use_rows[row] != NULL)
    {
      num_cols = 1U<<row;
      for(int col=0; col<num_cols; col++)
      {
        if(b->in_use_rows[row][col] == true)
        {
          (*func)(&b->data_rows[row][col], acc);
        }
      }
    }
  }

}


int indx2row(int indx)
{
  int row = 0;
  unsigned num = indx+1;
  num = num>>1;
  while(num != 0)
  {
    num = num>>1;
    row++;
  }
  return row;
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
