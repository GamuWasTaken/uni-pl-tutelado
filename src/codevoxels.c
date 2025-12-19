#ifndef CODEVOXELS
#define CODEVOXELS

#include "types.h"
#include "syn.h"

Ints dispatchBuiltin(char* name, Ints data) {

  printf("Builtin %s ", name);
  Ints_print(data, print_int); printf("\n");

  return data;
}

int main(void) {
  // yydebug = 1;
  //  status = 0: ok
  //  status = 1: err
  //  status = 2: oom
  Ast base = Ast_block((Location) {
    .first_column = 1,
    .first_line = 1,
    .last_column = 1,
    .last_line = 1
  }, Childs_empty());

  int status = yyparse(&base);
  Ctxs ctx = Ctxs_default();

  // Ast_print(&base, 0);
  Ast_eval(&ctx, &base);


  // Location l = (Location) {0};
  // char* from[2] = {"a", "b"};

  // Ast base = Ast_block(l, Childs_empty());

  // Ast body = Ast_var(l, "a");
  // Ast fn = Ast_fn(
  //   l,
  //   "test",
  //   (Pat){.names = Strs_new(2, from),.rest = false},
  //   body
  // );

  // int ints[2] = {3,7};
  // Ast call = Ast_call(l, Ast_lit(l, Ints_new(2, ints)), "test");
  // Ast let = Ast_bind(l, "a", call);

  // Childs_push(&base.childs, fn);
  // Childs_push(&base.childs, let);
  // //
  // // fn test | a, b | { a }
  // // let a = 8.test

  // Ast_eval(&ctx, &base);
  
  return 0;
}

#endif // CODEVOXELS
