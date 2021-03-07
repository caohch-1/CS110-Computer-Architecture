#include "doubll2d.h"
#include <stdlib.h>
#include <string.h>

/*Used to check if cursor is in list when CHECK_LIST is set*/
static int find_cursor(doubll2d *list, doubll2d_elem *cursor, int if_use) {
    doubll2d_elem *temp = list->head;
    /*col_num used to check if temp should go down, row_num used to check if temp should go right or left*/
    unsigned int row_num, col_num;
    int exist_cursor = 0;

    if (if_use == 0) return 0;

    for (row_num = 1; row_num <= list->dim_row; ++row_num) {
        /* temp go right when in odd row, go left when in even row*/
        int go_right;
        if (row_num % 2 == 1) go_right = 1;
        else go_right = 0;

        for (col_num = 1; col_num <= list->dim_col; ++col_num) {
            /*Find Cursor!*/
            if (temp == cursor) {
                exist_cursor = 1;
                break;
            }

            /*Move temp*/
            if (col_num == list->dim_col) temp = temp->down;
            else if (go_right == 1) temp = temp->right;
            else temp = temp->left;
        }

        /*Cursor has been found*/
        if (exist_cursor == 1) break;
    }

    if (exist_cursor == 1) return true;
    else return false;
}

void doubll2d_init(doubll2d *list) {
    /*Init head and tail, let them point to NULL*/
    list->head = NULL, list->tail = NULL;

    /*Init size of 2d-List */
    list->dim_row = 0, list->dim_col = 0;
}

doubll2d_elem *doubll2d_get_head(doubll2d *list) {
    /*Check null input*/
    if (list == NULL) return NULL;
    else return list->head;
}

doubll2d_elem *doubll2d_get_tail(doubll2d *list) {
    /*Check null input*/
    if (list == NULL) return NULL;
    else return list->tail;
}

size_t doubll2d_dim_row(doubll2d *list) {
    /*Check null input*/
    if (list == NULL) return -1;
    else return list->dim_row;
}

size_t doubll2d_dim_col(doubll2d *list) {
    /*Check null input*/
    if (list == NULL) return -1;
    else return list->dim_col;
}

doubll2d_elem *doubll2d_insert_row(doubll2d *list, doubll2d_elem *cursor,
                                   void **data, size_t *size, size_t length) {
    /*Check whether list is empty, if true than just skip check*/
#ifdef CHECK_LIST
    if (list->dim_col != 0 && list->dim_row != 0 && find_cursor(list, cursor, 1) == 0) { abort(); return NULL;}
#endif
    /*Don't mind it, just when CHECK_LIST isn't set the compiler will error since find_cursor() is unused*/
    find_cursor(list, cursor, 0);

    /*Special situation for insert into a empty list*/
    if (list->dim_col == 0 && list->dim_row == 0) {
        /*Create memory for new node*/
        doubll2d_elem *temp = (doubll2d_elem *) malloc(sizeof(doubll2d_elem));
        if (temp == NULL) {abort();return NULL;}
        /*Set four pointers*/
        temp->up = NULL, temp->down = NULL, temp->left = NULL, temp->right = NULL;
        /*Create memory for new node's data*/
        temp->data = malloc(*size);
        if (temp->data == NULL) {abort();return NULL;}
        /*Deep copy*/
        memcpy(temp->data, data[0], *size);
        temp->size = *size;

        /*Set other list properties*/
        list->head = temp, list->tail = temp;
        list->dim_row++, list->dim_col++;

        return temp;
    }

    /*Check if length is shorter than it should be (list->dim_col)*/
    if (length < list->dim_col) {abort();return NULL;}
    else {
        /*Variables preparation*/
        doubll2d_elem *current_node = cursor;
        unsigned int i;

        /*First Create memory for new row*/
        doubll2d_elem **new_row = (doubll2d_elem **) malloc(list->dim_col * sizeof(doubll2d_elem *));
        if (new_row == NULL) {abort();return NULL;}

        /*Second Create memory for every new node*/
        for (i = 0; i < list->dim_col; ++i) {
            new_row[i] = (doubll2d_elem *) malloc(sizeof(doubll2d_elem));
            if (new_row[i] == NULL) {abort();return NULL;}
        }

        /*Third Set new nodes' properties*/
        for (i = 0; i < list->dim_col; ++i) {
            /*Create memory for every new node's data*/
            new_row[i]->data = malloc(*size);
            if (new_row[i]->data == NULL) {abort();return NULL;}

            /*Deep copy data*/
            memcpy(new_row[i]->data, data[i], *size);

            new_row[i]->size = *size;

            /*Address new nodes' horizontal pointers*/
            if (list->dim_col == 1) {
                /*Special situation for only one node*/
                new_row[i]->left = NULL, new_row[i]->right = NULL;
            } else {
                /*Special situation for head node in new_row*/
                if (i == 0) new_row[i]->left = NULL, new_row[i]->right = new_row[i + 1];
                    /*Special situation for tail node in new_row*/
                else if (i == list->dim_col - 1) new_row[i]->right = NULL, new_row[i]->left = new_row[i - 1];
                else new_row[i]->left = new_row[i - 1], new_row[i]->right = new_row[i + 1];
            }
        }

        /*Fourth Move current_node to the head of this row and begin insertion*/
        while (current_node->left != NULL) current_node = current_node->left;

        for (i = 0; i < list->dim_col; ++i) {
            /* Set every new node's vertical pointers, down pointers in cursor's row and up pointers in original next row*/
            new_row[i]->up = current_node, new_row[i]->down = current_node->down, current_node->down = new_row[i];
            /*Check if new_row become the bottom row*/
            if (new_row[i]->down != NULL) new_row[i]->down->up = new_row[i];
            /*Move current_node to next(right) node*/
            current_node = current_node->right;
        }

        /*If new_row is bottom row, assign tail to last node in new_row*/
        if (new_row[0]->down == NULL) list->tail = new_row[list->dim_col - 1];
        free(new_row);
    }
    /*Set dim_row*/
    list->dim_row++;
    return cursor->down;
}

doubll2d_elem *doubll2d_insert_col(doubll2d *list, doubll2d_elem *cursor,
                                   void **data, size_t *size, size_t length) {
    /*Check whether list is empty, if true than just skip check*/
#ifdef CHECK_LIST
    if (list->dim_col != 0 && list->dim_row != 0 && find_cursor(list, cursor, 1) == 0) {abort();return NULL;}
#endif
    /*Don't mind it, just when CHECK_LIST isn't set the compiler will error since find_cursor() is unused*/
    find_cursor(list, cursor, 0);

    /*Special situation for insert into a empty list*/
    if (list->dim_col == 0 && list->dim_row == 0) {
        doubll2d_elem *temp = (doubll2d_elem *) malloc(sizeof(doubll2d_elem));
        if (temp == NULL) {abort();return NULL;}
        /*Set four pointers*/
        temp->up = NULL, temp->down = NULL, temp->left = NULL, temp->right = NULL;
        /*Create memory for new node's data*/
        temp->data = malloc(*size);
        if (temp->data == NULL) {abort();return NULL;}
        /*Deep copy*/
        memcpy(temp->data, data[0], *size);
        temp->size = *size;

        /*Set other list properties*/
        list->head = temp, list->tail = temp;
        list->dim_row++, list->dim_col++;

        return temp;
    }

    /*Check if length is shorter than it should be (list->dim_row)*/
    if (length < list->dim_row) {abort();return NULL;}
    else {
        /*Variables preparation*/
        doubll2d_elem *current_node = cursor;
        unsigned int i;

        /*First Create memory for new row*/
        doubll2d_elem **new_col = (doubll2d_elem **) malloc(list->dim_row * sizeof(doubll2d_elem *));
        if (new_col == NULL) {abort();return NULL;}

        /*Second Create memory for every new node*/
        for (i = 0; i < list->dim_row; ++i) {
            new_col[i] = (doubll2d_elem *) malloc(sizeof(doubll2d_elem));
            if (new_col[i] == NULL) {abort();return NULL;}
        }

        /*Third Set new nodes' properties*/
        for (i = 0; i < list->dim_row; ++i) {
            /*Create memory for every new node's data*/
            new_col[i]->data = malloc(*size);
            if (new_col[i]->data == NULL) {abort();return NULL;}

            /*Deep copy data*/
            memcpy(new_col[i]->data, data[i], *size);

            new_col[i]->size = *size;

            /*Address new nodes' horizontal pointers*/
            if (list->dim_row == 1) {
                /*Special situation for only one node*/
                new_col[i]->up = NULL, new_col[i]->down = NULL;
            } else {
                /*Special situation for head node in new_col*/
                if (i == 0) new_col[i]->up = NULL, new_col[i]->down = new_col[i + 1];
                    /*Special situation for tail node in new_col*/
                else if (i == list->dim_row - 1) new_col[i]->down = NULL, new_col[i]->up = new_col[i - 1];
                else new_col[i]->up = new_col[i - 1], new_col[i]->down = new_col[i + 1];
            }
        }

        /*Fourth Move current_node to the head of this row and begin insertion*/
        while (current_node->up != NULL) current_node = current_node->up;

        for (i = 0; i < list->dim_row; ++i) {
            /* Set every new node's vertical pointers, down pointers in cursor's row and up pointers in original next row*/
            new_col[i]->left = current_node, new_col[i]->right = current_node->right, current_node->right = new_col[i];
            /*Check if new_col become the rightest row*/
            if (new_col[i]->right != NULL) new_col[i]->right->left = new_col[i];
            /*Move current_node to next(down) node*/
            current_node = current_node->down;
        }

        /*If new_col is rightest row, assign tail to last node in new_col*/
        if (new_col[0]->right == NULL) list->tail = new_col[list->dim_row - 1];
        free(new_col);
    }

    /*Set dim_col*/
    list->dim_col++;
    return cursor->right;
}

doubll2d_elem *doubll2d_delete_row(doubll2d *list, doubll2d_elem *cursor) {
    /*Variables preparation*/
    doubll2d_elem *current_node = cursor;
    unsigned int i;
    doubll2d_elem *return_node;

    /*Check whether list is empty, if true than just skip check*/
#ifdef CHECK_LIST
    if (find_cursor(list, cursor, 1) == 0) {abort();return NULL;}
#endif
    /*Don't mind it, just when CHECK_LIST isn't set the compiler will error since find_cursor() is unused*/
    find_cursor(list, cursor, 0);

    /*Move current_node to head of cursor's row*/
    while (current_node->left != NULL) current_node = current_node->left;

    for (i = 0; i < list->dim_col; ++i) {
        /* Set down pointers in prev row and up pointers in next row*/
        if (current_node->up == NULL && current_node->down != NULL) {
            /*Special situation for delete first row:1.Move head; 2. Set return value*/
            if (i == 0) list->head = current_node->down;
            if (current_node == cursor) return_node = current_node->down;

            current_node->down->up = NULL;
        } else if (current_node->down == NULL && current_node->up != NULL) {
            /*Special situation for delete bottom row: 1.Move tail; 2. Set return value*/
            if (i == list->dim_col - 1) list->tail = current_node->up;
            if (current_node == cursor) return_node = current_node->up;

            current_node->up->down = NULL;
        } else if (current_node->down == NULL && current_node->up == NULL) {
            /*Special situation for only one row in 2d list*/
            return_node = NULL, list->head = NULL, list->tail = NULL;
        } else {
            /*Set return value*/
            if (current_node == cursor) return_node = current_node->up;

            current_node->up->down = current_node->down;
            current_node->down->up = current_node->up;
        }
        /*Special situation for free last node in this row*/
        if (i == list->dim_col - 1) {
            free(current_node->data), free(current_node);
            break;
        }
        /*Move current_node to next(right)*/
        current_node = current_node->right;

        free(current_node->left->data), free(current_node->left);
    }
    /*Set dim_row*/
    list->dim_row--;
    return return_node;
}

doubll2d_elem *doubll2d_delete_col(doubll2d *list, doubll2d_elem *cursor) {
    /*Variables preparation*/
    doubll2d_elem *current_node = cursor;
    unsigned int i;
    doubll2d_elem *return_node;

    /*Check whether list is empty, if true than just skip check*/
#ifdef CHECK_LIST
    if (find_cursor(list, cursor, 1) == 0) {abort();return NULL;}
#endif
    /*Don't mind it, just when CHECK_LIST isn't set the compiler will error since find_cursor() is unused*/
    find_cursor(list, cursor, 0);

    /*Move current_node to head of cursor's row*/
    while (current_node->up != NULL) current_node = current_node->up;

    for (i = 0; i < list->dim_row; ++i) {
        /* Set down pointers in prev row and up pointers in next row*/
        if (current_node->left == NULL && current_node->right != NULL) {
            /*Special situation for delete first row:1.Move head; 2. Set return value*/
            if (i == 0) list->head = current_node->right;
            if (current_node == cursor) return_node = current_node->right;

            current_node->right->left = NULL;
        } else if (current_node->right == NULL && current_node->left != NULL) {
            /*Special situation for delete bottom row: 1.Move tail; 2. Set return value*/
            if (i == list->dim_row - 1) list->tail = current_node->left;
            if (current_node == cursor) return_node = current_node->left;

            current_node->left->right = NULL;
        } else if (current_node->right == NULL && current_node->left == NULL) {
            /*Special situation for only one col in 2d list*/
            return_node = NULL, list->head = NULL, list->tail = NULL;
        } else {
            /*Set return value*/
            if (current_node == cursor) return_node = current_node->left;

            current_node->left->right = current_node->right;
            current_node->right->left = current_node->left;
        }
        /*Special situation for last node in this col*/
        if (i == list->dim_row - 1) {
            free(current_node->data), free(current_node);

            break;
        }
        /*Move current_node to next(down)*/
        current_node = current_node->down;

        free(current_node->up->data), free(current_node->up);
    }
    /*Set dim_col*/
    list->dim_col--;
    return return_node;
}

void doubll2d_purge(doubll2d *list) {
    doubll2d_elem *temp = list->head;
    /*col_num used to check if temp should go down, row_num used to check if temp should go right or left*/
    unsigned int row_num, col_num;

    for (row_num = 1; row_num <= list->dim_row; ++row_num) {
        /* temp go right when in odd row, go left when in even row*/
        int go_right;
        if (row_num % 2 == 1) go_right = 1;
        else go_right = 0;

        for (col_num = 1; col_num <= list->dim_col; ++col_num) {
            /*Don't forget free memory*/
            doubll2d_elem *ptr_for_free = temp;
            /*Move temp*/
            if (col_num == list->dim_col) temp = temp->down;
            else if (go_right == 1) temp = temp->right;
            else temp = temp->left;

            free(ptr_for_free->data), free(ptr_for_free);
        }
    }
    /*Reset other properties*/
    doubll2d_init(list);
}

doubll2d_elem *doubll2d_find_max(doubll2d *list, list_less_func *less) {
    /*Variables preparation*/
    doubll2d_elem *max;
    doubll2d_elem *temp;
    /*col_num used to check if temp should go down, row_num used to check if temp should go right or left*/
    unsigned int row_num, col_num;
    if (list == NULL || list->head == NULL) return NULL;
    else max = list->head, temp = list->head;

    for (row_num = 1; row_num <= list->dim_row; ++row_num) {
        /* temp go right when in odd row, go left when in even row*/
        int go_right;
        if (row_num % 2 == 1) go_right = 1;
        else go_right = 0;

        for (col_num = 1; col_num <= list->dim_col; ++col_num) {
            /*Compare less*/
            if (less(max->data, temp->data)) max = temp;
            /*Move temp*/
            if (col_num == list->dim_col) temp = temp->down;
            else if (go_right == 1) temp = temp->right;
            else temp = temp->left;
        }
    }
    /*Return result*/
    return max;
}

doubll2d_elem *doubll2d_find_min(doubll2d *list, list_less_func *less) {
    /*Variables preparation*/
    doubll2d_elem *min;
    doubll2d_elem *temp;
    /*col_num used to check if temp should go down, row_num used to check if temp should go right or left*/
    unsigned int row_num, col_num;
    if (list == NULL || list->head == NULL) return NULL;
    else min = list->head, temp = list->head;

    for (row_num = 1; row_num <= list->dim_row; ++row_num) {
        /* temp go right when in odd row, go left when in even row*/
        int go_right;
        if (row_num % 2 == 1) go_right = 1;
        else go_right = 0;

        for (col_num = 1; col_num <= list->dim_col; ++col_num) {
            /*Compare less*/
            if (less(temp->data, min->data)) min = temp;
            /*Move temp*/
            if (col_num == list->dim_col) temp = temp->down;
            else if (go_right == 1) temp = temp->right;
            else temp = temp->left;
        }
    }
    /*Return result*/
    return min;
}
