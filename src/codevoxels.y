%code requires {

  #include "types.h"
  void yyerror (Ast*, char const *);
  int yylex(void);
}

%define parse.trace

%union {
  char* text;
  Ints ints;
  Strs strs;

  Ints (*op)(Ints, Ints);
  bool (*comp)(Ints, Ints);
  Ast ast;
  Childs childs;
  Pat pat;
}

%parse-param { Ast* tree }
%define api.location.type {Location}

%token let
%token fn
%token line
%token lparen
%token rparen
%token lcurly
%token rcurly
%token lbrack
%token rbrack
%token comma
%token arrow

%token <comp> eq
%token <comp> lt
%token <comp> gt
%type <comp> COMP_OP

%token rest
%token dot 

%token <op> plus
%token <op> dash 
%type <op> SUM_OP

%token <op> star
%token <op> slash
%token <op> mod
%type <op> MUL_OP

%token <text> name
%printer { fprintf(yyo, "##(%s)", $$); } name
%token <ints> value

%type <ast> TOP BLOCK FUNC EXPR
%type <ast> SIMP CALL MUL SUM COMP ITER BIND
%type <childs> CV BLOCK_LIST VAL_LIST
%type <strs> NAME_LIST
%type <pat> PAT

%destructor { Ints_free(&$$); } <ints>
%destructor { Strs_free(&$$); } <strs>

%start S

%%

S : CV[program] { tree->childs = $program; tree->loc = @$; };

CV
  : CV[tail] TOP[head] { Childs_push(&$tail, $head); $$ = $tail; }
  | { $$ = Childs_empty(); }
;

TOP
  : FUNC
  | BIND
;

EXPR
  : SUM
  | COMP
  | ITER
  | BLOCK
  | FUNC
  | BIND
;

COMP_OP
  : lt { $$ = Ints_lt; }
  | gt { $$ = Ints_gt; }
  | eq { $$ = Ints_eq; }
;
MUL_OP
  : star { $$ = Ints_mul; }
  | slash { $$ = Ints_div; }
  | mod { $$ = Ints_mod; }
;
SUM_OP
  : plus { $$ = Ints_add; }
  | dash { $$ = Ints_sub; }
;

VAL_LIST
  : SUM[head] { $$ = Childs_of_single($head); }
  | VAL_LIST[tail] comma SUM[head] { Childs_push(&$tail, $head); $$ = $tail; }
;

SIMP
  : name    { $$ = Ast_var(@$, $name); }
  | value   { $$ = Ast_lit(@$, $value); }
  | lbrack rbrack { $$ = Ast_lit(@$, Ints_empty()); }
  | lbrack VAL_LIST[tail] rbrack { $$ = Ast_val(@$, $tail); }
;

CALL
  : SIMP[head] {}
  | CALL[head] dot name { $$ = Ast_call(@$, $head, $name); }
;

MUL
  : CALL[head] {}
  | MUL[head] MUL_OP[op] CALL[tail] { $$ = Ast_op(@$, $head, $op, $tail); }
;

SUM
  : MUL[head] {}
  | SUM[head] SUM_OP[op] MUL[tail] { $$ = Ast_op(@$, $head, $op, $tail); }
;

COMP
  : lparen SUM[lhs] COMP_OP[op] SUM[rhs] rparen EXPR[body]
    { $$ = Ast_branch(@$, $lhs, $op, $rhs, $body); }
;

ITER
  : lbrack SUM[iter] arrow name rbrack EXPR[body]
    { $$ = Ast_loop(@$, $iter, $name, $body); }
;

BLOCK_LIST
  : BLOCK_LIST[tail] EXPR[head] { Childs_push(&$tail, $head); $$ = $tail; }
  | { $$ = Childs_empty(); }
;
BLOCK
  : lcurly BLOCK_LIST[body] rcurly { $$ = Ast_block(@$, $body); }
;

NAME_LIST
  : name { $$ = Strs_of_single($name); }
  | NAME_LIST[tail] comma name[head] { Strs_push(&$tail, $head); $$ = $tail; }
;

PAT
  : NAME_LIST[list]
    {
      $$ = (Pat) { .names = $list, .rest = false };
    }
  | NAME_LIST[list] comma rest name[last]
    {
      Strs_push(&$list, $last);
      $$ = (Pat) { .names = $list, .rest = true };
    }
  | rest name
    {
      Strs list = Strs_of_single($name);
      $$ = (Pat) { .names = list, .rest = true };
    }
;

FUNC
  : fn name line PAT[pat] line BLOCK[body] { $$ = Ast_fn(@$, $name, $pat, $body); }
  | fn name BLOCK[body]
    {
      Pat pat = (Pat) { .names = Strs_empty(), .rest = false };
      $$ = Ast_fn(@$, $name, pat, $body);
    }
;

BIND
  : let name eq SUM[body] { $$ = Ast_bind(@$, $name, $body); }
  | name eq SUM[body] { $$ = Ast_bind(@$, $name, $body); }
;

%%
