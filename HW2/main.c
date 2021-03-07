#include <stdio.h>
#include "doubll2d.h"
#include <stdlib.h>
#include <time.h>

bool less(const void *a, const void *b) {
    if (*((int *)a) < *((int *)b)) {
        return 1;
    } else {
        return 0;
    }
}

void travel(doubll2d *list) {
    doubll2d_elem *temp = list->head;
    unsigned int row_num, col_num;

    for (row_num = 1; row_num <= list->dim_row; ++row_num) {
        for (col_num = 1; col_num <= list->dim_col; ++col_num) {
            printf("%d ", *((int *)temp->data));

            if (col_num == list->dim_col && row_num == list->dim_row) {
                return;
            } else if (col_num == list->dim_col && row_num != list->dim_row) {
                temp = temp->down;
                while (temp->left != NULL) temp = temp->left;
            } else {
                temp = temp->right;
            }
        }
        printf("\n");
    }
}


int main() {
    int** test_data = (int **)malloc(10 * sizeof(int *));
    int i;

    doubll2d test_list;
    size_t* size = (size_t *)malloc(sizeof(size_t));
    *size = sizeof(int);

    srand((unsigned)time(NULL));
    for (i = 0; i < 10; i++) {
        test_data[i] = (int *)malloc(sizeof(int));
        test_data[i][0] = rand()%1000;
        printf("%d ",test_data[i][0]);
    }
    printf("\n\n");

    doubll2d_init(&test_list);

    doubll2d_insert_row(&test_list, test_list.head, (void **)test_data, size, 10);
    doubll2d_insert_row(&test_list, test_list.head, (void **)test_data, size, 10);
    doubll2d_insert_col(&test_list, test_list.head, (void **)test_data, size, 10);
    doubll2d_insert_col(&test_list, test_list.head, (void **)test_data, size, 10);
    travel(&test_list);
    printf("\n\n");

    doubll2d_insert_row(&test_list, test_list.head, (void **)test_data, size, 10);
    travel(&test_list);
    printf("\n\n");

    doubll2d_insert_row(&test_list, test_list.head->right->right, (void **)test_data, size, 10);
    travel(&test_list);
    printf("\n\n");

    doubll2d_insert_row(&test_list, test_list.head->down->down->down, (void **)test_data, size, 10);
    travel(&test_list);
    printf("\n\n");

    doubll2d_purge(&test_list);
    travel(&test_list);
    printf("\n\n");



    return 0;
}
