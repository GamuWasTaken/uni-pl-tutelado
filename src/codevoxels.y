%code requires {
  #include "codevoxels.c"
}

%union {
  char* text;
  Ints value;
  Strs pat;
}

%token LET FN LINE LPAREN RPAREN LCURLY RCURLY LBRACK RBRACK COMMA ARROW EQ LT GT REST DOT PLUS STAR DASH DIV MOD
%token <text> ID
%token <value> INT

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

COMP_OP : LT | GT | EQ ;
MUL_OP  : STAR | DIV ;
SUM_OP  : PLUS | DASH ;

SUM_LIST
  : SUM_LIST COMMA SUM {}
  | {}
;

SIMP
  : ID {}
  | INT {}
  | LBRACK RBRACK {}
  | LBRACK SUM SUM_LIST RBRACK {}
;

CALL_LIST
  : CALL_LIST DOT ID {}
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
  : LPAREN SUM COMP_OP SUM RPAREN EXPR {}
;

ITER
  : LBRACK SUM ARROW ID RBRACK EXPR {}
;

BLOCK_LIST
  : BLOCK_LIST EXPR {}
  | {}
;
BLOCK
  : LCURLY BLOCK_LIST RCURLY {}
;

ID_LIST
  : ID_LIST COMMA ID {}
  | {}
;

PAT
  : ID ID_LIST {}
  | ID ID_LIST REST ID {}
  | REST ID {}
;

FUNC
  : FN ID LINE PAT LINE BLOCK {}
  | FN ID BLOCK {}
;

BIND
  : LET ID EQ EXPR {}
  | ID EQ EXPR {}
;
