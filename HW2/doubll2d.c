#include "doubll2d.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


int find_cursor(doubll2d *list, doubll2d_elem *cursor) {
    /*temp used to travel whole 2d-list*/
    doubll2d_elem * temp = list->head;
    /* 2 num used to check if temp should go down
     * In other word, if all elements in current row has been visited*/
    unsigned int row_num, col_num;
    /*exist_cursor is a flag to check if cursor has been found*/
    int exist_cursor;

    for (row_num = 1; row_num <= list->dim_row; ++row_num) {
        /* temp go right when in odd row,
         * gor left when in even row*/
        int go_right;
        if (row_num % 2 == 1) {
            go_right = 1;
        } else {
            go_right = 0;
        }

        for (col_num = 1; col_num <= list->dim_col; ++col_num) {
            /*Find Cursor!*/
            if (temp == cursor) {
                exist_cursor = 1;
                break;
            }

            /* col_num isn't index but num of visited node in current row_num
             * so use it to check if temp should go down*/
            if (col_num == list->dim_col) {
                temp = temp->down;
            } else if (go_right == 1) {
                temp = temp->right;
            } else {
                temp = temp->left;
            }
        }

        /*Cursor has been found*/
        if (exist_cursor == 1) {
            break;
        }
    }

    if (exist_cursor == 1) {
        return true;
    } else {
        return false;
    }
}

void doubll2d_init(doubll2d *list) {
    /*Init head and tail, let them point to NULL*/
    list->head = NULL;
    list->tail = NULL;

    /*Init size of 2d-List */
    list->dim_row = 0;
    list->dim_col = 0;
}

doubll2d_elem *doubll2d_get_head(doubll2d *list) {
    /*Check null input*/
    if (list == NULL) {
        return NULL;
    } else {
        return list->head;
    }
}

doubll2d_elem *doubll2d_get_tail(doubll2d *list) {
    /*Check null input*/
    if (list == NULL) {
        return NULL;
    } else {
        return list->tail;
    }
}

size_t doubll2d_dim_row(doubll2d *list) {
    /*Check null input*/
    if (list == NULL) {
        return -1;
    } else {
        return list->dim_row;
    }
}

size_t doubll2d_dim_col(doubll2d *list) {
    /*Check null input*/
    if (list == NULL) {
        return -1;
    } else {
        return list->dim_col;
    }
}

doubll2d_elem *doubll2d_insert_row(doubll2d *list, doubll2d_elem *cursor,
                                   void **data, size_t *size, size_t length) {
    /*Check if the marco CHECK_LIST is set*/
    #ifdef CHECK_LIST
        /*Check whether list is empty, if true than just skip check*/
        if (list->dim_col != 0 && list->dim_row != 0 && find_cursor(list, cursor) == 0) {
            abort();
            return NULL;
        }
    #endif
    /*printf("row: %lu, col: %lu, cursor_size: %lu, data: %p, size: %p, length: %lu", list->dim_row, list->dim_col, cursor->size, (void *)data, (void *)size, length);*/
    if (list->dim_col == 0 && list->dim_row == 0) {
        /* Init the first Node
         * Malloc memory for it
         * Set four pointers to null
         * Set its data to first node of input data*/
        doubll2d_elem *temp = (doubll2d_elem *)malloc(sizeof(doubll2d_elem));
        temp->up = NULL;
        temp->down = NULL;
        temp->left = NULL;
        temp->right = NULL;
        /*Deep copy data*/
        temp->data = malloc(*size);
        memcpy(temp->data, data[0], *size);
        temp->size = *size;
        /*Set head and tail*/
        list->head = temp;
        list->tail = temp;
        /*Update row and col num*/
        list->dim_row ++;
        list->dim_col ++;
        return temp;
    }

    /*Check if length is shorter than it should be (list->dim_col)*/
    if (length < list->dim_col) {
        abort();
        return NULL;
    } else {
        /*Used in Third*/
        doubll2d_elem *current_node = cursor;
        /*Used in For statement*/
        unsigned int i;

        /*First malloc memory for new row and Check if succeed*/
        doubll2d_elem *new_row = (doubll2d_elem *)malloc(list->dim_col * sizeof(doubll2d_elem));
        if (new_row == NULL) {
            abort();
            return NULL;
        }

        /*Second copy data into new row*/
        for (i = 0; i < list->dim_col; ++i) {
            /*Deep copy data*/
            new_row[i].data = malloc(*size);
            memcpy(new_row[i].data, data[i], *size);

            new_row[i].size = *size;
            /*Address new nodes' horizontal pointers*/
            if (list->dim_col == 1) {
                /*Special situation for only one node*/
                new_row[i].left = NULL;
                new_row[i].right = NULL;
            } else {
                if (i == 0) {
                    /*Special situation for head node*/
                    new_row[i].left = NULL;
                    new_row[i].right = &new_row[i + 1];
                } else if (i == list->dim_col - 1) {
                    /*Special situation for tail node*/
                    new_row[i].right = NULL;
                    new_row[i].left = &new_row[i - 1];
                } else {
                    new_row[i].left = &new_row[i - 1];
                    new_row[i].right = &new_row[i + 1];
                }
            }
        }

        /*Third, move current_node to the head of this row*/
        while (current_node->left != NULL) {
            current_node = current_node->left;
        }

        /*Fourth, from head insert new row*/
        for (i = 0; i < list->dim_col; ++i) {
            /* Actually, our work is to address every new node's vertical poniters
             * and down pointers of nodes in cursor's row
             * and up pointers of nodes in original next row of cursor's row*/
            new_row[i].up = current_node;
            new_row[i].down = current_node->down;
            current_node->down = &new_row[i];
            /*Check if new_row become the bottom row*/
            if (new_row[i].down != NULL) {
                new_row[i].down->up = &new_row[i];
            }
            /*Move to next(right) node*/
            current_node = current_node->right;
        }

        /*If new_row is bottom row, assign tail to last node in new_row*/
        if (new_row[0].down == NULL) {
            list->tail = &new_row[list->dim_col - 1];
        }

    }

    list->dim_row ++;
    return cursor->down;
}
