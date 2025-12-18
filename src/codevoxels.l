
%{
#include <stdio.h>
#include <stdlib.h>
#include "syn.h"

%}

%option yylineno

%%

"="   { return eq     ;}
"<"   { return lt     ;}
">"   { return gt     ;}
"let" { return let    ;}
"fn"  { return fn     ;}

"|"   { return line   ;}
"("   { return lparen ;}
")"   { return rparen ;}
"{"   { return lcurly ;}
"}"   { return rcurly ;}
"["   { return lbrack ;}
"]"   { return rbrack ;}
","   { return comma  ;}

"->"  { return arrow  ;}
".."  { return rest   ;}


"."   { return dot    ;}

"+"   { return plus   ;}
"*"   { return star   ;}
"-"   { return dash   ;}
"/"   { return slash  ;}
"%"   { return mod    ;}

[@a-zA-Z][a-zA-Z_0-9]* { yylval.text = strdup(yytext); return name; }
[0-9]+ { yylval.ints = Ints_of_single(atoi(yytext)); return value; }

"//".*"\n"   {;}
[ \n\t\r\f]+ {;}

. {;}

%%
