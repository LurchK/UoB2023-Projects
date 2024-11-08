
## The N-Queens Puzzle

In the game of chess, the piece known as the Queen can take other pieces horizontally, vertically, or diagonally:

The eight queens puzzle is the problem of placing eight chess queens on an $8 \times 8$ chessboard so that no two queens threaten each other. Thus, a solution requires that no two queens share the same row, column, or diagonal. [Read more](https://en.wikipedia.org/wiki/Eight_queens_puzzle).

### Example Solution
One possible solution for an $8 \times 8$ board is:

```
Qa8, Qb4, Qc1, Qd3, Qe6, Qf2, Qg7, Qh5
```
Since there must be one, and only one, queen in each file (column), the above board can be summarized using the queen's position on each rank (height) - the board above would be numbered `84136275`.

Finding all solutions may take a long time, so it is normal to allow for board sizes other than $8 \times 8$ (but always square) by specifying the size of the board, $n$.

## Exercise

Write a program to find solutions to the $n$ queens problem using a brute-force algorithm to find all possible solutions.

You will use an array (list) of structures, each one containing the data for one board:

1. Put the initial (empty) board into the front of this list, `f=0`.
2. Consider the board at the **front** of the list (index `f`).
3. For this (parent) board, find the resulting (child) boards which can be created by placing one queen into a position that doesn't check any other queens. For each of these child boards:
   - If a child is unique (i.e., it has not been seen before in the list), add it to the end of the list.
   - If it has been seen before (a duplicate), ignore it.
   - If it is a solution board, keep a record of the number of them (and possibly) print out a summary of this board.
4. Add one to `f`. If there are more boards in the list, go to step 2.

We will allow for board sizes other than $8 \times 8$, but **never** greater than $10 \times 10$, as specified on the command line:

```
$ ./8q 6
4 solutions
```

If the `-verbose` flag is used, your program will also print out summaries of each solution:

```
$ ./8q -verbose 6
362514
246135
531642
415263
4 solutions
```

(Since $10 \times 10$ boards may be printed, we'll use the numbers `1 ... 9` and also `A`).

### Program Requirements

- **Must** use the algorithm detailed above (which is similar to a queue and therefore a breadth-first search). Do not use other algorithms (e.g., best-first, permutations, etc.); the quality of your coding is being assessed against others taking the same approach.
- **Must not** use dynamic arrays or linked lists. Since boards cannot be larger than $10 \times 10$, you can create boards of this size and only use a sub-part of them if the board required is smaller. The list of boards can be a fixed (large) size.
- **Should** check for invalid board sizes specified and report in a graceful way if there is a problem, aborting with `exit(EXIT_FAILURE)` if so.
- **Should not** print anything else to the screen after successfully completing the search, except what is shown above. Automated checking will be used during marking, and therefore the output must be precise.
- **Should** call the function `test()` to perform any assertion testing, etc.
- **Should** be submitted by uploading a single `8q.zip` file to Blackboard, which will include (at least) `8q.c` and `Makefile`. This allows compilation with `make 8q`, creating the `8q` executable, and running it with `make run`.

### Extension

Basic assignment = **90%**.  
Extension = **10%**.

If you'd like to try an extension, include `extension.c` and an explanation in `extension.txt` as part of your submission. Update your `Makefile` so that we can build your code using `make extension` and run it with `make extrun`.

The extension could involve a faster search technique, better graphical display, user input, or something else of your choosing.
