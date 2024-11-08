#include "ms.h"

#define NUM_DIM 2
#define ROW 0
#define COL 1

// find all mines and unknowns in the board, 
// count them, and save coords of the unknowns
void findallmu(int* mcount, int* ucount, int ucoords[MAXSQ*MAXSQ][NUM_DIM], board* pb);

// count the mines around the given coord
int findneighbm(board* pb, int cj, int ci);

// count the unknowns around the given coord
int findneighbu(board* pb, int cj, int ci);

// transform the unknown with given coord to number by finding mines around
void unknown2num(board* pb, int uj, int ui);

// try to determine if the unknown with given coord is a mine
// by applying rule 2 on 3x3 grids where given coord is around the centre
void unknown2mine(board* pb, int uj, int ui);


board solve_board(board b)
{
  int mcount, lastucount, ucount, ucoords[MAXSQ*MAXSQ][NUM_DIM];

  // count all mines and unknowns in the grid
  // also save the locations of unknowns into ucoords as row and col coords
  findallmu( &mcount, &ucount, ucoords, &b);

  do
  {
    if(mcount > b.totmines)
    {
      fprintf(stderr, "More mines are found than totmine.\n");
    }

    lastucount = ucount;
    // go through the list of coords of unknowns
    for(int i=0; i<ucount; i++)
    {
      // rule 1, if all mines are found, 
      // fill numbers of neighbour mines into unknown squares
      if(mcount == b.totmines)
      {
        unknown2num(&b, ucoords[i][ROW], ucoords[i][COL]);
      }
      // if there are still mines to be found, try to apply 
      // rule 2 on 3x3 grids where unknowns are around the centre
      else
      {
        unknown2mine(&b, ucoords[i][ROW], ucoords[i][COL]);
      }
    }
    findallmu( &mcount, &ucount, ucoords, &b);
  }
  while( (lastucount!=ucount) && ucount!=0);
  return b;
}


void board2str(char s[MAXSQ*MAXSQ+1], board b)
{
  for(int j=0; j<b.h; j++)
  {
    for(int i=0; i<b.w; i++)
    {
      s[j*b.w+i]=b.grid[j][i];
    }
  }
}


bool syntax_check(unsigned totmines, unsigned width, unsigned height, char inp[MAXSQ*MAXSQ+1])
{
  // check number of characters in string
  if(strlen(inp) != width*height)
  {
    return false;
  }

  unsigned mcount=0;
  for(unsigned i=0; i<width*height; i++)
  {
    // check there are only characters from the set: 012345678?X
    if( (inp[i]<'0' || inp[i]>'8') && inp[i]!=UNK && inp[i]!=MINE)
    {
      return false;
    }

    // count mines in string, check against totmines
    if(inp[i]==MINE)
    {
      mcount++;
      if(mcount>totmines)
      {
        return false;
      }
    }
  }
  return true;
}


board make_board(int totmines, int width, int height, char inp[MAXSQ*MAXSQ+1])
{
  board b;
  b.w = width;
  b.h = height;
  b.totmines = totmines;
  for(int j=0; j<height; j++)
  {
    for(int i=0; i<width; i++)
    {
      b.grid[j][i] = inp[j*width+i];
    }
  }
  return b;
}


void findallmu(int* mcount, int* ucount, int ucoords[MAXSQ*MAXSQ][NUM_DIM], board* pb)
{
  *mcount = 0;
  *ucount = 0;
  for(int j=0; j<pb->h; j++)
  {
    for(int i=0; i<pb->w; i++)
    {
      if(pb->grid[j][i] == UNK)
      {
        ucoords[*ucount][ROW] = j;
        ucoords[*ucount][COL] = i;
        (*ucount)++;
      }
      if(pb->grid[j][i] == MINE)
      {
        (*mcount)++;
      }
    }
  }
}


int findneighbm(board* pb, int cj, int ci)
{
  int nbmcount = 0;
  for(int j=cj-1; j<=cj+1; j++)
  {
    for(int i=ci-1; i<=ci+1; i++)
    {
      // current coord is a valid neighbour?
      if( (j>=0 && j<pb->h) && (i>=0 && i<pb->w) && !(j==cj && i==ci) )
      {

        if(pb->grid[j][i] == MINE)
        {
          nbmcount++;
        }

      }
    }
  }
  return nbmcount;
}


int findneighbu(board* pb, int cj, int ci)
{
  int nbucount = 0;
  for(int j=cj-1; j<=cj+1; j++)
  {
    for(int i=ci-1; i<=ci+1; i++)
    {
      // current coord is a valid neighbour?
      if( (j>=0 && j<pb->h) && (i>=0 && i<pb->w) && !(j==cj && i==ci) )
      {

        if(pb->grid[j][i] == UNK)
        {
          nbucount++;
        }

      }
    }
  }
  return nbucount;
}


void unknown2num(board* pb, int uj, int ui)
{
  int nbmcount = findneighbm(pb, uj, ui);
  pb->grid[uj][ui] = '0'+nbmcount;
}


void unknown2mine(board* pb, int uj, int ui)
{
  int num, nbmcount, nbucount;
  for(int j=uj-1; j<=uj+1; j++)
  {
    for(int i=ui-1; i<=ui+1; i++)
    {
      // current coord is a valid neighbour?
      if( (j>=0 && j<pb->h) && (i>=0 && i<pb->w) && !(j==uj && i==ui) )
      {

        // make sure it is not a mine or unknown
        if( isdigit(pb->grid[j][i]) )
        {
          num = pb->grid[j][i] - '0';
          nbmcount = findneighbm(pb, j, i);
          nbucount = findneighbu(pb, j, i);
          if( num==nbmcount+nbucount )
          {
            // rule 2 passed, set current unknown to mine
            pb->grid[uj][ui] = MINE;
            return;
          }
        }

      }
    }
  }
}


void test(void)
{
  board b;
  char str[MAXSQ*MAXSQ+1];
  int mcount, ucount, ucoords[MAXSQ*MAXSQ][NUM_DIM]; 

  // Invalid string (too small or too large)
  strcpy(str, "11?010?");
  assert(syntax_check(1, 5, 5, str)==false);
  strcpy(str, "11?010?????????????????????????");
  assert(syntax_check(1, 5, 5, str)==false);

  // Invalid string (has an 'l' not a '1')
  strcpy(str, "11?0?X111l111X?11?11?X10?");
  assert(syntax_check(3, 5, 5, str)==false);
  // Invalid string (has an 'x' not an 'X')
  strcpy(str, "11?0?X1111111X?11?11?x10?");
  assert(syntax_check(3, 5, 5, str)==false);

  // Invalid string (has more mine than totmine)
  strcpy(str, "11?0?X11XXXX1X?11?11?X10?");
  assert(syntax_check(3, 5, 5, str)==false);

  // No unknowns
  strcpy(str, "000000111001X100111000000");
  assert(syntax_check(1, 5, 5, str)==true);
  b = make_board(1, 5, 5, str);
  board2str(str, b);
  assert(strcmp(str, "000000111001X100111000000")==0);

  // No unknowns
  strcpy(str, "XXXXXX535XX303XX535XXXXXX");
  assert(syntax_check(16, 5, 5, str)==true);
  b = make_board(16, 5, 5, str);
  board2str(str, b);
  assert(strcmp(str, "XXXXXX535XX303XX535XXXXXX")==0);

  // 4x5 with 4 mines, multiple unknowns, step by step
  strcpy(str, "?110?1?2101?X?1012?1");
  assert(syntax_check(4, 4, 5, str)==true);
  b = make_board(4, 4, 5, str);
  board2str(str, b);
  assert(strcmp(str, "?110?1?2101?X?1012?1")==0);

  findallmu( &mcount, &ucount, ucoords, &b);
  assert(mcount==1);
  assert(ucount==6);
  assert(ucoords[0][ROW]==0);
  assert(ucoords[0][COL]==0);
  assert(ucoords[5][ROW]==4);
  assert(ucoords[5][COL]==2);

  assert(findneighbm(&b, 0, 0)==0);
  assert(findneighbm(&b, 2, 1)==1);
  assert(findneighbu(&b, 0, 0)==1);
  assert(findneighbu(&b, 2, 2)==3);

  unknown2mine(&b, 0, 0);
  board2str(str, b);
  assert(strcmp(str, "?110?1?2101?X?1012?1")==0);
  unknown2mine(&b, 4, 2);
  board2str(str, b);
  assert(strcmp(str, "?110?1?2101?X?1012X1")==0);

  unknown2num(&b, 0, 0);
  board2str(str, b);
  assert(strcmp(str, "0110?1?2101?X?1012X1")==0);
  unknown2num(&b, 3, 1);
  board2str(str, b);
  assert(strcmp(str, "0110?1?2101?X21012X1")==0);

  // 1X1 mine
  strcpy(str, "X");
  assert(syntax_check(1, 1, 1, str)==true);
  b = make_board(1, 1, 1, str);
  b = solve_board(b);
  board2str(str, b);
  assert(strcmp(str, "X")==0);

  // 1X1 unknown
  strcpy(str, "?");
  assert(syntax_check(0, 1, 1, str)==true);
  b = make_board(0, 1, 1, str);
  b = solve_board(b);
  board2str(str, b);
  assert(strcmp(str, "0")==0);

  // 3X3 with 1 mine, all unknown
  strcpy(str, "?????????");
  assert(syntax_check(1, 3, 3, str)==true);
  b = make_board(1, 3, 3, str);
  b = solve_board(b);
  board2str(str, b);
  assert(strcmp(str, "?????????")==0);

  // Rule 1 : 3x3 with 8 mines, 1 unknown
  strcpy(str, "XXXX?XXXX");
  assert(syntax_check(8, 3, 3, str)==true);
  b = make_board(8, 3, 3, str);
  b = solve_board(b);
  board2str(str, b);
  assert(strcmp(str, "XXXX8XXXX")==0);

  // Rule 1 : 5x5 with 3 mines, multiple unknowns
  strcpy(str, "11?0?X1111111X?11?11?X10?");
  assert(syntax_check(3, 5, 5, str)==true);
  b = make_board(3, 5, 5, str);
  b = solve_board(b);
  board2str(str, b);
  assert(strcmp(str, "11000X1111111X1112111X100")==0);

  // Rules 1&2 : 5x5 with 3 mines, multiple unknowns
  strcpy(str, "?110?1?2101?X?1012?1?0111");
  assert(syntax_check(3, 5, 5, str)==true);
  b = make_board(3, 5, 5, str);
  b = solve_board(b);
  board2str(str, b);
  assert(strcmp(str, "111001X21012X21012X100111")==0);

  // Rules 1&2 : 4x5 with 4 mines, multiple unknowns
  strcpy(str, "?110?1?2101?X?1012?1");
  assert(syntax_check(4, 4, 5, str)==true);
  b = make_board(4, 4, 5, str);
  b = solve_board(b);
  board2str(str, b);
  assert(strcmp(str, "011001X2101XX21012X1")==0);
}
