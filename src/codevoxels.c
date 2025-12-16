#ifndef CODEVOXELS
#define CODEVOXELS

#include "fat_array.h"
#include <stdio.h>

IMPL_ARRAY(int, Ints);
IMPL_ARRAY(char*, Strs);


typedef struct {
  char* name;
  Ints value;
} Binding;

IMPL_ARRAY(Binding, Bindings);

void print_int(int a) { printf("%d", a); }

int main(void) {

  Ints a = Ints_empty();
  Ints b = Ints_of_single(1);
  Ints c = Ints_of_single(2);
  Ints d = Ints_of_single(3);
  Ints e = Ints_of_single(4);
  Ints f = Ints_of_single(5);

  Ints ab = Ints_concat(a,b);
  Ints_free(&a);
  Ints_free(&b);

  Ints_print(a, print_int);
  Ints_print(b, print_int);
  Ints_print(c, print_int);
  Ints_print(d, print_int);
  Ints_print(e, print_int);
  Ints_print(f, print_int);
  
  return 0;
}

#endif // CODEVOXELS
