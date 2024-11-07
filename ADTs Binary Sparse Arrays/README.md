## Binary Sparse Arrays

Using dynamic arrays is often a compromise between resizing the array too frequently (e.g. every time the memory required is fractionally too small) or else creating a great deal of "spare" memory, just in case it is needed later. It is common for such dynamic arrays to double the available memory every time an index is accessed out-of-bounds.

Here we create the Binary Sparse Array (BSA) which aims to be memory efficient for small arrays, releasing new memory at an exponential rate as the array grows. Using a BSA ensures that no data is ever copied (e.g. via `remalloc`) nor too much unused memory created early. The price we pay for this memory efficiency is a slightly more complex calculation for the address of the index required.

Another data structure, the Hashed Array Tree (HAT), has similar goals but is very different in practice and requires copying of data and resizing. [Read more](https://en.wikipedia.org/wiki/Hashed_array_tree).

The BSA structure consists of a (fixed-size) row pointer table, each cell of which enables access to a 1D row of data. These rows are of a known size, each of which is increasingly large as we go down the table, as shown in Figure 1.

![alt text](/ADTs%20Binary%20Sparse%20Arrays/Images/bsa1.png)

**Figure 1**: The Binary Sparse Array. A (vertical fixed-size) table allows access to each row. The rows are size `1` for the first, size `2` for the second, size `4` for the third, i.e., for row `k`, its size is `2^k`.

However, the space allocation for each row is only done as-and-when required. After creating an empty BSA, and inserting values into index `2` and `12`, then only rows one and three will have been allocated, as shown in Figure 2.

![alt text](/ADTs%20Binary%20Sparse%20Arrays/Images/bsa2.png)

**Figure 2**: A Binary Sparse Array, showing values inserted into index `2` and `12` (shaded cells). Other rows are currently unallocated.

If the only element in a row is deleted, then the entire row is freed up, as shown in Figure 3 where the last element in row three is removed (index `12`).

![alt text](/ADTs%20Binary%20Sparse%20Arrays/Images/bsa3.png)

**Figure 3**: Deletion in a BSA can result in entire rows being freed up.

## Exercise

Write an ADT to implement the `bsa.h` interface given. Write the source files `Alloc/specific.h` and `Alloc/alloc.c` such that the driver files `driver.c`, `isfactorial.c`, `sieve.c`, and `fibmemo.c` work correctly. Ensure that this all works with my `Makefile` which **must be** used **unaltered**. The basic operations are:

```c
bsa* bsa_init(void);
bool bsa_set(bsa* b, int indx, int d);
int* bsa_get(bsa* b, int indx);
bool bsa_delete(bsa* b, int indx);
int bsa_maxindex(bsa* b);
bool bsa_free(bsa* b);
```

The function `bsa_set()` allows an integer to be written to a particular index. If it is the first integer to be written into the row, then the space for this will need to be allocated.

`bsa_get()` returns a pointer to the integer stored in an index if it has been set, and `NULL` if it is a cell that is not in use, hasn't been written to, deleted, or past the end of the maximum index used.

The function `bsa_delete()` "removes" an integer from an index (maybe setting a Boolean flag to show if it's in use or not), and if it was the only cell being used on that row, the entire row is freed.

Additional functionality includes:

```c
bool bsa_tostring(bsa* b, char* str);
void bsa_foreach(void (*func)(int* p, int* n), bsa* b, int* acc);
```

`bsa_tostring()` allows a character-based version of the BSA to be produced with each row inside `{}` brackets. The function `bsa_foreach` allows a user-defined function to be passed which is performed on each element in turn.

### Notes

- The row pointer table is of a fixed size and never gets deleted until the final call to `bsa_free()`.
- You will never use `realloc` for the BSA – rows are created as required using, e.g., `calloc()` of a fixed size, and freed as required.
- Do not use the math library in this assignment (my Makefile doesn’t) – use bit manipulation if required rather than, e.g., `log2()`.
- Even if you don’t get all the functions to work correctly, make sure the code still compiles by writing "dummy" functions as placeholders, even if some of the assertions fail.

**This is worth 90% of the marks.**

The manner in which you’ve been asked to implement the functionality above is **very** specific. However, exactly the same functionality (getting/setting, etc.) could be implemented in very different ways (linked lists, 1D dynamic arrays, trees, hashing, etc.). As an extension, write a "rival" version to implement the core functionality of these functions without it necessarily being a BSA. This should still ensure that `fibmemo`, `sieve`, and `isfactorial` can be compiled against it.

If you do this, submit `extension.txt` which details what you have done, the motivation for it, and how well it works in practice (or not), and comparing it with the original BSA. This discussion (a few hundred words at most) is as (if not more) important than the code itself.

**This is worth 10% of the marks.**
