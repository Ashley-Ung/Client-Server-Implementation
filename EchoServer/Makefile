#############################################################################
# File name:  Makefile
# Author:     chadd williams
# Date:       Sept 1, 2021
# Class:      CS360
# Assignment: 
# Purpose:    
#############################################################################

all: bin bin/main

bin:
	mkdir -p bin

bin/main: bin/main.o
	gcc -o bin/main -g -Wall bin/main.o

bin/main.o: bin src/main.c
	gcc -c -o bin/main.o -g -Wall src/main.c


clean:
	rm -f bin/main bin/*.o
