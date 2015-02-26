FastMultiplication
=======================

Uses naive, Karatsuba, and Toom-Cook 3-Way for fast multiplication of integers with size 32 bit digits. Designed for Mac

It may require GMP, so if that's the case and you don't have it installed, the multiply, add, subtract, and divide files
all are fine on their own, so just remove any instances where GMP is called

To run:

make all

./test_32.sh
