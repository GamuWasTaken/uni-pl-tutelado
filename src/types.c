
#include "types.h"

/* Ints */
IMPL_ARRAY(int, Ints);
void Ints_free(Ints *arr) { Ints_simple_free(arr); }

Ints Ints_op(Ints a, Ints b, int (*op)(int, int)){
  Ints min, max; 

  if(a.len > b.len) { min = b; max = a; }
  else              { min = a; max = b; }

  // Spread op
  if(min.len == 1) {
    Ints res = Ints_new(max.len, max.data);
    for(int i = 0; i < res.len; i++) {
      res.data[i] = op(max.data[i], min.data[0]); 
    }
    return res;
  } 

  // op until first end
  Ints res = Ints_new(max.len, max.data);
  for(int i = 0; i < res.len; i++) {
    if(min.len > i)
      res.data[i] = op(max.data[i], min.data[i]); 
    else
      res.data[i] = max.data[i]; 
  }
  return res;
}

IMPL_INTS_OP(add, a+b);
IMPL_INTS_OP(sub, a-b);
IMPL_INTS_OP(mul, a*b);
IMPL_INTS_OP(div, a/b);
IMPL_INTS_OP(mod, a%b);

// TODO test this 
bool Ints_comp(Ints a, Ints b, bool (*op)(int, int)){
  Ints min, max; 

  if(a.len > b.len) { min = b; max = a; }
  else              { min = a; max = b; }

  // Spread op
  if(min.len == 1) {
    bool res = true;
    for(int i = 0; i < max.len; i++) {
      res = res && op(max.data[i], min.data[0]); 
    }
    return res;
  } 

  // op until first end
  bool res = true;
  for(int i = 0; i < max.len; i++) {
    if(min.len > i)
      res = res && op(max.data[i], min.data[i]); 
  }
  return res;
  
}

IMPL_INTS_COMP(gt, a>b);
IMPL_INTS_COMP(lt, a<b);
IMPL_INTS_COMP(eq, (a=b));

/* Strs */
IMPL_ARRAY(char*, Strs);
void Strs_free(Strs *arr) {
// TODO maybe impl complex free (free the char* too)
  // for(int i = 0; i < arr->len; i++){
  //   char* current = arr->data[i];
  //   if(current == NULL) continue;
  //   free(current);
  // }
  Strs_simple_free(arr);
}

char* Kind_name(Kind val) {
  static char* names[] = {
    "Val", "Var", "Op", "Call", "Branch", "Loop", "Binding", "Fn", "Block"
  };

  return names[val];
};

/* Binds */
IMPL_ARRAY(Bind, Binds);
void Binds_free(Binds *arr) {
  Binds_simple_free(arr);
}
IMPL_ARRAY_GET(Binds, Bind, Ints);
IMPL_ARRAY_HAS(Binds, Bind, Ints);
IMPL_ARRAY_SET(Binds, Bind, Ints);

/* Defs */
IMPL_ARRAY(Def, Defs);
void Defs_free(Defs *arr) {
  Defs_simple_free(arr);
}
IMPL_ARRAY_GET(Defs, Def, Childs);
IMPL_ARRAY_HAS(Defs, Def, Childs);
IMPL_ARRAY_SET(Defs, Def, Childs);

/* Ctxs */
IMPL_ARRAY(Ctx, Ctxs);
Ctx* Ctxs_last(Ctxs* arr){
  assert(arr->len > 0 || "Cannot get last of empty");
  return &arr->data[arr->len - 1];
}

Ctxs Ctxs_default() {
  return Ctxs_of_single((Ctx) {
    .fns = Defs_empty(),
    .vars = Binds_empty()
  });
}

void print_int(int i) {printf("%d", i);}
void print_bind(Bind b) { printf("{ %s = ", b.name); Ints_print(b.value, print_int); printf(" }"); }
void print_str(char* c) {printf("%s", c);}
void print_def(Def);
void print_ctx(Ctx);

void yyerror (Ast* ast, char const * msg) {
  printf("ploto");
}
