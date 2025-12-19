
#include <assert.h>

#include "types.h"
#include "api.h"

IMPL_ARRAY(Ast, Childs)

Ast Ast_lit(Location loc, Ints val) {
  return (Ast){
      .kind = Val, .expr.val = val, .loc = loc, .childs = Childs_empty()};
}
Ast Ast_val(Location loc, Childs vals) {
  return (Ast){
      .kind = Val, .expr.val = Ints_empty(), .loc = loc, .childs = vals};
}
Ast Ast_var(Location loc, char *var) {
  return (Ast){
      .kind = Var, .expr.var = var, .loc = loc, .childs = Childs_empty()};
}
Ast Ast_op(Location loc, Ast lhs, Ints (*eval)(Ints, Ints), Ast rhs) {
  Ast childs[2] = {lhs, rhs};
  return (Ast){
      .kind = Op, .expr.op = {eval}, .loc = loc, .childs = Childs_new(2, childs)};
}
Ast Ast_call(Location loc, Ast args, char *name) {
  return (Ast){.kind = Call,
               .expr.call = {name},
               .loc = loc,
               .childs = Childs_of_single(args)};
}
Ast Ast_branch(Location loc, Ast lhs, bool (*eval)(Ints, Ints), Ast rhs,
               Ast body) {
  Ast childs[3] = {lhs, rhs, body};
  return (Ast){.kind = Branch,
               .expr.branch = {eval},
               .loc = loc,
               .childs = Childs_new(3, childs)};
}
Ast Ast_loop(Location loc, Ast iter, char *name, Ast body) {
  Ast childs[2] = {iter, body};
  return (Ast){.kind = Loop,
               .expr.loop = {name},
               .loc = loc,
               .childs = Childs_new(2, childs)};
}
Ast Ast_bind(Location loc, char *name, Ast args) {
  return (Ast){.kind = Binding,
               .expr.bind = {name},
               .loc = loc,
               .childs = Childs_of_single(args)};
}
Ast Ast_fn(Location loc, char *name, Pat pat, Ast body) {
  return (Ast){.kind = Fn,
               .expr.fn = {.name = name, .pat = pat},
               .loc = loc,
               .childs = Childs_of_single(body)};
}
Ast Ast_block(Location loc, Childs childs) {
  return (Ast){.kind = Block, .loc = loc, .childs = childs};
}

void Ast_push(Ctxs *ctx) {
  // Copy last ctx and push it to ctxs
  Ctx *last = Ctxs_last(ctx);

  Ctx copy = (Ctx){.fns = Defs_new(last->fns.len, last->fns.data),
                   .vars = Binds_new(last->vars.len, last->vars.data)};
  Ctxs_push(ctx, copy);
}

Ctx Ast_pop(Ctxs *ctx) {
  // Pop the last ctx and return it
  return *Ctxs_pop(ctx);
}

void Ast_print(Ast *ast, int depth) {
  printf("  %d-%d, %d-%d",
    ast->loc.first_line,
    ast->loc.last_line,
    ast->loc.first_column,
    ast->loc.first_column
  );
  printf(": %s ", Kind_name(ast->kind));
  if (ast->childs.len == 0) {
    if (ast->kind == Val) {
      printf("= ");
      Ints_print(ast->expr.val, print_int);
    }
    if (ast->kind == Var) {
      printf("%s", ast->expr.var);
    }
  }

  if (ast->kind == Binding) {
    printf("let %s", ast->expr.bind.name);
  }
  if (ast->kind == Fn) {
    printf("fn %s", ast->expr.fn.name);
  }
  if (ast->kind == Call) {
    printf(".%s", ast->expr.call.name);
  }

  printf("\n");
  for (int i = 0; i < ast->childs.len; i++) {
    for (int s = 0; s < depth; s++)
      printf(" ");
    Ast_print(&ast->childs.data[i], depth + 1);
  }
}

Ints Ast_eval(Ctxs *ctx, Ast *ast) {
  printf("%d\n", ast->loc.first_line);
  switch (ast->kind) {
  case Val: {
    if (ast->childs.len == 0) {
      return ast->expr.val;
    }

    Ints val = Ints_empty();
    for (int i = 0; i < ast->childs.len; i++) {
      Ast current = ast->childs.data[i];
      val = Ints_concat(val, Ast_eval(ctx, &current));
    }
    return val;
  }
  case Var:
    return Binds_get(&Ctxs_last(ctx)->vars, ast->expr.var)->value;

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
    // eval child (args)
    Ints args = Ast_eval(ctx, &ast->childs.data[0]);
    char *name = ast->expr.call.name;

    if(name[0] == '@') return dispatchBuiltin(name, args);

    // get function body
    Def *fn = Defs_get(&Ctxs_last(ctx)->fns, name);

    Ast_push(ctx);

      // TODO convert this assert into a lang error msg
      assert(args.len == fn->pat.names.len ||
             (args.len > fn->pat.names.len && fn->pat.rest) ||
             "Argument count is not valid for this call");
      // foreach arg bind pat[i] = arg[i]
      // last gets the rest
      for (int i = 0; i < fn->pat.names.len; i++) {
        char *current = fn->pat.names.data[i];

        if (i != fn->pat.names.len - 1) {
          Binds_set(
              &Ctxs_last(ctx)->vars,
              (Bind){.name = current, .value = Ints_of_single(args.data[i])});
        } else {
          Ints rest = Ints_new(args.len - fn->pat.names.len,
                                             &args.data[i]);
          Binds_set(&Ctxs_last(ctx)->vars,
                    (Bind){.name = current,
                           .value = rest});
        }

      }
      // eval block
      Ints val = Ast_eval(ctx, fn->value);

    Ast_pop(ctx);

    return val;
  }

  // ( lhs op rhs ) expr
  case Branch: {
    assert(ast->childs.len == 3 || "Branch expects 3 childs");
    Ints lhs = Ast_eval(ctx, &ast->childs.data[0]);
    Ints rhs = Ast_eval(ctx, &ast->childs.data[1]);
    Ast *body = &ast->childs.data[2];

    Ints value = Ints_empty();

    if (ast->expr.branch.eval(lhs, rhs)) {
      Ast_push(ctx);
      Ast_eval(ctx, body);
      Ast_pop(ctx);
    }

    return value;
  }

  // [ iter -> name ] expr
  case Loop: {
    assert(ast->childs.len == 2 || "Loop expects 2 childs");

    char *name = ast->childs.data[1].expr.loop.name;

    Ints iter;
    Ints iter_value = Ints_empty();
    int i = 0;
    do {
      // Eval iter
      iter = Ast_eval(ctx, &ast->childs.data[0]);

      if (iter.len == 0)
        return iter_value;

      Ints current = Ints_of_single(iter.data[i]);

      Ast_push(ctx);

      Binds_set(&Ctxs_last(ctx)->vars, (Bind){.name = name, .value = current});
      iter_value = Ast_eval(ctx, &ast->childs.data[2]);

      Ast_pop(ctx);

      i++;
    } while (i < iter.len);
    return iter_value;
  }

  // let name = expr
  case Binding: {
    assert(ast->childs.len == 1 || "Binding expects 1 child");

    Ints val = Ast_eval(ctx, &ast->childs.data[0]);

    Binds_set(&Ctxs_last(ctx)->vars,
              (Bind){.name = ast->expr.bind.name, .value = val});

    return val;
  }

  // fn name | pat | block
  case Fn: {
    assert(ast->childs.len == 1 || "Fn expects 1 child");

    Defs_set(&Ctxs_last(ctx)->fns, (Def){.name = ast->expr.fn.name,
                                         .pat = ast->expr.fn.pat,
                                         .value = &ast->childs.data[0]});
    return Ints_empty();
  }

  // { expr* }
  case Block: {
    Ints last_value = Ints_empty();

    Ast_push(ctx);

    for (int i = 0; i < ast->childs.len; i++) {
      last_value = Ast_eval(ctx, &ast->childs.data[i]);
    }

    Ast_pop(ctx); // TODO Debug site, pop returns the poped context

    return last_value;
  }
  } // switch

  return Ints_empty();
}

