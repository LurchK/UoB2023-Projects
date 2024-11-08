#include "extension.h"


int main(int argc, char* argv[])
{
  test();

  int size;
  bool verbose;
  if(is_valid_cmdline(argc, argv, &size, &verbose) == false)
  {
    fprintf(stderr, "Usage: 8q [-verbose] board_size\n");
    fprintf(stderr, "For board_size being positive and no larger than %i.\n", ARRSIZELIM);
    exit(EXIT_FAILURE);
  }

  int nsolutions = solve_nqueens(size, verbose);
  printf("%i solutions\n", nsolutions);
  return 0;
}


void test(void)
{
  int size, argc;
  bool verbose;
  char* argv[4];
  argc=1; argv[1] = "0";
  assert(is_valid_cmdline(argc, argv, &size, &verbose)==false);
  argc=2; argv[1] = "-verbose";
  assert(is_valid_cmdline(argc, argv, &size, &verbose)==false);
  argc=2; argv[1] = "0";
  assert(is_valid_cmdline(argc, argv, &size, &verbose)==false);
  argc=2; argv[1] = "36";
  assert(is_valid_cmdline(argc, argv, &size, &verbose)==false);
  argc=2; argv[1] = "-1";
  assert(is_valid_cmdline(argc, argv, &size, &verbose)==false);
  argc=2; argv[1] = "1";
  assert(is_valid_cmdline(argc, argv, &size, &verbose)==true);
  argc=3; argv[1] = "1"; argv[2] = "-verbose";
  assert(is_valid_cmdline(argc, argv, &size, &verbose)==false);
  argc=3; argv[1] = "-verbose1"; argv[2] = "1";
  assert(is_valid_cmdline(argc, argv, &size, &verbose)==false);
  argc=3; argv[1] = "-verbose"; argv[2] = "1";
  assert(is_valid_cmdline(argc, argv, &size, &verbose)==true);
  argc=4; argv[1] = "-verbose"; argv[2] = "1";
  assert(is_valid_cmdline(argc, argv, &size, &verbose)==false);

  int sum[ARRSIZELIM] = {2, 4, 1, 3};
  int index = 1;
  assert(is_queen_safe(sum, index)==true);
  sum[index] -= 1;
  assert(is_queen_safe(sum, index)==false);

  size = 4; index = 1;
  digit_increment(sum, size, &index);
  assert(index == 1);
  assert(sum[index] == 4);
  digit_increment(sum, size, &index);
  assert(index == 0);
  assert(sum[index] == 3);
  
  assert(solve_nqueens(1, 0)==1);
  assert(solve_nqueens(2, 0)==0);
  assert(solve_nqueens(3, 0)==0);
  assert(solve_nqueens(4, 0)==2);
}


bool is_valid_cmdline(int argc, char* argv[], int* size, bool* verbose)
{
  if(argc < 2 || argc > 3)
  {
    return false;
  }
  for(int i=0; argv[argc-1][i]!=0; i++)
  {
    if(isdigit(argv[argc-1][i]) == false)
    {
      return false;
    }
  }
  if(sscanf(argv[argc-1], "%d", size) != 1)
  {
    return false;
  }
  if(*size<1 || *size>ARRSIZELIM)
  {
    return false;
  }

  if(argc == 2)
  {
    *verbose=false;
    return true;
  }
  else if(strcmp(argv[1], "-verbose") == 0)
  {
    *verbose=true;
    return true;
  }

  return false;
}


int solve_nqueens(int size, bool verbose)
{  
  int nsolutions=0, index=0, sum[ARRSIZELIM];
  for(int i=0; i<size; i++)
  {
    sum[i] = 1;
  }

  while(index >= 0)
  {
    if(is_queen_safe(sum, index))
    {
      if(index != size-1)
      {
        index++;
      }
      else
      {
        nsolutions++;
        if(verbose)
        {
          print_board(sum, size);
        }
        digit_increment(sum, size, &index);
      }
    }
    else
    {
      digit_increment(sum, size, &index);
    }
  }

  return nsolutions;
}


bool is_queen_safe(int sum[ARRSIZELIM], int index)
{
  int i1, i2, j1, j2;
  i1 = index;
  j1 = sum[index]-1;
  for(i2=0; i2<i1; i2++)
  {
    j2 = sum[i2]-1;

    // if same collumn or row
    if(i1 == i2 || j1 == j2)
    {
      return false;
    }
    // if in the diagonals
    if((i1-i2) == (j1-j2) || (i1-i2) == (j2-j1))
    {
      return false;
    }

  }

  return true;
}


void digit_increment(int sum[ARRSIZELIM], int size, int* index)
{
  sum[*index]++;
  while(sum[*index] > size)
  {
    sum[*index] = 1;
    (*index)--;
    if(*index < 0)
    {
      return;
    }
    else
    {
      sum[*index]++;
    }
  }
}


void print_board(int sum[ARRSIZELIM], int size)
{
  putchar('\n');
  for(int k=0; k<size; k++)
  {
    if(sum[k] < 10)
    {
      printf("%i ", sum[k]);
    }
    else
    {
      printf("%c ", 'A'+sum[k]-10);
    }
  }
  putchar('\n');
  for(int j=0; j<size; j++)
  {
    for(int i=0; i<size; i++)
    {
      if(sum[i] == j+1)
      {
        printf("%c ", QUEEN);
      }
      else
      {
        printf("%c ", EMPTY);
      }
    }
    putchar('\n');
  }
  putchar('\n');
}
