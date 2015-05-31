#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
//#include <gmp.h>
#include <time.h>
#include "multiply.h"
#include "addition.h"
#include "subtraction.h"
#include "arrayExpansion.h"
#include "division.h"

void multiply_numbers(int a[], int b[], unsigned int input1_size, unsigned int input2_size, unsigned int base);

unsigned int* small_mult(unsigned int *int_a, unsigned int *int_b, unsigned int wa, unsigned int wb, unsigned int base);
unsigned int* medium_mult(unsigned int *int_a, unsigned int *int_b, unsigned int wa, unsigned int wb, unsigned int base);
unsigned int* large_mult(unsigned int *int_a, unsigned int *int_b, unsigned int wa, unsigned int wb, unsigned int base);


//takes two integers and multiplies them
void multiply_numbers(int a[], int b[], unsigned int input1_size, unsigned int input2_size, unsigned int base) {
  void *retArray = malloc((input1_size + input2_size) * sizeof(int));
  unsigned int retSize;
  unsigned int retBits;
  
  unsigned int temp;
  int i = 0, j = input1_size - 1;
  while (i < j) {
    temp = a[i];
    a[i] = a[j];
    a[j] = temp;
    i++;
    j--;
  }

  i = 0, j = input2_size - 1;
  while (i < j) {
    temp = b[i];
    b[i] = b[j];
    b[j] = temp;
    i++;
    j--;
  }
  
  void *input1 = a;
  void *input2 = b;

  Multiply(input1, input2, retArray, input1_size, input1_size * 32, input2_size, input2_size * 32, &retSize, &retBits, base);

  unsigned int *arr = (unsigned int *) input1;
  printf("\n---------------------------------------------------------------\n");
  printf("INPUT 1:\n");
  for (i = input1_size - 1; i >= 0; i--) {
    printf("%u", arr[i]);
  }
  printf("\n---------------------------------------------------------------\n");
  arr = (unsigned int *) input2;
  printf("\n---------------------------------------------------------------\n");
  printf("INPUT 2:\n");
  for (i = input2_size - 1; i >= 0; i--) {
    printf("%u", arr[i]);
  }
  printf("\n---------------------------------------------------------------\n");

  arr = (unsigned int *) retArray;
  printf("\n---------------------------------------------------------------\n");
  printf("OUTPUT: \n");
  for (i = retSize - 1; i >= 0; i--) {
    printf("%u", arr[i]);
  }
  printf("\n---------------------------------------------------------------\n");
  printf("\n");
}

/* wa is word length of a, ba is bit length of a */
//as input this takes an array of integers with each integer representing a digit, a size of each array, and a return array
void Multiply(void *a, void *b, void *c, unsigned int wa,
	      unsigned int ba, unsigned int wb, unsigned int bb, unsigned int *wc, unsigned int *bc, unsigned int base){

  unsigned int *int_a = (unsigned int *) a;
  unsigned int *int_b = (unsigned int *) b;
  unsigned int *int_c = (unsigned int *) c;
  unsigned int i;

  *wc = wb + wa;
  *bc = (wb + wa) * 32;

  //decide whether naive multiplication algorithm would be faster than karatsuba multiplication algorithm
  //based on size of number
  unsigned int *ret_c;
  if (*wc <= 200) {
    ret_c = small_mult(int_a, int_b, wa, wb, base);
  } else {
    ret_c = medium_mult(int_a, int_b, wa, wb, base);
  }
    for (i = 0; i < wa + wb; i++) {
      int_c[i] = ret_c[i];
    }

}

//naive multiplication algorithm, fast but unsuited for large numbers
unsigned int* small_mult(unsigned int *int_a, unsigned int *int_b, unsigned int wa, unsigned int wb, unsigned int base) {

    unsigned int i;
    unsigned int j;

    unsigned long long current;
    unsigned int carry;

    unsigned int *ret_c = malloc((wa + wb) * sizeof(int));
    memset(ret_c, 0, (wa + wb) * sizeof(int));

    for (i = 0; i < wa; i++) {
      carry = 0;
      for (j = 0; j < wb; j++) {
	current = int_a[i];
	current *= int_b[j];
	current += ret_c[i + j];
	current += carry;

	ret_c[i+j] = current % base;
	
	carry = current / base;
     }
      
      ret_c[i + j] = carry;
    }
    return ret_c;
}



//implementation of karatsubas algorithm for base 32
unsigned int* medium_mult(unsigned int *int_a, unsigned int *int_b, unsigned int wa, unsigned int wb, unsigned int base) {

  int i;
  int j;

  unsigned int wc = wa + wb;

  //when size of number to multiply is small enough, default to naive multiplication algorithm
  if (wc <= 156) {
    return small_mult(int_a, int_b, wa, wb, base);
  }

  unsigned int upper_a = (wa + 1) >> 1;
  unsigned int upper_b = (wb + 1) >> 1;

  //this part handles the multiplication of numbers that are digits removed from each other (i.e. 100 * 3 needs special
  //handling because everything is in array format)
  if (upper_a != upper_b) {
    if (wa >= wb && wb > 64) {
      unsigned int *lower_part = medium_mult(int_a, int_b, wb, wb, base);
      unsigned int *upper_part = medium_mult(&int_a[wb], int_b, wa - wb, wb, base);
      unsigned int *expansion = expand_array_left(upper_part, wa, wa + wb); 
      return addition(expansion, lower_part, wa + wb, wb + wb, base);
    } else if (wa > 64) {
      unsigned int *lower_part = medium_mult(int_a, int_b, wa, wa, base);
      unsigned int *upper_part = medium_mult(int_a, &int_b[wa], wa, wb - wa, base);
      unsigned int *expansion = expand_array_left(upper_part, wb, wa + wb); 
      return addition(expansion, lower_part, wb + wa, wa + wa, base);
    } else {
      return small_mult(int_a, int_b, wa, wb, base);
    }
  }

  unsigned int upper_bound_a = wa - upper_a;
  unsigned int upper_bound_b = wb - upper_b;

  //Change back to large multiplication
  unsigned int *upper_c = medium_mult(&int_a[upper_a], &int_b[upper_b], upper_bound_a, upper_bound_b, base);
  unsigned int *lower_c = medium_mult(int_a, int_b, upper_a, upper_b, base);

  unsigned int *add_a = addition(int_a, &int_a[upper_a], upper_a, upper_bound_a, base);
  unsigned int *add_b = addition(int_b, &int_b[upper_b], upper_b, upper_bound_b, base);

  int upper_a_greater = upper_a > upper_bound_a;
  int upper_b_greater = upper_b > upper_bound_b;

  unsigned int mid_size_a = upper_a_greater ? upper_a + 1 : (upper_bound_a) + 1;
  unsigned int mid_size_b = upper_b_greater ? upper_b + 1: (upper_bound_b) + 1;

  unsigned int *mid_c = medium_mult(add_a, add_b, mid_size_a, mid_size_b, base);
  unsigned int mid_size = mid_size_a + mid_size_b;

  unsigned int *sub_components = subtraction(mid_c, upper_c, lower_c, mid_size, upper_bound_a + upper_bound_b, upper_a + upper_b, base);

  //separating components into their respective "slots"
  unsigned int upper_limit = wa + wb;
  unsigned int upper_limit_half = upper_a;

  unsigned int new_c_size = upper_limit;
  unsigned int *new_p_c = expand_array_left(upper_c, upper_bound_a + upper_bound_b, new_c_size);
  
  unsigned int new_mid_size = (upper_limit_half) + mid_size;
  unsigned int *new_p_mid_c = expand_array_left(sub_components, mid_size, new_mid_size);
  //done separating

  unsigned int *add_components = addition(new_p_mid_c, new_p_c, new_mid_size, new_c_size, base);
  unsigned int *full_components = addition(add_components, lower_c, new_c_size, upper_a + upper_b, base);

  return full_components;
}
