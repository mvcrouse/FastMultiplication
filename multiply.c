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

unsigned int* small_mult(unsigned int *int_a, unsigned int *int_b, unsigned int wa, unsigned int wb);
unsigned int* medium_mult(unsigned int *int_a, unsigned int *int_b, unsigned int wa, unsigned int wb);
unsigned int* large_mult(unsigned int *int_a, unsigned int *int_b, unsigned int wa, unsigned int wb);

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
unsigned int* medium_mult(unsigned int *int_a, unsigned int *int_b, unsigned int wa, unsigned int wb) {

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

unsigned int* large_mult(unsigned int *int_a, unsigned int *int_b, unsigned int wa, unsigned int wb) {
  //this function is still a work in progress so it is not yet hooked up to anything

  int i;
  int j;

  unsigned int wc = wa + wb;

  //when size of number to multiply is small enough, default to naive multiplication algorithm
  if (wc <= 156) {
    return small_mult(int_a, int_b, wa, wb);
  }

  //for Toom 3-Way we need to split each number into three distinct numbers, an upper, middle, and lower part
  unsigned int upper_a = wa - wa / 3;
  unsigned int upper_b = wb - wb / 3;
  
  unsigned int middle_a = wa / 3;
  unsigned int middle_b = wb / 3;

  unsigned int lower_a = 0;
  unsigned int lower_b = 0;

  //first let's define some helpful variables that determine the size of each limb
  unsigned int largest_a_1;
  unsigned int largest_a_2;
  unsigned int largest_b_1;
  unsigned int largest_b_2;

  int initial;
  int secondary;

  unsigned int *upper_part;
  unsigned int *middle_part;
  unsigned int *summation;
  unsigned int *difference;

  unsigned int *part_0_ab;
  unsigned int part_0_ab_size;

  unsigned int *part_1_a;
  unsigned int *part_1_b;
  unsigned int *part_1_ab;
  unsigned int part_1_ab_size;

  unsigned int *part_2_a;
  unsigned int *part_2_b;
  unsigned int *part_2_ab;
  unsigned int part_2_ab_size;

  unsigned int *part_3_a;
  unsigned int *part_3_b;
  unsigned int *part_3_ab;
  unsigned int part_3_ab_size;

  unsigned int *part_4_ab;
  unsigned int part_4_ab_size;

  //now we go to split point 0, which is 0
  part_0_ab = medium_mult(int_a, int_b, middle_a, middle_b);
  part_0_ab_size = middle_a + middle_b;

  //now we go to split point 1, which is 1
  initial = 1;
  secondary = 1;

  largest_a_1 = wa - upper_a + 1 > upper_a - middle_a + 1 ? wa - upper_a + 2 : upper_a - middle_a + 2;
  //upper_part = mult_by_constant(&int_a[upper_a], initial, wa - upper_a);
  //middle_part = mult_by_constant(&int_a[middle_a], secondary, upper_a - middle_a);
  summation = addition(upper_part, middle_part, wa - upper_a + 1, upper_a - middle_a + 1);
  part_1_a = addition(summation, int_a, largest_a_1, middle_a);
  largest_a_2 = largest_a_1 > middle_a ? largest_a_1 + 1 : middle_a + 1;

  largest_b_1 = wb - upper_b + 1 > upper_b - middle_b + 1 ? wb - upper_b + 2 : upper_b - middle_b + 2;
  //  upper_part = mult_by_constant(&int_b[upper_b], initial, wb - upper_b);
  //middle_part = mult_by_constant(&int_b[middle_b], secondary, upper_b - middle_b);
  summation = addition(upper_part, middle_part, wb - upper_b + 1, upper_b - middle_b + 1);
  part_1_b = addition(summation, int_b, largest_b_1, middle_b);
  largest_b_2 = largest_b_1 > middle_b ? largest_b_1 + 1 : middle_b + 1;

  part_1_ab = medium_mult(part_1_a, part_1_b, largest_a_2, largest_b_2);
  part_1_ab_size = largest_a_2 + largest_b_2;

  //now we go to split point 2, which is -1
  initial = 1;
  secondary = 1;

  largest_a_1 = wa - upper_a + 1 > middle_a ? wa - upper_a + 2 : middle_a + 1;
  //upper_part = mult_by_constant(&int_a[upper_a], initial, wa - upper_a);
  //middle_part = mult_by_constant(&int_a[middle_a], secondary, upper_a - middle_a);
  summation = addition(upper_part, int_a, wa - upper_a + 1, middle_a);
  //difference = subtraction(summation, middle_part, largest_a_1, upper_a - middle_a + 1);
  part_2_a = difference;
  largest_a_2 = largest_a_1 > upper_a - middle_a + 1 ? largest_a_1 + 1 : upper_a - middle_a + 2;

  largest_b_1 = wb - upper_b + 1 > middle_b ? wb - upper_b + 2 : middle_b + 1;
  //upper_part = mult_by_constant(&int_b[upper_b], initial, wb - upper_b);
  //middle_part = mult_by_constant(&int_b[middle_b], secondary, upper_b - middle_b);
  summation = addition(upper_part, int_b, wb - upper_b + 1, middle_b);
  //difference = subtraction(summation, middle_part, largest_b_1, upper_b - middle_b + 1);
  part_2_b = difference;
  largest_b_2 = largest_b_1 > upper_b - middle_b + 1 ? largest_b_1 + 1 : upper_b - middle_b + 2;

  part_2_ab = medium_mult(part_2_a, part_2_b, largest_a_2, largest_b_2);
  part_2_ab_size = largest_a_2 + largest_b_2;

  //now the goal is to generate the product of the three limbs with each of the split points
  //we begin with split point 1, which is -2
  initial = 4;
  secondary = 2;

  largest_a_1 = wa - upper_a + 1 > middle_a ? wa - upper_a + 2 : middle_a + 1;  
  //upper_part = mult_by_constant(&int_a[upper_a], initial, wa - upper_a);
  //middle_part = mult_by_constant(&int_a[middle_a], secondary, upper_a - middle_a);
  summation = addition(upper_part, int_a, wa - upper_a + 1, middle_a);
  //difference = subtraction(summation, middle_part, largest_a_1, upper_a - middle_a + 1);
  part_3_a = difference;
  largest_a_2 = largest_a_1 > upper_a - middle_a + 1 ? largest_a_1 : upper_a - middle_a + 1;

  largest_b_1 = wb - upper_b + 1 > middle_b ? wb - upper_b + 2 : middle_b + 1;
  //upper_part = mult_by_constant(&int_b[upper_b], initial, wb - upper_b);
  //middle_part = mult_by_constant(&int_b[middle_b], secondary, upper_b - middle_b);
  summation = addition(upper_part, int_b, wb - upper_b + 1, middle_b);
  //difference = subtraction(summation, middle_part, largest_b_1, upper_b - middle_b + 1);
  part_3_b = difference;
  largest_b_2 = largest_b_1 > upper_b - middle_b + 1 ? largest_b_1 + 1: upper_b - middle_b + 2;

  part_3_ab = medium_mult(part_3_a, part_3_b, largest_a_2, largest_b_2);
  part_3_ab_size = largest_a_2 + largest_b_2;

  //now we go to split point 5, which is the largest 2 limbs
  part_4_ab = medium_mult(&int_a[upper_a], &int_b[upper_b], wa - upper_a, wb - upper_b);
  part_4_ab_size = wa - upper_a + wb - upper_b;

  //now that we have found all 5 points, we have 5 linear equations to solve to get the final points p1, p2, p3, p4, p5
  unsigned int *p0 = part_0_ab;
  unsigned int p0_size = part_0_ab_size;

  unsigned int *p4 = part_4_ab;
  unsigned int p4_size = part_4_ab_size;
  
  unsigned int *p3;// = subtraction(part_3_ab, part_1_ab, part_3_ab_size, part_1_ab_size);
  unsigned int p3_size = part_3_ab_size > part_1_ab_size ? part_3_ab_size : part_1_ab_size;
  p3 = div_3(p3, p3_size);

  unsigned int *p1;// = subtraction(part_1_ab, part_2_ab, part_1_ab_size, part_2_ab_size);
  unsigned int p1_size = part_1_ab_size > part_2_ab_size ? part_1_ab_size : part_2_ab_size;
  p1 = div_2(p1, p1_size);

  unsigned int *p2;// = subtraction(part_2_ab, p0, part_2_ab_size, p0_size);
  unsigned int p2_size = part_2_ab_size > p0_size ? part_2_ab_size : p0_size;

  unsigned int *psub;

  //  p3 = subtraction(p2, p3, p2_size, p3_size);
  p3_size = p2_size > p3_size ? p2_size : p3_size;
  p3 = div_2(p3, p3_size);
  //psub = mult_by_constant(p4, 2, p4_size);
  p3 = addition(p3, psub, p3_size, p4_size + 1);
  p3_size = p3_size > p4_size + 1 ? p3_size : p4_size + 1;

  p2 = addition(p2, p1, p2_size, p1_size);
  p2_size = p2_size > p1_size ? p2_size : p1_size;
  //  p2 = subtraction(p2, p4, p2_size, p4_size);
  p2_size = p2_size > p4_size ? p2_size : p4_size;

  //  p1 = subtraction(p1, p3, p1_size, p3_size);
  p1_size = p1_size > p3_size ? p1_size : p3_size;

  //now we simply recombine all 5 points together
  p4 = expand_array_left(p4, p4_size, wc);

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

  unsigned int *sub_components;// = subtraction(mid_c, upper_c, lower_c, mid_size, upper_bound_a + upper_bound_b, upper_a + upper_b);

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
