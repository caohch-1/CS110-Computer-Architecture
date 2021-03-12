#!/bin/bash 
gcc -c -fPIC doubll2d.c
gcc -shared -o liblist.so doubll2d.o
gcc -o dynamiclist test.c -Wl,-rpath=. liblist.so