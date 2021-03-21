#include <stdio.h>
#include <stdlib.h>

#include "doubll2d.h"

#define DATA_SIZE 10

void data_shift(int** data, int shift);
bool less(const doubll2d_elem* a, const doubll2d_elem* b);
bool rand_less(const doubll2d_elem* a, const doubll2d_elem* b);
void check_null_list(doubll2d *list);

int main()
{
    size_t i = 0, j = 0;
    int **data = (int **)malloc(sizeof(int *) * DATA_SIZE);
    size_t *size = (size_t *)malloc(sizeof(size_t) * DATA_SIZE);
    doubll2d *list = NULL;
    doubll2d_elem *max, *min;
    doubll2d_elem *current, *current_row, *return_cursor;
    srand(1);

    /* data and size */
    for (i = 0; i < DATA_SIZE; i++)
    {
        data[i] = (int *)malloc(sizeof(int));
        *(data[i]) = i + j;
        size[i] = sizeof(*(data[i]));
    }

    printf("Test if list == NULL.\n");
    check_null_list(list);
    putchar('\n');

    list = (doubll2d *)malloc(sizeof(doubll2d));
    doubll2d_init(list);
    printf("Test if list is alloced ahead and initialized.\n");
    check_null_list(list);

    putchar('\n');

    printf("Test insert, delete.\n");
    return_cursor = doubll2d_delete_row(list, list->tail);
    printf("doubll2d_dim_row(list) = %lu[0], doubll2d_dim_col(list) = %lu[0].\n", doubll2d_dim_row(list), doubll2d_dim_col(list));
    printf("return_cursor = %p[%p].\n", (void*)return_cursor, NULL);
    return_cursor = doubll2d_delete_col(list, list->head);
    printf("doubll2d_dim_row(list) = %lu[0], doubll2d_dim_col(list) = %lu[0].\n", doubll2d_dim_row(list), doubll2d_dim_col(list));
    printf("return_cursor = %p[%p].\n", (void*)return_cursor, NULL);
    doubll2d_insert_row(list, NULL, (void**)data, size, 5);
    printf("doubll2d_dim_row(list) = %lu[1], doubll2d_dim_col(list) = %lu[1].\n", doubll2d_dim_row(list), doubll2d_dim_col(list));
    data_shift(data, 10);
    doubll2d_insert_row(list, list->head, (void**)data, size, 5);
    printf("doubll2d_dim_row(list) = %lu[2], doubll2d_dim_col(list) = %lu[1].\n", doubll2d_dim_row(list), doubll2d_dim_col(list));
    data_shift(data, 10);
    doubll2d_insert_row(list, list->head, (void**)data, size, 5);
    printf("doubll2d_dim_row(list) = %lu[3], doubll2d_dim_col(list) = %lu[1].\n", doubll2d_dim_row(list), doubll2d_dim_col(list));
    data_shift(data, 10);
    doubll2d_insert_row(list, list->head->down, (void**)data, size, 5);
    printf("doubll2d_dim_row(list) = %lu[4], doubll2d_dim_col(list) = %lu[1].\n", doubll2d_dim_row(list), doubll2d_dim_col(list));
    data_shift(data, 10);
    doubll2d_insert_row(list, list->tail->up, (void**)data, size, 5);
    printf("doubll2d_dim_row(list) = %lu[5], doubll2d_dim_col(list) = %lu[1].\n", doubll2d_dim_row(list), doubll2d_dim_col(list));
    data_shift(data, 10);
    doubll2d_insert_col(list, list->head, (void**)data, size, 5);
    printf("doubll2d_dim_row(list) = %lu[5], doubll2d_dim_col(list) = %lu[2].\n", doubll2d_dim_row(list), doubll2d_dim_col(list));
    data_shift(data, 10);
    doubll2d_insert_col(list, list->head, (void**)data, size, 5);
    printf("doubll2d_dim_row(list) = %lu[5], doubll2d_dim_col(list) = %lu[3].\n", doubll2d_dim_row(list), doubll2d_dim_col(list));
    data_shift(data, 10);
    doubll2d_insert_col(list, list->head->right, (void**)data, size, 5);
    printf("doubll2d_dim_row(list) = %lu[5], doubll2d_dim_col(list) = %lu[4].\n", doubll2d_dim_row(list), doubll2d_dim_col(list));
    data_shift(data, -10);
    doubll2d_insert_col(list, list->tail->left, (void**)data, size, 5);
    printf("doubll2d_dim_row(list) = %lu[5], doubll2d_dim_col(list) = %lu[5].\n", doubll2d_dim_row(list), doubll2d_dim_col(list));
    data_shift(data, -10);
    doubll2d_insert_col(list, list->head, (void**)data, size, 5);
    printf("doubll2d_dim_row(list) = %lu[5], doubll2d_dim_col(list) = %lu[6].\n", doubll2d_dim_row(list), doubll2d_dim_col(list));
    data_shift(data, -10);
    doubll2d_insert_row(list, list->head, (void**)data, size, 5);
    printf("doubll2d_dim_row(list) = %lu[5], doubll2d_dim_col(list) = %lu[6].\n", doubll2d_dim_row(list), doubll2d_dim_col(list));
    data_shift(data, -10);
    doubll2d_insert_row(list, list->head, NULL, size, 5);
    printf("doubll2d_dim_row(list) = %lu[5], doubll2d_dim_col(list) = %lu[6].\n", doubll2d_dim_row(list), doubll2d_dim_col(list));
    data_shift(data, -10);
    return_cursor = doubll2d_delete_row(list, list->head);
    printf("doubll2d_dim_row(list) = %lu[4], doubll2d_dim_col(list) = %lu[6].\n", doubll2d_dim_row(list), doubll2d_dim_col(list));
    printf("return_cursor = %p[%p].\n", (void*)return_cursor, (void*)list->head);
    data_shift(data, -10);
    return_cursor = doubll2d_delete_row(list, NULL);
    printf("doubll2d_dim_row(list) = %lu[4], doubll2d_dim_col(list) = %lu[6].\n", doubll2d_dim_row(list), doubll2d_dim_col(list));
    printf("return_cursor = %p[%p].\n", (void*)return_cursor, NULL);
    data_shift(data, -10);
    return_cursor = doubll2d_delete_row(list, list->tail);
    printf("doubll2d_dim_row(list) = %lu[3], doubll2d_dim_col(list) = %lu[6].\n", doubll2d_dim_row(list), doubll2d_dim_col(list));
    printf("return_cursor = %p[%p].\n", (void*)return_cursor, (void*)list->tail);
    data_shift(data, -10);
    return_cursor = doubll2d_delete_col(list, list->head);
    printf("doubll2d_dim_row(list) = %lu[3], doubll2d_dim_col(list) = %lu[5].\n", doubll2d_dim_row(list), doubll2d_dim_col(list));
    printf("return_cursor = %p[%p].\n", (void*)return_cursor, (void*)list->head);
    data_shift(data, -10);
    return_cursor = doubll2d_delete_col(list, list->tail);
    printf("doubll2d_dim_row(list) = %lu[3], doubll2d_dim_col(list) = %lu[4].\n", doubll2d_dim_row(list), doubll2d_dim_col(list));
    printf("return_cursor = %p[%p].\n", (void*)return_cursor, (void*)list->tail);
    data_shift(data, 10);
    doubll2d_insert_row(list, list->tail->up, (void**)data, size, 5);
    printf("doubll2d_dim_row(list) = %lu[4], doubll2d_dim_col(list) = %lu[4].\n", doubll2d_dim_row(list), doubll2d_dim_col(list));
    data_shift(data, 10);
    doubll2d_insert_col(list, list->head, (void**)data, size, 5);
    printf("doubll2d_dim_row(list) = %lu[4], doubll2d_dim_col(list) = %lu[5].\n", doubll2d_dim_row(list), doubll2d_dim_col(list));
    data_shift(data, 10);
    doubll2d_insert_row(list, list->tail->up, (void**)data, size, 5);
    printf("doubll2d_dim_row(list) = %lu[5], doubll2d_dim_col(list) = %lu[5].\n", doubll2d_dim_row(list), doubll2d_dim_col(list));
    data_shift(data, 10);
    doubll2d_insert_col(list, list->head, (void**)data, size, 5);
    printf("doubll2d_dim_row(list) = %lu[5], doubll2d_dim_col(list) = %lu[6].\n", doubll2d_dim_row(list), doubll2d_dim_col(list));
    data_shift(data, 10);

    max = doubll2d_find_max(list, less);
    min = doubll2d_find_min(list, less);
    printf("max = %d[73], min = %d[-10].\n", (*(int*)max->data), (*(int*)min->data));
    max = doubll2d_find_max(list, rand_less);
    min = doubll2d_find_min(list, rand_less);
    printf("In linux gcc: max = %d[73], min = %d[63].\n", (*(int*)max->data), (*(int*)min->data));

    current_row = list->head;
    /* row scanning */
    while (current = current_row, current_row != NULL) {
        for (i = 0; i < doubll2d_dim_col(list); i++, current = current->right)
            printf("data = %d\t", *(int*)(current->data));

        putchar('\n');
        current_row = current_row->down;
    }


    printf("doubll2d_dim_row(list) = %lu[5], doubll2d_dim_col(list) = %lu[6].\n", doubll2d_dim_row(list), doubll2d_dim_col(list));
    doubll2d_purge(list);
    printf("doubll2d_dim_row(list) = %lu[0], doubll2d_dim_col(list) = %lu[0].\n", doubll2d_dim_row(list), doubll2d_dim_col(list));
    for (i = 0; i < DATA_SIZE; i++)
        free(data[i]);
    free(data);
    free(size);
    free(list);
    return 0;
}

void data_shift(int** data, int shift)
{
    int i = 0;
    for (i = 0; i < DATA_SIZE; i++)
    {
        *(data[i]) += shift;
    }
}

bool less(const doubll2d_elem* a, const doubll2d_elem* b)
{
    return *((int*)(a->data)) < *((int*)(b->data));
}

bool rand_less(const doubll2d_elem* a, const doubll2d_elem* b)
{
    int r = rand();
    int x = (*((int*)(a->data)) * r) % RAND_MAX;
    int y = (*((int*)(b->data)) * r * r) % RAND_MAX;
    return x < y;
}


void check_null_list(doubll2d *list)
{
    if (doubll2d_get_head(list) == NULL)
        printf("doubll2d_get_head(list) gets NULL[yes].\n");
    else
        printf("doubll2d_get_head(list) gets head[no].\n");
    if (doubll2d_get_tail(list) == NULL)
        printf("doubll2d_get_tail(list) gets NULL[yes].\n");
    else
        printf("doubll2d_get_tail(list) gets tail[no].\n");
    if (list == NULL)
        printf("doubll2d_dim_row(list) = %lu[-1], doubll2d_dim_col(list) = %lu[-1].\n", doubll2d_dim_row(list), doubll2d_dim_col(list));
    else
        printf("doubll2d_dim_row(list) = %lu[0], doubll2d_dim_col(list) = %lu[0].\n", doubll2d_dim_row(list), doubll2d_dim_col(list));
}
/*if (doubll2d_insert_row(list, NULL, data, size, DATA_SIZE) == NULL) {
    printf("doubll2d_insert_row(list, NULL, data, size, DATA_SIZE) gets NULL[yes].\n");
} else {
    printf("doubll2d_insert_row(list, NULL, data, size, DATA_SIZE) gets something[no].\n");
}
if (doubll2d_insert_col(list, NULL, data, size, DATA_SIZE) == NULL) {
    printf("doubll2d_insert_col(list, NULL, data, size, DATA_SIZE) gets NULL[yes].\n");
} else {
    printf("doubll2d_insert_col(list, NULL, data, size, DATA_SIZE) gets something[no].\n");
}
if (doubll2d_delete_row(list, NULL) == NULL) {
    printf("doubll2d_delete_row(list, NULL) gets NULL[yes].\n");
} else {
    printf("doubll2d_delete_row(list, NULL) gets something[no].\n");
}
if (doubll2d_delete_col(list, NULL) == NULL) {
    printf("doubll2d_delete_col(list, NULL) gets NULL[yes].\n");
} else {
    printf("doubll2d_delete_col(list, NULL) gets something[no].\n");
}*/
