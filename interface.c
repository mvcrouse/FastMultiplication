/*
This file is where you should call your multiplications
*/
#include <stdio.h>
#include "multiply.h"
#include <string.h>
#include <limits.h>

int main() {

  /*
    |********************| 
    |YOUR INPUT GOES HERE|
    |********************|
  */

  unsigned int input1[7] = { 1, 1, 1, 1, 1, 1, 1 };

  unsigned int input2[6] = { 1, 0, 0, 1, 1, 0 };

  unsigned int base = 2;

  /*
    |********************| 
    |     RESULT WILL    |
    |     BE PRINTED     |
    |********************|
  */

  multiply_numbers(input1, input2, sizeof(input1) / sizeof(int), sizeof(input2) / sizeof(int), base);

  return 0;
}

