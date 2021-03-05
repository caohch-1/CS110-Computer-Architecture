#include <stdio.h>
#include "doubll2d.h"
#include <stdlib.h>

int main() {
    doubll2d list;
    int** data = (int**)malloc(sizeof(int *));
    size_t *size = (size_t *)malloc(sizeof(size_t));
    size_t length = 1;

    *size = sizeof(int);
    data[0] = (int*)malloc(sizeof(int));
    data[0][0] = 100;

    doubll2d_init(&list);
    doubll2d_insert_row(&list, list.head,(void *) data, size, length);


    data[0][0] = 200;
    doubll2d_insert_row(&list, list.head,(void *) data, size, length);


    data[0][0] = 300;
    doubll2d_insert_row(&list, list.head->down,(void *) data, size, length);


    data[0][0] = 400;
    doubll2d_insert_row(&list, list.head->down->down,(void *) data, size, length);

    printf("%d-%d-%d-%d", *((int *)list.head->data), *((int *)list.head->down->data), *((int *)list.head->down->down->data), *((int *)list.head->down->down->down->data));



    return 0;
}
