#include <stdio.h>
#include <stdlib.h>
#define CHECK_LIST
#include "doubll2d.h"

/* #define NDEBUG */
#include <assert.h>
#ifndef prt
#define prt(x) printf("%d ", x)
#endif
#define hh head
#define tt tail
typedef doubll2d list2d;
typedef doubll2d_elem elem;
elem *new_elem(int* val) {
    elem *res = (elem*)malloc(sizeof(elem));
    res->up = res->right = res->down = res->left = NULL;
    res->data = (int*)val;
    res->size = 4;
    return res;
}
int value(elem* e) {
    if (e == NULL) return -1;
    return *(int*)e->data;
}
void prt_elem(elem* e) {
    prt(value(e));
}
bool same(elem* e, int val) {
    return value(e) == val;
}
bool cmp(const doubll2d_elem* a, const doubll2d_elem* b) {
    return *(int*)a->data < *(int*)b->data;
}

int main() {
    list2d* l = (list2d*)malloc(sizeof(list2d));
    int* v[25];
    size_t s[5] = {4,4,4,4,4};
    elem *p;
    elem *e[100];
    /*elem *ee;*/
    int i, n;
    for (i = 0; i < 25; i++) {
        v[i] = (int*)malloc(sizeof(int));
        v[i][0] = i + 1;
        e[i] = new_elem(v[i]);
    }
    assert(value(e[3]) == 4);
    assert((cmp(e[2], e[1]) == 0 ? 0 : 1) == 0);
    assert((cmp(e[1], e[3]) == 0 ? 0 : 1) == 1);
    doubll2d_init(l);

    /* test insert */
    doubll2d_insert_row(l, l->head, (void**)(v+19), s, 1); /*  20 */
    assert(same(l->hh, 20));
    doubll2d_insert_row(l, l->hh, (void**)(v+16), s, 1); /*  17  */
    assert(same(l->hh->down, 17));
    doubll2d_insert_col(l, l->hh, (void**)(v+6), s, 3); /*  7 8 */
    assert(same(l->hh->right, 7));
    assert(same(l->hh->right->down, 8));
    p =  doubll2d_insert_row(l, l->hh->down->right, (void**)(v+3), s, 1);
    /*  size not enough, return NULL */
    assert(p == NULL);
    p = doubll2d_insert_col(l, NULL, (void**)(v+1), s, 1);
    assert(p == NULL);

    p = doubll2d_get_head(l);
    assert(same(p, 20));
    p = doubll2d_get_tail(l);
    assert(same(p, 8));

    p = doubll2d_insert_row(l, l->hh, (void**)(v+10), s, 2); /*  11 12 */
    assert(same(l->hh->down, 11));
    assert(same(p, 11));

    n = doubll2d_dim_row(l);
    assert(n == 3);
    p = doubll2d_insert_col(l, l->hh->down->right, (void**)(v+21), s, 3); /*  22 23 24 */
    assert(same(l->hh->right->right, 22));
    assert(same(p, 23));
    n = doubll2d_dim_col(l);
    assert(n == 3);
/*
    ee = doubll2d_new_element(*(v+11), 4);
    assert(*(int*)ee->data == 12);
#ifdef CHECK_LIST
    p = doubll2d_insert_col(l, ee, (void**)(v+11), s, 5);
    assert(p == NULL);
#endif
 */

    /* test delete */
    p = doubll2d_delete_col(l, l->hh->right);
    assert(same(p, 20));
    assert(same(l->hh->right, 22));
    n = doubll2d_dim_col(l);
    assert(n == 2);
    p = doubll2d_delete_row(l, l->hh);
    assert(same(p, 11));
    assert(same(l->hh, 11));
    assert(same(l->hh->right, 23));
    assert(same(l->tt, 24));
#ifdef CHECK_LIST
    p = doubll2d_delete_row(l, NULL);
    assert(p == NULL);
#endif
    p = doubll2d_delete_col(NULL, l->head);
    assert(p == NULL);

    /* test max & min & purge */
    p = doubll2d_find_max(l, cmp);
    assert(same(p, 24));
    p = doubll2d_find_min(l, cmp);
    assert(same(p, 11));
    doubll2d_purge(l);
    assert(l->head == NULL);
    assert(l->tail == NULL);
    assert(l->dim_col == 0);
    p = doubll2d_find_min(l, cmp);
    assert(p == NULL);

#ifndef NDEBUG
    printf("Pass all tests!\n");
#else
    printf("Finish all cases!\n");
#endif
    free(l);
    for (i = 0; i < 25; i++) {
        free(v[i]);
        free(e[i]);
    }
    return 0;
}
