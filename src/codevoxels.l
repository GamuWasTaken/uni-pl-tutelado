
%{
#include <stdio.h>
#include <stdlib.h>
#include "syn.h"

int c = 0;
int l = 1;

#define LOC(_col, _line) \
yylloc.first_line = l;   \
yylloc.first_column = c; \
c = _col;                \
l = _line;               \
yylloc.last_line = l;    \
yylloc.last_column = c;  \


%}

%option yylineno

%%

"="   { LOC(c+1,l); return eq     ;}
"<"   { LOC(c+1,l); return lt     ;}
">"   { LOC(c+1,l); return gt     ;}
"let" { LOC(c+1,l); return let    ;}
"fn"  { LOC(c+1,l); return fn     ;}

"|"   { LOC(c+1,l); return line   ;}
"("   { LOC(c+1,l); return lparen ;}
")"   { LOC(c+1,l); return rparen ;}
"{"   { LOC(c+1,l); return lcurly ;}
"}"   { LOC(c+1,l); return rcurly ;}
"["   { LOC(c+1,l); return lbrack ;}
"]"   { LOC(c+1,l); return rbrack ;}
","   { LOC(c+1,l); return comma  ;}

"->"  { LOC(c+1,l); return arrow  ;}
".."  { LOC(c+1,l); return rest   ;}


"."   { LOC(c+1,l); return dot    ;}

"+"   { LOC(c+1,l); return plus   ;}
"*"   { LOC(c+1,l); return star   ;}
"-"   { LOC(c+1,l); return dash   ;}
"/"   { LOC(c+1,l); return slash  ;}
"%"   { LOC(c+1,l); return mod    ;}

[@a-zA-Z][a-zA-Z_0-9]* { LOC(c+strlen(yytext),l); yylval.text = strdup(yytext); return name; }
[0-9]+ { LOC(c+strlen(yytext),l); yylval.ints = Ints_of_single(atoi(yytext)); return value; }

"//".*"\n"   {LOC(1, l+1);}
"\r"?"\n"    {LOC(1, l+1);}
[ \t]+ {LOC(c+1,l);}

. {LOC(c+1,l);}

%%
