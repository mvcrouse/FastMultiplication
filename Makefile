OPT= -O 

CC = gcc-4.8

all: interface

interface: interface.o multiply.o addition.o subtraction.o division.o arrayExpansion.o
	$(CC) $(OPT) -w -o result interface.o multiply.o addition.o subtraction.o division.o arrayExpansion.o

interface.o: interface.c multiply.h
	$(CC) $(OPT) -c interface.c

multiply.o: multiply.c multiply.h
	$(CC) $(OPT) -c multiply.c

addition.o: addition.c addition.h
	$(CC) $(OPT) -c addition.c

subtraction.o: subtraction.c subtraction.h
	$(CC) $(OPT) -c subtraction.c

division.o: division.c division.h
	$(CC) $(OPT) -c division.c

arrayExpansion.o: arrayExpansion.c arrayExpansion.h
	$(CC) $(OPT) -c arrayExpansion.c

clean: 
	make -i clear
	clear
clear:
	rm *~
	rm *.o main32
	rm result
