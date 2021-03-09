#include <stdlib.h>
#include <stdio.h>

struct vector_t {
    size_t size;
    int *data;
};

typedef struct vector_t vector_t;

/* Utility function to handle allocation failures. In this
   case we print a message and exit. */
static void allocation_failed() {
    fprintf(stderr, "Out of memory.\n");
    exit(1);
}

/* Bad example of how to create a new vector */
vector_t *bad_vector_new() {
    /* Create the vector and a pointer to it */
    vector_t *retval, v;
    retval = &v;

    /* Initialize attributes */
    retval->size = 1;
    retval->data = malloc(sizeof(int));
    if (retval->data == NULL) {
        allocation_failed();
    }

    retval->data[0] = 0;
    printf("external retval:%p, v:%p\n", (void *)&retval, (void *)&v);
    // printf("debug here: %d\n", *v.data);

    return retval;
}

vector_t also_bad_vector_new() {
    /* Create the vector */
    vector_t v;

    /* Initialize attributes */
    v.size = 1;
    v.data = malloc(sizeof(int));
    if (v.data == NULL) {
        allocation_failed();
    }
    v.data[0] = 10;
    printf("internal: %p\n", (void *)&v);
    return v;
}

int main() {
    // vector_t *test = bad_vector_new();
    // printf("external :%p\n", (void *)test);
    // free(test->data);

    vector_t test = also_bad_vector_new();
    printf("external: %p\n", (void *)&test);
    printf("debug here: %d\n", *test.data);
    free(test.data);

    return 0;
}
