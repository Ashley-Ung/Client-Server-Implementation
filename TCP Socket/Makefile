#############################################################################
# File name:  Makefile
# Author:     Chadd Williams
# Date:       Sept 10, 2018
# Class:      CS300
# Assignment: TCP Lab
# Purpose:    Build a small TCP client
#############################################################################

all: bin bin/TCP_Lab

bin:
	mkdir -p bin

bin/TCP_Lab: bin/TCP_Lab.o
	gcc -o bin/TCP_Lab -g -Wall bin/TCP_Lab.o

bin/TCP_Lab.o: bin src/TCP_Lab.c
	gcc -c -o bin/TCP_Lab.o -g -Wall src/TCP_Lab.c


clean:
	rm -f bin/TCP_Lab bin/*.o
