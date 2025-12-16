%code requires {
  #include "codevoxels.c"
}

%union {
  char* text;
  Ints value;
  Strs pat;
}

%token let fn line lparen rparen lcurly rcurly lbrack rbrack comma arrow eq lt gt rest dot plus star dash div mod
%token <text> id
%token <value> int

%type <value> TOP EXPR SIMP CALL MUL SUM COMP ITER BLOCK FUNC BIND

%start REPLI

%%

REPLI
  : REPLI TOP {}
  | {}
;

TOP
  : FUNC {}
  | BIND {}
;

EXPR
  : SUM {}
  | COMP {}
  | ITER {}
  | BLOCK {}
  | FUNC {}
  | BIND {}
;

COMP_OP : lt | gt | eq ;
MUL_OP  : star | div | mod ;
SUM_OP  : plus | dash ;

SUM_LIST
  : SUM_LIST comma SUM
    {
      Ints val = Ints_concat($1, $2);
      Ints_free($1);
      Ints_free($2);
      return val;
    }
  | { return Ints_empty(); }
;

SIMP
  : id {}
  | int
  | lbrack rbrack { Ints_empty(); }
  | lbrack SUM SUM_LIST rbrack {}
;

CALL_LIST
  : CALL_LIST dot id {}
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
  : lbrack SUM arrow id rbrack EXPR {}
;

BLOCK_LIST
  : BLOCK_LIST EXPR {}
  | {}
;
BLOCK
  : lcurly BLOCK_LIST rcurly {}
;

id_LIST
  : id_LIST comma id {}
  | {}
;

PAT
  : id id_LIST {}
  | id id_LIST rest id {}
  | rest id {}
;

FUNC
  : fn id line PAT line BLOCK {}
  | fn id BLOCK {}
;

BIND
  : let id eq EXPR {}
  | id eq EXPR {}
;
