%code requires {
  #include "fat_array.h"

  DEF_ARRAY(int, Ints);

  typedef struct {
    char* name;
    Ints value;
  } Bind;
  DEF_ARRAY(Bind, Bindings);

  int yylex(void);
  void yyerror (char const *);
}

%{

  #include "syn.h"
  IMPL_ARRAY(int, Ints);
  IMPL_ARRAY(Bind, Bindings);
%}

%union {
  char* text;
  Ints value;
}

%token let fn line lparen rparen lcurly rcurly lbrack rbrack comma arrow eq lt gt rest dot plus star dash slash mod
%token <text> name
%token <value> value

%type CV TOP BLOCK FUNC EXPR
%type <value> SIMP CALL MUL SUM COMP ITER BIND
%type <value> SUM_LIST CALL_LIST MUL_LIST ADD_LIST BLOCK_LIST

%start S

%%

S : CV ;

CV
  : CV TOP
  | 
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

COMP_OP : '<' | '>' | '=' ;
MUL_OP  : '*' | '/' | '%' ;
SUM_OP  : '+' | '-' ;

SUM_LIST
  : SUM_LIST[tail] comma SUM[head]
    {
      $$ = Ints_concat($head, $tail);
      Ints_free(&$head);
      Ints_free(&$tail);
    }
  | { $$ = Ints_empty(); }
;

SIMP
  : name { /* TODO get binding from context */ }
  | value
  | lbrack rbrack { $$ = Ints_empty(); }
  | lbrack SUM[head] SUM_LIST[tail] rbrack
    {
      $$ = Ints_concat($head, $tail);
      Ints_free(&$head);
      Ints_free(&$tail);
    }
;

CALL_LIST
  : CALL_LIST dot name {}
  | {}
;

CALL
  : SIMP CALL_LIST {}
;

MUL_LIST
  : MUL_LIST MUL_OP CALL {}
  | {}
;

MUL
  : CALL MUL_LIST {}
;

ADD_LIST
  : ADD_LIST SUM_OP MUL {}
  | {}
;

SUM
  : MUL ADD_LIST {}
;

COMP
  : lparen SUM COMP_OP SUM rparen EXPR {}
;

ITER
  : lbrack SUM arrow name rbrack EXPR {}
;

BLOCK_LIST
  : BLOCK_LIST EXPR {}
  | {}
;
BLOCK
  : lcurly BLOCK_LIST rcurly {}
;

NAME_LIST
  : NAME_LIST comma name {}
  | {}
;

PAT
  : name NAME_LIST {}
  | name NAME_LIST rest name {}
  | rest name {}
;

FUNC
  : fn name line PAT line BLOCK {}
  | fn name BLOCK {}
;

BIND
  : let name eq EXPR {}
  | name eq EXPR {}
;

%%

int main(void) {
  return 0;
}
