#ifndef CODEVOXELS
#define CODEVOXELS

#include "types.h"

void print_int(int i) {printf("%d", i);}
void print_bind(Bind b) { printf("{ %s = ", b.name); Ints_print(b.value, print_int); printf(" }"); }

int main(void) {

  int from[3] = {1,2,3};

  Ints a = Ints_new(3, from);
  Ints b = Ints_empty();
  Ints c = Ints_of_single(8);

  printf("\n");
  Ints_print(c, print_int);
  printf("\n");
  Ints_print(*Ints_push(&c, 4), print_int);
  printf("\n");
  print_int(*Ints_pop(&c));
  printf("\n");


 //
 // TODO polish pat (Strs + bool?)
 // TODO Finish lang impl
 // TODO test it
 // TODO generate the ast with bison 
 // TODO test it
 // TODO compile to wasm
 // TODO test it
 // :)
  
  
  return 0;
}

#endif // CODEVOXELS
