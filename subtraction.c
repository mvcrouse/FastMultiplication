//because digits can be up to base 32, it is necessary to have a custom subtraction function designed to handle
//integer overflow
unsigned int* subtraction(unsigned int *int_c, unsigned int *int_a, unsigned int *int_b, unsigned int size_c, unsigned int size_a, unsigned int size_b, unsigned int base) {
  unsigned int i;
  unsigned int carry;
  if (size_a <= size_b) {
    for (i = 0; i < size_a; i++) {
      if (int_c[i] >= int_a[i]) {
	int_c[i] = int_c[i] - int_a[i];
      } else {
	carry = base;
	carry -= int_a[i];
	carry += int_c[i] + 1;
	int_c[i] = carry;
	int_c[i+1] -= 1;
      }
      if (int_c[i] >= int_b[i]) {
	int_c[i] = int_c[i] - int_b[i];
      } else {
	carry = base;
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
	carry = base;
	carry -= int_b[i];
	carry += int_c[i] + 1;
	int_c[i] = carry;
	int_c[i+1] -= 1;
      }
    }
  } else {
    return subtraction(int_c, int_b, int_a, size_c, size_b, size_a, base);
  }
  return int_c;
}
