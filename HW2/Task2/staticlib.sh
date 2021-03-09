gcc -Wpedantic -Wall -Wextra -Werror -std=c89 -c doubll2d.c -o doubll2d.o
ar rcs liblist.a doubll2d.o
gcc -Wpedantic -Wall -Wextra -Werror -std=c89 -static test.c liblist.a -o staticlist