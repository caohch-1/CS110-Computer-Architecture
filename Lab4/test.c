#include <stdio.h>

int main() {
    int i = 0;
    int test_num = 0x00010000;
    // printf("%0x", test_num<<31);
    for(i=31; i>= 0; i--) {
        if ((test_num >> 31) != 0) break;
        test_num = test_num<<1;
    }

    printf("%d\n", i);
}