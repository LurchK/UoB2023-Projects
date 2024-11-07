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

- Even if you don’t get all the functions to work correctly, make sure the code still compiles by writing “dummy” functions as placeholders, even if some of the assertions fail.

**This is worth 90% of the marks.**

The manner in which you’ve been asked to implement the functionality above is **very** specific. However, exactly the same functionality (getting/setting, etc.) could be implemented in very different ways (linked lists, 1D dynamic arrays, trees, hashing, etc.). As an extension, write a "rival" version to implement the core functionality of these functions without it necessarily being a BSA. This should still ensure that `fibmemo`, `sieve`, and `isfactorial` can be compiled against it. This means you can ignore `bsa_tostring()` since this gives something very specific to the “shape” of the BSA data structure. Again, use an unchanged version of my Makefile to achieve this. These files will be `Extension/specific.h` and `Extension/extension.c`.

If you do this, submit `extension.txt` which details what you have done, the motivation for it, and how well it works in practice (or not), and comparing it with the original BSA. This discussion (a few hundred words at most) is as (if not more) important than the code itself.

**This is worth 10% of the marks.**
