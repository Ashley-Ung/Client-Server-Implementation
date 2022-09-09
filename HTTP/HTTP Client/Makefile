#############################################################################
# File name:  Makefile
# Author:     chadd williams
# Date:       Sept 1, 2021
# Class:      CS360
# Assignment: Lab HTTP
# Purpose:    Build a TCP Client
#############################################################################

all: bin bin/httpget

bin:
	mkdir -p bin

bin/httpget: bin/httpget.o
	gcc -o bin/httpget -g -Wall bin/httpget.o

bin/httpget.o: bin src/httpget.c
	gcc -c -o bin/httpget.o -g -Wall src/httpget.c


clean:
	rm -f bin/httpget bin/*.o
