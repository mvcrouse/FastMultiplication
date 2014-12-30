OPT= -O 
#-m32

all: main32

main32: main32.o multiply.o myutil.o addition.o subtraction.o arrayExpansion.o
	gcc-4.8 $(OPT) -o main32 main32.o multiply.o myutil.o addition.o subtraction.o arrayExpansion.o -lgmp
# -lrt

main32.o: main32.c multiply.h
	gcc-4.8 $(OPT) -c main32.c

multiply.o: multiply.c multiply.h
	gcc-4.8 $(OPT) -c multiply.c

myutil.o: myutil.c
	gcc-4.8 $(OPT) -c myutil.c

addition.o: addition.c addition.h
	gcc-4.8 $(OPT) -c addition.c

subtraction.o: subtraction.c subtraction.h
	gcc-4.8 $(OPT) -c subtraction.c

arrayExpansion.o: arrayExpansion.c arrayExpansion.h
	gcc-4.8 $(OPT) -c arrayExpansion.c

clean: 
	make -i clear
	clear
clear:
	rm *~
	rm *.o main32

