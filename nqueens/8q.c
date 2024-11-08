#include "8q.h"


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
  argc=2; argv[1] = "11";
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

  size = 4;
  int sum1[ARRSIZELIM] = {0, 0, 0, 10};
  int sum2[ARRSIZELIM] = {0, 0, 0, 10, 10};
  int sum3[ARRSIZELIM] = {2, 0, 0, 10};
  assert(is_same_sum(sum1, sum2, size)==true);
  assert(is_same_sum(sum1, sum3, size)==false);

  board boardlist[5] = {{{0},0}};
  int f=0, fcnt=1, jq=1, iq=2;
  board* parent = &boardlist[f];
  make_child_sum(sum1, parent, size, jq, iq);
  int sum4[ARRSIZELIM] = {0, 0, 2, 0};
  assert(is_same_sum(sum1, sum2, size)==false);
  assert(is_same_sum(sum1, sum4, size)==true);

  fcnt=1; jq=1; iq=2; size = 4;
  boardlist[fcnt].sum[iq] = jq+1;
  boardlist[fcnt].numq = 1;
  fcnt=2; f=1;
  parent = &boardlist[f];
  assert(is_safe(parent, size, jq, 0)==false);
  assert(is_safe(parent, size, 0, iq)==false);
  assert(is_safe(parent, size, jq+1, iq+1)==false);
  assert(is_safe(parent, size, jq+1, iq-1)==false);
  assert(is_safe(parent, size, jq-1, iq+1)==false);
  assert(is_safe(parent, size, jq-1, iq-1)==false);
  assert(is_safe(parent, size, 0, 0)==true);

  fcnt=2; f=0; jq=1; iq=2; size = 4;
  parent = &boardlist[f];
  assert(is_unique(boardlist, fcnt, parent, size, jq, iq)==false);
  assert(is_unique(boardlist, fcnt, parent, size, 0, 0)==true);

  fcnt=2; f=1; jq=1; iq=2; size = 4;
  parent = &boardlist[f];
  board* child = &boardlist[fcnt];
  make_child_board(child, parent, size, jq, iq);
  fcnt++;
  assert(is_same_sum(child->sum, sum4, size)==true);
  assert(parent->numq+1 == child->numq);
  
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
  // using static to avoid depletion of stack space
  static board boardlist[LISTSIZE] = {{{0},0}};
  int f=0, fcnt=1, nsolutions=0;
  board* parent = &boardlist[f];
  board* child;
  while(f<fcnt)
  {
    for(int j=0; j<size; j++)
    {
      for(int i=0; i<size; i++)
      {

        if(is_safe(parent, size, j, i) && is_unique(boardlist, fcnt, parent, size, j, i))
        {
          child = &boardlist[fcnt];
          make_child_board(child, parent, size, j, i);
          if(child->numq == size)
          {
            nsolutions++;
            if(verbose)
            {
              print_sum(child->sum, size);
            }
          }
          fcnt++;
        }

      }
    }
    f++;
    parent = &boardlist[f];
  }
  return nsolutions;
}


bool is_safe(board* parent, int size, int jnew, int inew)
{
  int jold, iold;
  for(iold=0; iold<size; iold++)
  {
    // if there is a queen in this column.
    if(parent->sum[iold] != 0)
    {
      jold = parent->sum[iold]-1;
      if(iold == inew || jold == jnew)
      {
        return false;
      }
      if((iold-inew) == (jold-jnew) || (iold-inew) == (jnew-jold))
      {
        return false;
      }
    }
  }
  return true;
}


bool is_unique(board* boardlist, int fcnt, board* parent, int size, int jnew, int inew)
{
  int newsum[ARRSIZELIM];
  make_child_sum(newsum, parent, size, jnew, inew);

  for(int f=fcnt-1; f>0; f--)
  {
    if(boardlist[f].numq != parent->numq+1)
    {
      return true;
    }
    if(is_same_sum(boardlist[f].sum, newsum, size))
    {
      return false;
    }
  }
  return true;
}


void make_child_board(board* child, board* parent, int size, int jnew, int inew)
{
  make_child_sum(child->sum, parent, size, jnew, inew);

  child->numq = parent->numq+1;
}


void make_child_sum(int sum[ARRSIZELIM], board* parent, int size, int jnew, int inew)
{
  for(int k=0; k<size; k++)
  {
    sum[k] = parent->sum[k];
  }
  sum[inew] = jnew+1;
}


bool is_same_sum(int sum1[ARRSIZELIM], int sum2[ARRSIZELIM], int size)
{
  for(int k=0; k<size; k++)
  {
    if(sum1[k] != sum2[k])
    {
      return false;
    }
  }
  return true;
}


void print_sum(int sum[ARRSIZELIM], int size)
{
  for(int k=0; k<size; k++)
  {
    if(sum[k] < 10)
    {
      putchar('0'+sum[k]);
    }
    else
    {
      putchar('A'+sum[k]-10);
    }
  }
  putchar('\n');
}
