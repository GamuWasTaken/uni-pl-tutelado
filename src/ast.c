

#include "types.h"
#include <assert.h>

IMPL_ARRAY(Ast, Childs)

Ast Ast_lit(Ints val) {
  return (Ast) { .kind = Val, .expr.val = val, .childs = Childs_empty() };
}
Ast Ast_val(Childs vals) {
  return (Ast) { .kind = Val, .expr.val = Ints_empty(), .childs = vals };
}
Ast Ast_var(char* var) {
  return (Ast) { .kind = Var, .expr.var = var, .childs = Childs_empty() };
}
Ast Ast_op(Ast lhs, Ints (*eval)(Ints, Ints), Ast rhs){
  Ast childs[2] = {lhs, rhs};
  return (Ast) { .kind = Op, .expr.op = eval, .childs = Childs_new(2, childs) };
}
Ast Ast_call(Ast args, char* name) {
  return (Ast) { .kind = Call, .expr.call = name, .childs = Childs_of_single(args) };
}
Ast Ast_branch(Ast lhs, bool (*eval)(Ints, Ints), Ast rhs, Ast body) {
  Ast childs[3] = {lhs, rhs, body};
  return (Ast) { .kind = Branch, .expr.branch = eval, .childs = Childs_new(3, childs) };
}
Ast Ast_loop(Ast iter, char* name, Ast body) {
  Ast childs[2] = {iter, body};
  return (Ast) { .kind = Loop, .expr.loop= name, .childs = Childs_new(2, childs) };
}
Ast Ast_bind(char* name, Ast args) {
  return (Ast) { .kind = Binding, .expr.bind = name, .childs = Childs_of_single(args) };
}
Ast Ast_fn(char* name, Pat pat, Ast body) {
  return (Ast) { .kind = Fn, .expr.fn = {
    .name = name,
    .pat = pat
  }, .childs = Childs_of_single(body) };
}
Ast Ast_block(Childs childs) {
  return (Ast) { .kind = Block, .childs = childs };
}

#define TODO() {printf("Todo, %d",__LINE__); return Ints_empty();}

void Ast_push(Ctxs *ctx){
  
}
void Ast_pop(Ctxs *ctx);

Ints Ast_eval(Ctxs ctx, Ast *ast) {
  printf("Evaluating kind: %d\n", ast->kind);

  Binds *vars = &Ctxs_last(&ctx)->vars;
  Defs *fns = &Ctxs_last(&ctx)->fns;
  // TODO push and pop states

  switch (ast->kind) {
  case Val: {
    if(ast->childs.len == 0) return ast->expr.val;

    Ints val = Ints_empty();
    for(int i = 0; i < ast->childs.len; i++) {
      Ast current = ast->childs.data[i];
      val = Ints_concat(val, Ast_eval(ctx, &current));
    }
    return val;
  }
  case Var:
    return Binds_get(vars, ast->expr.var)->value; // TODO what happens if its not found?
    
  // lhs op rhs
  case Op: {
    assert(ast->childs.len == 2 || "Op expects 2 childs");
    Ints lhs = Ast_eval(ctx, &ast->childs.data[0]);
    Ints rhs = Ast_eval(ctx, &ast->childs.data[1]);
    return ast->expr.op.eval(lhs, rhs);
  }

  // args.name
  case Call: {
    assert(ast->childs.len == 1 || "Call expects 1 child");
    Ints args = Ast_eval(ctx, &ast->childs.data[0]);  
    char* name = ast->childs.data[1].expr.call.name;

    TODO() // TODO add mapped pattern to the ctx

    Ast* body = Defs_get(fns, name)->value; // TODO what happens if its not found?
    return Ast_eval(ctx, body);
  }

  // ( lhs op rhs ) expr
  case Branch: {
    assert(ast->childs.len == 3 || "Branch expects 3 childs");
    Ints lhs = Ast_eval(ctx, &ast->childs.data[0]);
    Ints rhs = Ast_eval(ctx, &ast->childs.data[1]);
    Ast* body = &ast->childs.data[2];

    if(ast->expr.branch.eval(lhs, rhs))
      return Ast_eval(ctx, body);
  }

  // [ iter -> name ] expr
  case Loop: {
    assert(ast->childs.len == 2 || "Loop expects 2 childs");

    char* name = ast->childs.data[1].expr.loop.name;

    Ints iter;
    Ints iter_value = Ints_empty();
    int i = 0;
    do {
      // Eval iter
      iter = Ast_eval(ctx, &ast->childs.data[0]);

      if(iter.len == 0) return iter_value;

      Ints current = Ints_of_single(iter.data[i]);
      // Update ctx
      Binds_set(vars, (Bind){ .name = name, .value = current });
      iter_value = Ast_eval(ctx, &ast->childs.data[2]);

      i++;
    } while (i < iter.len);
    return iter_value;
  }

  // let name = expr
  case Binding: {
    assert(ast->childs.len == 1 || "Binding expects 1 child");

    Ints val = Ast_eval(ctx, &ast->childs.data[0]);

    Binds_set(vars, (Bind){.name = ast->expr.bind.name, .value = val});

    return val;
  }

  // fn name | pat | block
  case Fn: {
    assert(ast->childs.len == 1 || "Fn expects 1 child");
    TODO()
  }

  // { expr* }
  case Block: {
    Ints last_value = Ints_empty();
    for(int i = 0; i < ast->childs.len; i++) {
      last_value = Ast_eval(ctx, &ast->childs.data[i]);
    }
    return last_value;
  }
  } // switch

  return Ints_empty();
}
