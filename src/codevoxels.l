
%{
#include <stdio.h>
#include <stdlib.h>
#include "syn.h"

%}

%option yylineno

%%

"="   {;}
"<"   {;}
">"   {;}
"let" {;}
"fn"  {;}

"|"   {;}
"("   {;}
")"   {;}
"{"   {;}
"}"   {;}
"["   {;}
"]"   {;}
","   {;}

"->"  {;}
".."  {;}


"."   {;}

"+"   {;}
"*"   {;}
"-"   {;}
"/"   {;}
"%"   {;}

[@a-zA-Z][a-zA-Z_0-9] { yylval.text = strdup(yytext); return name; }
[0-9]+ { yylval.value = Ints_of_single(atoi(yytext)); return value; }

"//".*"\n"   {;}
[ \n\t\r\f]+ {;}

. {;}

%%
