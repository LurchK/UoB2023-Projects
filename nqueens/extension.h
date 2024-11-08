#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

// set the limit to be 35 
// for it is possible to use 'Z' in summary printing
#define ARRSIZELIM 35
#define QUEEN 'Q'
#define EMPTY '.'


void test(void);

// check validity of commandline usage, set size and verbose accordingly.
bool is_valid_cmdline(int argc, char* argv[], int* size, bool* verbose);

// main function for solving the nqueens problem.
int solve_nqueens(int size, bool verbose);

// check if the queen referred by the index is 
// unthreatened by the queens in front of the index 
bool is_queen_safe(int sum[ARRSIZELIM], int index);

// increase the values in summary as if it is a <size>-based number,
// with sum[0] being the most significant digit,
// and with digit values from 1 to <size>.
// it returns the index as -1 if overflowed
void digit_increment(int sum[ARRSIZELIM], int size, int* index);

void print_board(int sum[ARRSIZELIM], int size);
