#include <stdio.h>
#include <stdint.h>
#include <time.h>

#define PT(type, size) printf("Size of %s: %lu\n", type, size)

int main() {
    PT("char", sizeof(char ));
    PT("short", sizeof(short ));
    PT("short int", sizeof(short int));
    PT("int", sizeof(int));
    PT("long int", sizeof(long int));
    PT("unsigned int", sizeof(unsigned int));
    PT("void *", sizeof(void *));
    PT("size_t", sizeof(size_t));
    PT("float", sizeof(float));
    PT("double", sizeof(double));
    PT("int8_t", sizeof(int8_t));
    PT("int16_t", sizeof(int16_t));
    PT("int32_t", sizeof(int32_t));
    PT("int64_t", sizeof(int64_t));
    PT("time_t", sizeof(time_t));
    PT("clock_t", sizeof(clock_t));
    PT("struct tm", sizeof(struct tm));
    PT("NULL", sizeof(NULL));
    return 0;
}
