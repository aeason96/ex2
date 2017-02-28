gcc -fPIC -Wall -c verboselock.c
gcc -shared -o libverboselock.so.1.0.1 verboselock.o -ldl

