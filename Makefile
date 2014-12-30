OPT= -O -m32

all: main32

main32: main32.o multiply.o myutil.o
	gcc $(OPT) -o main32 main32.o multiply.o myutil.o -lgmp -lrt

main32.o: main32.c multiply.h
	gcc $(OPT) -c main32.c

multiply.o: multiply.c multiply.h
	gcc $(OPT) -c multiply.c

myutil.o: myutil.c
	gcc $(OPT) -c myutil.c

clean: 
	rm *.o main32

