#include <stdio.h>
#include <stdint.h>
#include <time.h>

#define PT(tuple) printf("Size of %s: %u\n", tuple.type, tuple.size)

struct Tuple {
    char* type;
    unsigned int size;
};

int main() {
    struct Tuple t1 = {"char", sizeof(char)};
    struct Tuple t2 = {"short", sizeof(short)};
    struct Tuple t3 = {"short int", sizeof(short int)};
    struct Tuple t4 = {"int", sizeof(int)};
    struct Tuple t5 = {"long int", sizeof(long int)};
    struct Tuple t6 = {"unsigned int", sizeof(unsigned int)};
    struct Tuple t7 = {"void *", sizeof(void *)};
    struct Tuple t8 = {"size_t", sizeof(size_t)};
    struct Tuple t9 = {"float", sizeof(float)};
    struct Tuple t10 = {"double", sizeof(double)};
    struct Tuple t11 = {"int8_t", sizeof(int8_t)};
    struct Tuple t12 = {"int16_t", sizeof(int16_t)};
    struct Tuple t13 = {"int32_t", sizeof(int32_t)};
    struct Tuple t14 = {"int64_t", sizeof(int64_t)};
    struct Tuple t15 = {"time_t", sizeof(time_t)};
    struct Tuple t16 = {"clock_t", sizeof(clock_t)};
    struct Tuple t17 = {"struct tm", sizeof(struct tm)};
    struct Tuple t18 = {"NULL", sizeof(NULL)};
    PT(t1);
    PT(t2);
    PT(t3);
    PT(t4);
    PT(t5);
    PT(t6);
    PT(t7);
    PT(t8);
    PT(t9);
    PT(t10);
    PT(t11);
    PT(t12);
    PT(t13);
    PT(t14);
    PT(t15);
    PT(t16);
    PT(t17);
    PT(t18);


    return 0;

}
