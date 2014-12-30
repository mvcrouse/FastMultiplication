#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <gmp.h>
#include <time.h>
#include "multiply.h"
#include "addition.h"
#include "subtraction.h"
#include "arrayExpansion.h"

unsigned int* small_mult(unsigned int *int_a, unsigned int *int_b, unsigned int wa, unsigned int wb);
unsigned int* large_mult(unsigned int *int_a, unsigned int *int_b, unsigned int wa, unsigned int wb);
void super_large_mult(unsigned int *a, unsigned int *b, unsigned int *c, unsigned int wa,
		unsigned int ba, unsigned int wb, unsigned int bb, unsigned int *wc, unsigned int *bc);

/* wa is word length of a, ba is bit length of a */
//as input this takes an array of integers with each integer representing a digit, a size of each array, and a return array
void Multiply(void *a, void *b, void *c, unsigned int wa,
        unsigned int ba, unsigned int wb, unsigned int bb, unsigned int *wc, unsigned int *bc){

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
    ret_c = small_mult(int_a, int_b, wa, wb);
  } else {
    ret_c = large_mult(int_a, int_b, wa, wb);
  }
    for (i = 0; i < wa + wb; i++) {
      int_c[i] = ret_c[i];
    }

}

//naive multiplication algorithm, fast but unsuited for large numbers
unsigned int* small_mult(unsigned int *int_a, unsigned int *int_b, unsigned int wa, unsigned int wb) {

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

	ret_c[i+j] = current & 0xFFFFFFFF;
	
	carry = current >> 32;	
      }
      ret_c[i + j] = carry;
    }
    return ret_c;
}



//implementation of karatsubas algorithm for base 32
unsigned int* large_mult(unsigned int *int_a, unsigned int *int_b, unsigned int wa, unsigned int wb) {

  int i;
  int j;

  unsigned int wc = wa + wb;

  //when size of number to multiply is small enough, default to naive multiplication algorithm
  if (wc <= 156) {
    return small_mult(int_a, int_b, wa, wb);
  }

  unsigned int upper_a = (wa + 1) >> 1;
  unsigned int upper_b = (wb + 1) >> 1;

  //this part handles the multiplication of numbers that are digits removed from each other (i.e. 100 * 3 needs special
  //handling because everything is in array format)
  if (upper_a != upper_b) {
    if (wa >= wb && wb > 64) {
      unsigned int *lower_part = large_mult(int_a, int_b, wb, wb);
      unsigned int *upper_part = large_mult(&int_a[wb], int_b, wa - wb, wb);
      unsigned int *expansion = expand_array_left(upper_part, wa, wa + wb); 
      return addition(expansion, lower_part, wa + wb, wb + wb);
    } else if (wa > 64) {
      unsigned int *lower_part = large_mult(int_a, int_b, wa, wa);
      unsigned int *upper_part = large_mult(int_a, &int_b[wa], wa, wb - wa);
      unsigned int *expansion = expand_array_left(upper_part, wb, wa + wb); 
      return addition(expansion, lower_part, wb + wa, wa + wa);
    } else {
      return small_mult(int_a, int_b, wa, wb);
    }
  }

  unsigned int upper_bound_a = wa - upper_a;
  unsigned int upper_bound_b = wb - upper_b;

  //Change back to large multiplication
  unsigned int *upper_c = large_mult(&int_a[upper_a], &int_b[upper_b], upper_bound_a, upper_bound_b);
  unsigned int *lower_c = large_mult(int_a, int_b, upper_a, upper_b);

  unsigned int *add_a = addition(int_a, &int_a[upper_a], upper_a, upper_bound_a);
  unsigned int *add_b = addition(int_b, &int_b[upper_b], upper_b, upper_bound_b);

  int upper_a_greater = upper_a > upper_bound_a;
  int upper_b_greater = upper_b > upper_bound_b;

  unsigned int mid_size_a = upper_a_greater ? upper_a + 1 : (upper_bound_a) + 1;
  unsigned int mid_size_b = upper_b_greater ? upper_b + 1: (upper_bound_b) + 1;

  unsigned int *mid_c = large_mult(add_a, add_b, mid_size_a, mid_size_b);
  unsigned int mid_size = mid_size_a + mid_size_b;

  unsigned int *sub_components = subtraction(mid_c, upper_c, lower_c, mid_size, upper_bound_a + upper_bound_b, upper_a + upper_b);

  //separating components into their respective "slots"
  unsigned int upper_limit = wa + wb;
  unsigned int upper_limit_half = upper_a;

  unsigned int new_c_size = upper_limit;
  unsigned int *new_p_c = expand_array_left(upper_c, upper_bound_a + upper_bound_b, new_c_size);
  
  unsigned int new_mid_size = (upper_limit_half) + mid_size;
  unsigned int *new_p_mid_c = expand_array_left(sub_components, mid_size, new_mid_size);
  //done separating

  unsigned int *add_components = addition(new_p_mid_c, new_p_c, new_mid_size, new_c_size);
  unsigned int *full_components = addition(add_components, lower_c, new_c_size, upper_a + upper_b);

  return full_components;
}
