
#include "api.h"
#include "string.h"
#include "types.h"
#include "lex.h"
#include "syn.h"


// extern void forward__();
Ints __forward() {
   // forward__();
   return Ints_empty();
 }

// extern void turnRight__();
Ints __turnRight() {
  // turnRight__();
  return Ints_empty();
}

// extern void turnLeft__();
Ints __turnLeft() {
  // turnLeft__();
  return Ints_empty();
}

// extern void up__();
Ints __up() {
  // up__();
  return Ints_empty();
}

// extern void down__();
Ints __down() {
  // down__();
  return Ints_empty();
}

// extern void setBlock__(int, int, int, int);
Ints __setBlock(Ints col) {
  assert(col.len == 4 || "@setBlock expects 4 arguments");
  // setBlock__(col.data[0], col.data[1], col.data[2], col.data[3]);
  return Ints_empty();
}

// extern void readBlock__();
// Ints __readBlock() {readBlock__(); return Ints_empty(); }

Ints dispatchBuiltin(char* name, Ints data) {

  printf("Builtin");
  
  if(strcmp(name, "@forward")   == 0) return __forward();
  if(strcmp(name, "@turnRight") == 0) return __turnRight();
  if(strcmp(name, "@turnLeft")  == 0) return __turnLeft();
  if(strcmp(name, "@up")        == 0) return __up();
  if(strcmp(name, "@down")      == 0) return __down();
  if(strcmp(name, "@setBlock")  == 0) return __setBlock(data);
  // if(strcmp(name, "@readBlock") == 0) return __readBlock();

  return Ints_empty();
}

void __parse(char* code) {

  printf("Reading this: ");
  printf("%s\n", code);

  Ast base = Ast_block((Location) {
    .first_column = 1,
    .first_line = 1,
    .last_column = 1,
    .last_line = 1
  }, Childs_empty());

  struct yy_buffer_state* buff = yy_scan_string(code);
  int status = yyparse(&base);
  
  Ctxs ctx = Ctxs_default();
  Ast_print(&base, 0);
  Ast_eval(&ctx, &base);

  yy_delete_buffer(buff);

}
