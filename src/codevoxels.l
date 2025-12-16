
%{
#include <stdio.h>
#include <stdlib.h>
#include "codevoxels.syn.h"
%}

%option yylineno

%%

"let" {return LET   ;}
"fn"  {return FN    ;}

"|"   {return LINE  ;}
"("   {return LPAREN;}
")"   {return RPAREN;}
"{"   {return LCURLY;}
"}"   {return RCURLY;}
"["   {return LBRACK;}
"]"   {return RBRACK;}
","   {return COMMA ;}

"->"  {return ARROW ;}
";"   {return END   ;}
":"   {return DEF   ;}
"="   {return EQ    ;}
"<"   {return LT    ;}
">"   {return GT    ;}
".."  {return REST  ;}
"."   {return DOT   ;}

"+"   {return PLUS  ;}
"*"   {return STAR  ;}
"-"   {return DASH  ;}
"/"   {return DIV   ;}
"%"   {return MOD   ;}

[@a-zA-Z][a-zA-Z_0-9] { yyval.text = strdup(yytext); return ID; }
[0-9]+ { yyval.value = Ints_of_single(atoi(yytext)); return INT; }

"//".*"\n"   {;}
[ \n\t\r\f]+ {;}
