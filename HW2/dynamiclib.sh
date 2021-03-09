gcc -c -Wpedantic -Wall -Wextra -Werror -std=c89 -fpic doubll2d.c
gcc -Wpedantic -Wall -Wextra -Werror -std=c89 -shared -o liblist.so doubll2d.o
gcc -Wpedantic -Wall -Wextra -Werror -std=c89 -L. -o dynamiclist test.c -l:liblist.so