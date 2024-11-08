
## Minesweeper

The game [Minesweeper](https://en.wikipedia.org/wiki/Minesweeper_(video_game)) is a logic puzzle game played on a two-dimensional grid of squares. There are "mines" hidden in the grid, and other numbered squares tell you how many mines there are in that square's (eight-count) Moore neighbourhood. [Learn more](https://en.wikipedia.org/wiki/Moore_neighborhood).

You will know in advance the width and height of the grid, and also the total number of mines in the completed (solved) grid. In our version of the game, we'll be using just two rules to "solve" the grid by working out the unknown squares.

### Rule 1: Count the Mines
If we've discovered all the mines on the board already, then any unknown cell can simply be numbered with the count of mines in its Moore neighbourhood.

For example, if we know in advance that the total number of mines in the grid is five, and if these have all been found:

```
0 1 1 ? 0
1 3 X 3 1
1 X X X 1
1 3 X 3 1
0 1 1 1 0
```

The solution for the unknown square must be:

```
0 1 1 1 0
1 3 X 3 1
1 X X X 1
1 3 X 3 1
0 1 1 1 0
```

### Rule 2: Unknowns are Mines
For a known square having the number `n`, with `u` unknown and `m` known mines in its Moore neighbourhood, if `n = m + u` and `u > 0`, then all `u` unknown squares should be marked as mines.

Applying Rule 2 to the following grid:

```
0 1 1 1 0
1 3 X 3 1
1 X X X 1
1 3 ? 3 1
0 1 1 1 0
```

Will yield:

```
0 1 1 1 0
1 3 X 3 1
1 X X X 1
1 3 X 3 1
0 1 1 1 0
```

Repeated application of these rules will allow some (but not all) boards to be solved.

## Exercise

Code for this exercise can be found in [GitHub](https://github.com/csnwc/Exercises-In-C). Navigate into `Code/Week4/Minesweeper`.

Complete the file `ms.c`, which, along with my files `ms.h` and `drv.c`, allows the puzzles to be solved.

My file `ms.h` contains the function definitions which you'll have to implement in your `ms.c` file. The file `drv.c` contains the `main()` function to act as a driver to run the code. Your file will contain many other functions in addition to those specified, so you'll wish to test them as normal using the `test()` function.

If all of these files are in the same directory, you can compile them using the `Makefile` provided.

**Do not alter or resubmit** `ms.h` or `drv.c` – my original versions will be used to compile the `ms.c` file that you create. Only submit `ms.c`.
