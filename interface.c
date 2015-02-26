/*
This file is where you should call your multiplications
*/
#include <stdio.h>
#include "multiply.h"

int* strip_digits(char input_str[], int *size);

int main() {

  char input_str[] = "999";
  int* digit_size;
  int* digits = strip_digits(input_str, &digit_size);

  int i;
  printf("Input:\n\n%s\n-----------------------------\n", input_str);

  printf("Output:\n\n");
  for (i = 0; i < digit_size; i++) {
    printf("%d", digits[i]);
  }
  printf("\n-----------------------------\n", digits);
  return 0;
}


int* strip_digits(char input_str[], int *size) {
  //function to strip the digits off of input string

  //i will hold the index of the string and curr_digit will
  //be the number at the index of the string
  int i, curr_digit;

  //the digit is the 32 bit size that we need for the
  //multiplication framework
  unsigned int digit = 0, curr_val = 0;

  //bound on string size
  int len = strlen(input_str);

  //number of 32 bit words the string takes up and current
  //digit in return array value
  int words = 0, ret_index = 0;
 
  //first pass we get the size of the integer
  for (i = 0; i < len; i++) {
    curr_digit = input_str[i] - '0';
    curr_val = 0xFFFFFFFF - digit;
    if (curr_val < curr_digit) {
      digit = 1;
      words += 1;
    } else {
      digit += curr_digit;
    }
  }

  //now we allocate the array
  unsigned int *ret_val = malloc((words + 1) * sizeof(int));
  memset(ret_val, 0, (words + 1) * sizeof(int));

  //second pass we fill in the integer values
  digit = 0;
  for (i = 0; i < len; i++) {
    curr_digit = input_str[i] - '0';
    curr_val = 0xFFFFFFFF - digit;
    if (curr_val < curr_digit) {
      ret_val[ret_index] = digit;
      ret_index += 1;
      digit = 1;
    } else {
      digit += curr_digit;
    }
  }

  if (0xFFFFFFFF - digit > curr_digit) {
    ret_val[ret_index] = digit;
  }

  *size = words + 1;
  return ret_val;
}

char* recreate_string(unsigned int *input, int words) {
  int i, j;
  unsigned int current;
  char* ret_val;

  for (i = 0; i < words; i++) {
    //this is a VERY inefficient method of getting string
    //I know a faster one but I want to get the basics
    //down first
    char substring[] = "";
    current = input[i];
  }

  return ret_val;
}
