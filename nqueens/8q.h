#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

#define ARRSIZELIM 10
// theoretically approximating...
// 10000000 > 1 + 100 + 100*72 + 100*72*48 + 100*72*48*24 ...
// tested with size 10, found out the size should be 
// at least 7535369
#define LISTSIZE 10000000


typedef struct board
{
  int sum[ARRSIZELIM];
  int numq;
} 
board;


void test(void);

// check validity of commandline usage, set size and verbose accordingly.
bool is_valid_cmdline(int argc, char* argv[], int* size, bool* verbose);

// main function for solving the nqueens problem.
int solve_nqueens(int size, bool verbose);

// check if a new queen to be put on the parent board
// with the indexes jnew and inew is unthreatened,
// by using the summary of the parent board.
bool is_safe(board* parent, int size, int jnew, int inew);

// check if a board with a new queen to be put on the parent board
// with the indexes jnew and inew is a unique board by comparing the summaries
// between the new board and previous boards.
bool is_unique(board* boardlist, int fcnt, board* parent, int size, int jnew, int inew);

void make_child_board(board* child, board* parent, int size, int jnew, int inew);

void make_child_sum(int sum[ARRSIZELIM], board* parent, int size, int jnew, int inew);

bool is_same_sum(int sum1[ARRSIZELIM], int sum2[ARRSIZELIM], int size);

void print_sum(int sum[ARRSIZELIM], int size);
