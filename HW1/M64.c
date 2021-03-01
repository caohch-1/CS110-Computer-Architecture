#include <stdio.h>
#include <stdint.h>
#include <time.h>
#define PT(type) printf("Size of %s: %lu\n", #type, sizeof(type))

int main() {
    PT(char);
    PT(short);
    PT(short int);
    PT(int);
    PT(long int);
    PT(unsigned int);
    PT(void *);
    PT(size_t);
    PT(float);
    PT(double);
    PT(int8_t);
    PT(int16_t);
    PT(int32_t);
    PT(int64_t);
    PT(time_t);
    PT(clock_t);
    PT(struct tm);
    PT(NULL);
    return 0;
}
