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
