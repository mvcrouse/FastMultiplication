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
