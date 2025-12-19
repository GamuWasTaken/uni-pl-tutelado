#ifndef TYPES_HEADER
#define TYPES_HEADER

#include <stdbool.h>
#include <assert.h>

#include "fat_array.h"

#define IMPL_INTS_OP(_name, _body)           \
int _##_name(int a, int b) { return _body; } \
Ints Ints_##_name(Ints a, Ints b) {          \
  return Ints_op(a, b, _##_name);            \
}                                            \


#define IMPL_INTS_COMP(_name, _body)          \
bool _##_name(int a, int b) { return _body; } \
bool Ints_##_name(Ints a, Ints b) {           \
  return Ints_comp(a, b, _##_name);           \
}                                             \

// MAYBE Handle errors                                        
#define DEF_ARRAY_GET(_name, _in, _out) \
_in* _name##_get(_name *arr, char* name)
#define IMPL_ARRAY_GET(_name, _in, _out)                     \
DEF_ARRAY_GET(_name, _in, _out) {                            \
  for(int i = 0; i < arr->len; i++) {                        \
    _in current = arr->data[i];                              \
    if(strcmp(current.name, name) == 0)                      \
      return &arr->data[i];                                  \
  }                                                          \
  assert(false || "Assoc array doesnt have requested name"); \
  return NULL;                                               \
}                                                            \

#define DEF_ARRAY_HAS(_name, _in, _out) \
bool _name##_has(_name *arr, char* name)
#define IMPL_ARRAY_HAS(_name, _in, _out)             \
DEF_ARRAY_HAS(_name, _in, _out) {                    \
  for(int i = 0; i < arr->len; i++) {                \
    _in current = arr->data[i];                      \
    if(strcmp(current.name, name) == 0) return true; \
  }                                                  \
  return false;                                      \
}                                                    \

#define DEF_ARRAY_SET(_name, _in, _out) \
_name* _name##_set(_name *arr, _in elem)
#define IMPL_ARRAY_SET(_name, _in, _out) \
DEF_ARRAY_SET(_name, _in, _out) {           \
  if(_name##_has(arr, elem.name)) {         \
    _in* val = _name##_get(arr, elem.name); \
    val->value = elem.value;                \
  } else {                                  \
    _name##_push(arr, elem);                \
  }                                         \
                                            \
  return arr;                               \
}                                           \


/* Ints */
DEF_ARRAY(int, Ints);
void Ints_free(Ints *arr);

Ints Ints_op(Ints a, Ints b, int (*op)(int, int));
Ints Ints_add(Ints a, Ints b);
Ints Ints_sub(Ints a, Ints b);
Ints Ints_mul(Ints a, Ints b);
Ints Ints_div(Ints a, Ints b);
Ints Ints_mod(Ints a, Ints b);

bool Ints_comp(Ints a, Ints b, bool (*op)(int, int));
bool Ints_gt(Ints a, Ints b);
bool Ints_lt(Ints a, Ints b);
bool Ints_eq(Ints a, Ints b);
/* Strs */
DEF_ARRAY(char*, Strs);
void Strs_free(Strs *arr);

/* Binds */
typedef struct {
  char* name;
  Ints value;
} Bind;
DEF_ARRAY(Bind, Binds);
void Binds_free(Binds *arr);
DEF_ARRAY_GET(Binds, Bind, Ints);
DEF_ARRAY_HAS(Binds, Bind, Ints);
DEF_ARRAY_SET(Binds, Bind, Ints);

/* Ast */
typedef struct Ast Ast;
DEF_ARRAY(Ast, Childs);

  /* Pat */
  typedef struct {
    Strs names;
    bool rest;
  } Pat;

  /* Defs */
  typedef struct {
    char* name;
    Pat pat;
    Ast* value;
  } Def;
  DEF_ARRAY(Def, Defs);
  void Defs_free(Defs *arr);
  DEF_ARRAY_GET(Defs, Def, Ast*);
  DEF_ARRAY_HAS(Defs, Def, Ast*);
  DEF_ARRAY_SET(Defs, Def, Ast*);

typedef enum { Val, Var, Op, Call, Branch, Loop, Binding, Fn, Block } Kind;
char* Kind_name(Kind val);
typedef union {
  Ints val;
  char* var;

  struct {
    Ints (*eval)(Ints, Ints);
  } op;

  struct {
    char* name;
  } call;

  struct {
    bool (*eval)(Ints, Ints);
  } branch;

  struct {
    char* name;
  } loop;

  struct {
    char* name;
  } bind;

  struct {
    char* name;
    Pat pat;
  } fn;

  struct {
  } block;

} Expr;

typedef struct {
  int first_line;
  int first_column;
  int last_line;
  int last_column;
} Location;

struct Ast {
  Kind kind;
  Expr expr;
  Childs childs;
  Location loc;
};

Ast Ast_lit(Location loc, Ints val);
Ast Ast_val(Location loc, Childs val);
Ast Ast_var(Location loc, char* var);
Ast Ast_op(Location loc, Ast lhs, Ints (*eval)(Ints, Ints), Ast rhs);
Ast Ast_call(Location loc, Ast args, char* name);
Ast Ast_branch(Location loc, Ast lhs, bool (*eval)(Ints, Ints), Ast rhs, Ast body);
Ast Ast_loop(Location loc, Ast iter, char* name, Ast body);
Ast Ast_bind(Location loc, char* name, Ast val);
Ast Ast_fn(Location loc, char* name, Pat pat, Ast body);
Ast Ast_block(Location loc, Childs childs);

  /* Ctx */
  typedef struct {
    Binds vars;
    Defs fns;  
  } Ctx;
  DEF_ARRAY(Ctx, Ctxs);
  Ctx* Ctxs_last(Ctxs* arr);
  Ctxs Ctxs_default();

Ints Ast_eval(Ctxs *ctx, Ast *ast);


void print_int(int);
void print_str(char*);
void print_bind(Bind);
void print_def(Def);
void print_ctx(Ctx);

void Ast_print(Ast *ast, int depth);

void yyerror (Ast* ast, char const * msg);
int yylex(void);

#endif //TYPES_HEADER
