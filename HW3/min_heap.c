#include <stdio.h>
#include "min_heap_input.h"

void MIN_HEAPIFY(int *arr, int i)
{
    int l = 2 * i + 1;
    int r = 2 * i + 2;
    int min;
    if (l <= len && arr[i] > arr[l]) min = l;
    else min = i;

    if (r <= len && arr[min] > arr[r]) min = r;

    if (min != i)
    {
        int temp = arr[i];
        arr[i] = arr[min];
        arr[min] = temp;
        MIN_HEAPIFY(arr, min);
    }
}

int HEAP_EXTRACT_MIN(int *arr)
{
    int min = arr[0];
    arr[0] = arr[len - 1];
    len--;
    MIN_HEAPIFY(arr, 0);
    return min;
}

void HEAP_DECREASE_KEY(int *arr, int i, int key)
{
    arr[i] = key;
    while (i > 0 && arr[i / 2] > arr[i])
    {
        int temp = arr[i];
        arr[i] = arr[i / 2];
        arr[i / 2] = temp;
        i = i / 2;
    }
}

void MIN_HEAP_INSERT(int *arr, int key)
{
    HEAP_DECREASE_KEY(arr, len, key);
    len++;
}

int main()
{
    for (int i = 0; i < input_len; i++)
    {
        if (input[i] == -1)
        {
            printf("%d\n", HEAP_EXTRACT_MIN(heap));
            continue;
        }
        else if (input[i] == -2)
        {
            for (int j = 0; j < len; j++) printf("%d ", heap[j]);
            printf("\n");
        } else {
            MIN_HEAP_INSERT(heap, input[i]);
        }


    }
    return 0;
}