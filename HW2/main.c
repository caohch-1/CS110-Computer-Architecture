#include <stdio.h>
#include "doubll2d.h"
#include <stdlib.h>

int main() {
    doubll2d_elem *test_return;
    doubll2d list;
    int **data = (int **) malloc(sizeof(int *));
    size_t *size = (size_t *) malloc(sizeof(size_t));
    size_t length = 1;

    int **data2 = (int **) malloc(5 * sizeof(int *));
    size_t *size2 = (size_t *) malloc(sizeof(size_t));
    size_t length2 = 4;
    *size2 = sizeof(int);
    data2[0] = (int *) malloc(sizeof(int));
    data2[0][0] = 1000;
    data2[1] = (int *) malloc(sizeof(int));
    data2[1][0] = 2000;
    data2[2] = (int *) malloc(sizeof(int));
    data2[2][0] = 3000;
    data2[3] = (int *) malloc(sizeof(int));
    data2[3][0] = 4000;

    *size = sizeof(int);
    data[0] = (int *) malloc(sizeof(int));
    data[0][0] = 100;

    doubll2d_init(&list);
    doubll2d_insert_row(&list, list.head, (void **) data, size, length);


    data[0][0] = 200;
    doubll2d_insert_row(&list, list.head, (void **) data, size, length);


    data[0][0] = 300;
    doubll2d_insert_row(&list, list.head->down, (void **) data, size, length);


    data[0][0] = 400;
    doubll2d_insert_row(&list, list.head->down->down, (void **) data, size, length);


    printf("%d-%d-%d-%d\n", *((int *) list.head->data), *((int *) list.head->down->data),
           *((int *) list.head->down->down->data), *((int *) list.head->down->down->down->data));

    doubll2d_insert_col(&list, list.head->down->down->up, (void **) data2, size2, length2);

    printf("%d-%d-%d-%d\n", *((int *) list.head->right->data), *((int *) list.head->down->right->data),
           *((int *) list.head->down->down->right->data), *((int *) list.tail->data));

    doubll2d_delete_row(&list, list.tail);

    test_return = doubll2d_delete_col(&list, list.head->right);

    printf("%d-%d-%d\n", *((int *) list.head->data), *((int *) list.head->down->data),
           *((int *) list.head->down->down->data));
    printf("%d\n", *((int *) test_return->data));

    doubll2d_purge(&list);


    return 0;
}
