/*
 * ShanghaiTech University
 *
 * CS 110 Computer Architecture I - 2021 Spring
 *
 * HW 2
 */

#ifndef _DOUBLL2D_H
#define _DOUBLL2D_H

/* Header file for a 2d doubly linked list. */

#include <stdbool.h>
#include <stddef.h>

/* A list element (doubll2d_elem) has pointers to the four elements around it
   (i.e. left, right, up and down). The pointer should point to NULL if the
   neighbor it points to doesn't exist.

   It also has a pointer to the data (owned by the list element) and the size of
   that data. */

typedef struct doubll2d_elem doubll2d_elem;

struct doubll2d_elem {
    doubll2d_elem *left;
    doubll2d_elem *right;
    doubll2d_elem *up;
    doubll2d_elem *down;

    void *data;
    size_t size;
};

typedef struct doubll2d doubll2d;

/*
   You can imagine that our 2D linked list is a matrix, which MUST have
   a shape of rectangular at all time.

   The 2D doubly linked list with 4 elements (2 by 2) is like this:

    head -----------+   NULL              NULL
                    |    ^                 ^
                    |    |                 |
                    v    |                 |
                  +---------+         +---------+
    NULL <--------|    1    |<------->|    2    |------> NULL
                  +---------+         +---------+
                       ^                   ^
                       |                   |
                       v                   v
                  +---------+         +---------+
    NULL <--------|    3    |<------->|    4    |------> NULL
                  +---------+         +---------+
                       |                 |    ^
                       |                 |    |
                       v                 v    +--------- tail
                      NULL              NULL

    An empty list looks like this:

    head ------> NULL
    NULL <------ tail

    (If not empty) The head pointer of a list should always point to the
    upper-left most element, and the tail pointer should always point to the
    lower-right most element.

    The dim_row and dim_col stores the dimension of the rows and columns (i.e.
    the number of rows and columns).
*/
struct doubll2d {
    doubll2d_elem *head;
    doubll2d_elem *tail;

    size_t dim_row;
    size_t dim_col;
};

/* Initialize a 2D doubly linked list. */
void doubll2d_init(doubll2d *list);

/* Return the upper-left most element of the list.
   Return NULL on error. */
doubll2d_elem *doubll2d_get_head(doubll2d *list);

/* Return the lower-right most element of the list.
   Return NULL on error. */
doubll2d_elem *doubll2d_get_tail(doubll2d *list);

/* Return number of rows in a list.
   Return -1 on error. */
size_t doubll2d_dim_row(doubll2d *list);

/* Return number of columns in a list.
   Return -1 on error. */
size_t doubll2d_dim_col(doubll2d *list);

/* Insert a new row in the list below the row where the given `cursor` locates.
   The given `data` should be **copied** to the newly created elements.
   The `length` is the number of data pointers given to you.
   Return the pointer to the newly created element below the given `cursor`.
   Return NULL on error.

   Remember, our 2D doubly linked list should always be a rectangular. So if
   the `length` is shorter than it should be, abort immediately and return NULL.
   However, it is okay that the `length` is longer than needed. And in this case
   you should make use of the first `doubll.dim_col` items in `data` and leave
   alone the rest.

   If the marco CHECK_LIST is set, your code has to check if the element `cursor` is
   actually a member of the list. If not, abort and return NULL. This check is slow.
   If the marco CHECK_LIST is not set, your code shall not perform that check
   but rather run very fast. */
doubll2d_elem *doubll2d_insert_row(doubll2d *list, doubll2d_elem *cursor,
                                   void **data, size_t *size, size_t length);

/* Insert a new column in the list after the column where the given `cursor`
   locates. The given `data` should be **copied** to the newly created elements.
   The `length` is the number of data pointers given to you.
   Return the pointer to the newly created element after the given `cursor`.
   Return NULL on error.

   Remember, our 2D doubly linked list should always be a rectangular. So if
   the `length` is shorter than it should be, abort immediately and return NULL.
   However, it is okay that the `length` is longer than needed. And in this case
   you should make use of the first `doubll.dim_row` items in `data` and leave
   alone the rest.

   If the marco CHECK_LIST is set, your code has to check if the element `cursor` is
   actually a member of the list. If not, abort and return NULL. This check is slow.
   If the marco CHECK_LIST is not set, your code shall not perform that check
   but rather run very fast. */
doubll2d_elem *doubll2d_insert_col(doubll2d *list, doubll2d_elem *cursor,
                                   void **data, size_t *size, size_t length);

/* Delete the row where the given `cursor` locates and returns the element above
   the given `cursor`. If the first row is deleted, then return the element
   below the given `cursor`. If the deleted row is the only row in the list,
   return NULL. Return NULL on error.

   If the marco CHECK_LIST is set, your code has to check if the element `cursor` is
   actually a member of the list. If not, abort and return NULL. This check is slow.
   If the marco CHECK_LIST is not set, your code shall not perform that check
   but rather run very fast. */
doubll2d_elem *doubll2d_delete_row(doubll2d *list, doubll2d_elem *cursor);

/* Delete the column where the given `cursor` locates and returns the element
   before the given `cursor`. If the first column is deleted, then return the
   element after the given `cursor`. If the deleted column is the only column in
   the list, return NULL. Return NULL on error.

   If the marco CHECK_LIST is set, your code has to check if the element `cursor` is
   actually a member of the list. If not, abort and return NULL. This check is slow.
   If the marco CHECK_LIST is not set, your code shall not perform that check
   but rather run very fast. */
doubll2d_elem *doubll2d_delete_col(doubll2d *list, doubll2d_elem *cursor);

/*  Purge all items from the list. */
void doubll2d_purge(doubll2d *list);

/* Compare function.
   You could implement your own compare function in your test file. */
typedef bool list_less_func(const void *a, const void *b);

/* Use the given `less` function to find the maximum element in the whole list.
   If there are multiple maximum elements, return any one of them.
   When the 2D linked list is empty, return NULL.
 */
doubll2d_elem *doubll2d_find_max(doubll2d *list, list_less_func *less);

/* Use the given `less` function to find the minimum element in the whole list.
   If there are multiple minimum elements, return any one of them.
   When the 2D linked list is empty, return NULL.
 */
doubll2d_elem *doubll2d_find_min(doubll2d *list, list_less_func *less);

#endif /* doubll2d.h */