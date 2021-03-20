#include <stdio.h>

int main() {
    int i = 0;
    int test_num = 0x0010;
    while ((test_num>>31) != 0x1) {
        test_num = test_num<<1;
        printf("%0x\t", (test_num>>31));
        i ++;
    }

    printf("%d\n", i);
}