#include "doubll2d.h"
#include <stdlib.h>
#include <string.h>


int static find_cursor(doubll2d *list, doubll2d_elem *cursor) {
    /*temp used to travel whole 2d-list*/
    doubll2d_elem *temp = list->head;
    /* 2 num used to check if temp should go down
     * In other word, if all elements in current row has been visited*/
    unsigned int row_num, col_num;
    /*exist_cursor is a flag to check if cursor has been found*/
    int exist_cursor = 0;

    for (row_num = 1; row_num <= list->dim_row; ++row_num) {
        /* temp go right when in odd row,
         * go left when in even row*/
        int go_right;
        if (row_num % 2 == 1) go_right = 1;
        else go_right = 0;

        for (col_num = 1; col_num <= list->dim_col; ++col_num) {
            /*Find Cursor!*/
            if (temp == cursor) {
                exist_cursor = 1;
                break;
            }

            /* col_num isn't index but num of visited node in current row_num
             * so use it to check if temp should go down*/
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
    list->head = NULL, list->tail = NULL;;

    /*Init size of 2d-List */
    list->dim_row = 0, list->dim_col = 0;;
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
    /*Check if the marco CHECK_LIST is set*/
#ifdef CHECK_LIST
    /*Check whether list is empty, if true than just skip check*/
    if (list->dim_col != 0 && list->dim_row != 0 && find_cursor(list, cursor) == 0) {
        abort();
        return NULL;
    }
#endif
    /* Init the first Node
     * Malloc memory for it
     * Set four pointers to null
     * Set its data to first node of input data*/
    if (list->dim_col == 0 && list->dim_row == 0) {
        doubll2d_elem *temp = (doubll2d_elem *) malloc(sizeof(doubll2d_elem));
        if (temp == NULL) {
            abort();
            return NULL;
        }
        temp->up = NULL, temp->down = NULL, temp->left = NULL, temp->right = NULL;
        /*Deep copy data*/
        temp->data = malloc(*size);
        if (temp->data == NULL) {
            abort();
            return NULL;
        }
        memcpy(temp->data, data[0], *size);
        temp->size = *size;
        /*Set head and tail*/
        list->head = temp, list->tail = temp;

        /*Update row and col num*/
        list->dim_row++, list->dim_col++;

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
        doubll2d_elem **new_row = (doubll2d_elem **) malloc(list->dim_col * sizeof(doubll2d_elem *));
        if (new_row == NULL) {
            abort();
            return NULL;
        }
        for (i = 0; i < list->dim_col; ++i) {
            new_row[i] = (doubll2d_elem *) malloc(sizeof(doubll2d_elem));
            if (new_row[i] == NULL) {
                abort();
                return NULL;
            }
        }

        /*Second copy data into new row*/
        for (i = 0; i < list->dim_col; ++i) {
            /*Deep copy data and Check if succeed*/
            new_row[i]->data = malloc(*size);
            if (new_row[i]->data == NULL) {
                abort();
                return NULL;
            }
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

        /*Third, move current_node to the head of this row*/
        while (current_node->left != NULL) current_node = current_node->left;

        /*Fourth, from head insert new row*/
        for (i = 0; i < list->dim_col; ++i) {
            /* Actually, our work is to address every new node's vertical pointers
             * and down pointers of nodes in cursor's row
             * and up pointers of nodes in original next row of cursor's row*/
            new_row[i]->up = current_node, new_row[i]->down = current_node->down, current_node->down = new_row[i];
            /*Check if new_row become the bottom row*/
            if (new_row[i]->down != NULL) new_row[i]->down->up = new_row[i];
            /*Move to next(right) node*/
            current_node = current_node->right;
        }

        /*If new_row is bottom row, assign tail to last node in new_row*/
        if (new_row[0]->down == NULL) list->tail = new_row[list->dim_col - 1];
        free(new_row);
    }


    list->dim_row++;
    return cursor->down;
}

doubll2d_elem *doubll2d_insert_col(doubll2d *list, doubll2d_elem *cursor,
                                   void **data, size_t *size, size_t length) {
#ifdef CHECK_LIST
    /*Check whether list is empty, if true than just skip check*/
    if (list->dim_col != 0 && list->dim_row != 0 && find_cursor(list, cursor) == 0) {
        abort();
        return NULL;
    }
#endif
    /* Init the first Node
     * Malloc memory for it
     * Set four pointers to null
     * Set its data to first node of input data*/
    if (list->dim_col == 0 && list->dim_row == 0) {
        doubll2d_elem *temp = (doubll2d_elem *) malloc(sizeof(doubll2d_elem));
        if (temp == NULL) {
            abort();
            return NULL;
        }
        temp->up = NULL, temp->down = NULL, temp->left = NULL, temp->right = NULL;

        /*Deep copy data*/
        temp->data = malloc(*size);
        if (temp->data == NULL) {
            abort();
            return NULL;
        }
        memcpy(temp->data, data[0], *size);
        temp->size = *size;
        /*Set head and tail*/
        list->head = temp, list->tail = temp;;
        /*Update row and col num*/
        list->dim_row++, list->dim_col++;
        return temp;
    }

    /*Check if length is shorter than it should be (list->dim_row)*/
    if (length < list->dim_row) {
        abort();
        return NULL;
    } else {
        /*Used in Third*/
        doubll2d_elem *current_node = cursor;
        /*Used in For statement*/
        unsigned int i;

        /*First malloc memory for new col and Check if succeed*/
        doubll2d_elem **new_col = (doubll2d_elem **) malloc(list->dim_row * sizeof(doubll2d_elem *));
        if (new_col == NULL) {
            abort();
            return NULL;
        }
        for (i = 0; i < list->dim_row; ++i) {
            new_col[i] = (doubll2d_elem *) malloc(sizeof(doubll2d_elem));
            if (new_col[i] == NULL) {
                abort();
                return NULL;
            }
        }

        /*Second copy data into new col*/
        for (i = 0; i < list->dim_row; ++i) {
            /*Deep copy data and Check if succeed*/
            new_col[i]->data = malloc(*size);
            if (new_col[i]->data == NULL) {
                abort();
                return NULL;
            }
            memcpy(new_col[i]->data, data[i], *size);

            new_col[i]->size = *size;
            /*Address new nodes' vertical pointers*/
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

        /*Third, move current_node to the head of this col*/
        while (current_node->up != NULL) current_node = current_node->up;

        /*Fourth, from head insert new col*/
        for (i = 0; i < list->dim_row; ++i) {
            /* Actually, our work is to address every new node's horizontal pointers
             * and right pointers of nodes in cursor's row
             * and left pointers of nodes in original next row of cursor's row*/
            new_col[i]->left = current_node, new_col[i]->right = current_node->right, current_node->right = new_col[i];

            /*Check if new_col become the rightest row*/
            if (new_col[i]->right != NULL) new_col[i]->right->left = new_col[i];
            /*Move to next(down) node*/
            current_node = current_node->down;
        }

        /*If new_col is rightest row, assign tail to last node in new_col*/
        if (new_col[0]->right == NULL) list->tail = new_col[list->dim_row - 1];

        free(new_col);
    }

    list->dim_col++;
    return cursor->right;
}

doubll2d_elem *doubll2d_delete_row(doubll2d *list, doubll2d_elem *cursor) {
    /*Used to travel nodes to be removed*/
    doubll2d_elem *current_node = cursor;
    /*Used in for statement*/
    unsigned int i;
    /*Used to return*/
    doubll2d_elem *return_node;

#ifdef CHECK_LIST
    /*Check whether list is empty, if true than just skip check*/
    if (find_cursor(list, cursor) == 0) {
        abort();
        return NULL;
    }
#endif
    /*First Move current_node to head of cursor's row*/
    while (current_node->left != NULL) current_node = current_node->left;

    /*Second Remove each node*/
    for (i = 0; i < list->dim_col; ++i) {
        /* Actually, our work is to address down pointers of nodes in prev row
         * and up pointers of nodes in next row
         * Warning: Don't forget to free these removed nodes*/

        if (current_node->up == NULL && current_node->down != NULL) {
            /*Special situation for delete first row*/
            /*Also Move head*/
            if (i == 0) list->head = current_node->down;
            /*Set return value*/
            if (current_node == cursor) return_node = current_node->down;

            current_node->down->up = NULL;
        } else if (current_node->down == NULL && current_node->up != NULL) {
            /*Special situation for delete bottom row*/
            /*Also Move tail*/
            if (i == list->dim_col - 1) list->tail = current_node->up;
            /*Set return value*/
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
        /*Special situation for last node in this row*/
        if (i == list->dim_col - 1) {
            free(current_node->data), free(current_node);
            break;
        }
        /*Move current_node to next(right)*/
        current_node = current_node->right;
        /*Both data and struct should be freed*/
        free(current_node->left->data), free(current_node->left);
    }

    list->dim_row--;
    return return_node;
}

doubll2d_elem *doubll2d_delete_col(doubll2d *list, doubll2d_elem *cursor) {
    /*Used to travel nodes to be removed*/
    doubll2d_elem *current_node = cursor;
    /*Used in for statement*/
    unsigned int i;
    /*Used to return*/
    doubll2d_elem *return_node;

#ifdef CHECK_LIST
    /*Check whether list is empty, if true than just skip check*/
    if (find_cursor(list, cursor) == 0) {
        abort();
        return NULL;
    }
#endif
    /*First Move current_node to top of cursor's col*/
    while (current_node->up != NULL) current_node = current_node->up;

    /*Second Remove each node*/
    for (i = 0; i < list->dim_row; ++i) {
        /* Actually, our work is to address down pointers of nodes in prev col
         * and up pointers of nodes in next col
         * Warning: Don't forget to free these removed nodes*/

        if (current_node->left == NULL && current_node->right != NULL) {
            /*Special situation for delete first col*/
            /*Also Move head*/
            if (i == 0) list->head = current_node->right;
            /*Set return value*/
            if (current_node == cursor) return_node = current_node->right;

            current_node->right->left = NULL;
        } else if (current_node->right == NULL && current_node->left != NULL) {
            /*Special situation for delete rightest col*/
            /*Also Move tail*/
            if (i == list->dim_row - 1) list->tail = current_node->left;
            /*Set return value*/
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
        /*Both data and struct should be freed*/
        free(current_node->up->data), free(current_node->up);
    }

    list->dim_col--;
    return return_node;
}

void doubll2d_purge(doubll2d *list) {
    /*temp used to travel whole 2d-list*/
    doubll2d_elem *temp = list->head;
    /* 2 num used to check if temp should go down
     * In other word, if all elements in current row has been visited*/
    unsigned int row_num, col_num;

    for (row_num = 1; row_num <= list->dim_row; ++row_num) {
        /* temp go right when in odd row,
         * gor left when in even row*/
        int go_right;
        if (row_num % 2 == 1) go_right = 1;
        else go_right = 0;

        for (col_num = 1; col_num <= list->dim_col; ++col_num) {
            /* col_num isn't index but num of visited node in current row_num
             * so use it to check if temp should go down*/
            doubll2d_elem *ptr_for_free = temp;
            if (col_num == list->dim_col) temp = temp->down;
            else if (go_right == 1) temp = temp->right;
            else temp = temp->left;

            free(ptr_for_free->data), free(ptr_for_free);
        }
    }

    doubll2d_init(list);
}

doubll2d_elem *doubll2d_find_max(doubll2d *list, list_less_func *less) {
    /*Store maximum node*/
    doubll2d_elem *max;
    /*temp used to travel whole 2d-list*/
    doubll2d_elem *temp;
    /* 2 num used to check if temp should go down
     * In other word, if all elements in current row has been visited*/
    unsigned int row_num, col_num;
    if (list == NULL || list->head == NULL) return NULL;
    else {
        max = list->head, temp = list->head;

    }

    for (row_num = 1; row_num <= list->dim_row; ++row_num) {
        /* temp go right when in odd row,
         * gor left when in even row*/
        int go_right;
        if (row_num % 2 == 1) go_right = 1;
        else go_right = 0;

        for (col_num = 1; col_num <= list->dim_col; ++col_num) {
            /*Compare less*/
            if (less(max->data, temp->data)) max = temp;
            /* col_num isn't index but num of visited node in current row_num
             * so use it to check if temp should go down*/
            if (col_num == list->dim_col) temp = temp->down;
            else if (go_right == 1) temp = temp->right;
            else temp = temp->left;
        }
    }

    return max;
}

doubll2d_elem *doubll2d_find_min(doubll2d *list, list_less_func *less) {
    /*Store minimum node*/
    doubll2d_elem *min;
    /*temp used to travel whole 2d-list*/
    doubll2d_elem *temp;
    /* 2 num used to check if temp should go down
     * In other word, if all elements in current row has been visited*/
    unsigned int row_num, col_num;
    if (list == NULL || list->head == NULL) return NULL;
    else {
        min = list->head, temp = list->head;
    }

    for (row_num = 1; row_num <= list->dim_row; ++row_num) {
        /* temp go right when in odd row,
         * gor left when in even row*/
        int go_right;
        if (row_num % 2 == 1) go_right = 1;
        else go_right = 0;

        for (col_num = 1; col_num <= list->dim_col; ++col_num) {
            /*Compare less*/
            if (less(temp->data, min->data)) min = temp;
            /* col_num isn't index but num of visited node in current row_num
             * so use it to check if temp should go down*/
            if (col_num == list->dim_col) temp = temp->down;
            else if (go_right == 1) temp = temp->right;
            else temp = temp->left;
        }
    }

    return min;
}
