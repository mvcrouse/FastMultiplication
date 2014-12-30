#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <gmp.h>
#include <time.h>
#include "multiply.h"

unsigned int* small_mult(unsigned int *int_a, unsigned int *int_b, unsigned int wa, unsigned int wb);
unsigned int* large_mult(unsigned int *int_a, unsigned int *int_b, unsigned int wa, unsigned int wb);
void super_large_mult(unsigned int *a, unsigned int *b, unsigned int *c, unsigned int wa,
		unsigned int ba, unsigned int wb, unsigned int bb, unsigned int *wc, unsigned int *bc);
unsigned int* addition(unsigned int *int_a, unsigned int *int_b, unsigned int size_a, unsigned int size_b);
unsigned int* subtraction(unsigned int *int_c, unsigned int *int_a, unsigned int *int_b, unsigned int size_c, unsigned int size_a, unsigned int size_b);
int* expand_array_left(unsigned int *array_a, int old_size, int new_size);
int* expand_array_right(unsigned int *array_a, int old_size, int new_size);

/* wa is word length of a, ba is bit length of a */
//as input this takes an array of integers with each integer representing a digit, a size of each array, and a return array
void KaratsubaMultiply(void *a, void *b, void *c, unsigned int wa,
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

//because digits can be up to base 32, it is necessary to have a custom addition function designed to handle
//integer overflow
unsigned int* addition(unsigned int *int_a, unsigned int *int_b, unsigned int size_a, unsigned int size_b) {
  unsigned int i;
  unsigned long long carry = 0;
  unsigned int *int_c;
  if (size_a > size_b) {
    int_c = malloc((size_a + 1) * sizeof(int));
    memset(int_c, 0, (size_a + 1) * sizeof(int));
    for (i = 0; i < size_b; i++) {
      carry = int_a[i];
      carry += int_b[i];
      int_c[i] += carry & 0xFFFFFFFF;
      int_c[i+1] += carry >> 32;
    }
    for (i; i < size_a; i++) {
      carry = int_a[i]; 
      int_c[i] += carry & 0xFFFFFFFF;
      int_c[i+1] += carry >> 32;
    }
  } else {
    int_c = malloc((size_b + 1) * sizeof(int));
    memset(int_c, 0, (size_b + 1) * sizeof(int));
    for (i = 0; i < size_a; i++) {
      carry = int_a[i];
      carry += int_b[i];
      int_c[i] += carry & 0xFFFFFFFF;
      int_c[i+1] += carry >> 32;
    }
    for (i; i < size_b; i++) {
      carry = int_b[i];
      int_c[i] += carry & 0xFFFFFFFF;
      int_c[i+1] += carry >> 32;
    }
  }
  return int_c;
}

//because digits can be up to base 32, it is necessary to have a custom subtraction function designed to handle
//integer overflow
unsigned int* subtraction(unsigned int *int_c, unsigned int *int_a, unsigned int *int_b, unsigned int size_c, unsigned int size_a, unsigned int size_b) {
  unsigned int i;
  unsigned int carry;
  if (size_a <= size_b) {
    for (i = 0; i < size_a; i++) {
      if (int_c[i] >= int_a[i]) {
	int_c[i] = int_c[i] - int_a[i];
      } else {
	carry = 0xFFFFFFFF;
	carry -= int_a[i];
	carry += int_c[i] + 1;
	int_c[i] = carry;
	int_c[i+1] -= 1;
      }
      if (int_c[i] >= int_b[i]) {
	int_c[i] = int_c[i] - int_b[i];
      } else {
	carry = 0xFFFFFFFF;
	carry -= int_b[i];
	carry += int_c[i] + 1;
	int_c[i] = carry;
	int_c[i+1] -= 1;
      }
      
    }
    for (i = i; i < size_b; i++) {
      if (int_c[i] >= int_b[i]) {
	int_c[i] -= int_b[i];
      } else {
	carry = 0xFFFFFFFF;
	carry -= int_b[i];
	carry += int_c[i] + 1;
	int_c[i] = carry;
	int_c[i+1] -= 1;
      }
    }
  } else {
    return subtraction(int_c, int_b, int_a, size_c, size_b, size_a);
  }
  return int_c;
}

//this function serves to move over digits, similarly to multplying by the base, except because the number is
//in array format you have to handle digit movement manually
int* expand_array_left(unsigned int *array_a, int old_size, int new_size) {
  unsigned int *ret_a = malloc(new_size * sizeof(int));
  memset(ret_a, 0, new_size * sizeof(int));
  int i;
  int j = new_size - 1;
  
  for (i = old_size - 1; i >= 0; i--) {
    ret_a[j] = array_a[i];
    j--;
  }
  return ret_a;
}

//this function serves to move over digits, similarly to dividing by the base, except because the number is
//in array format you have to handle digit movement manually
int* expand_array_right(unsigned int *array_a, int old_size, int new_size) {
  unsigned int *ret_a = calloc(new_size, sizeof(int));
  int i;
  for (i = 0; i < old_size; i++) {
    ret_a[i] = array_a[i];
  }
  return ret_a;
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
