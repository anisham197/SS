%{
#include <stdio.h>
int yylex();
int yyerror(char *s);
%}

%token NUM LET NL
%%

S : LET A NL {printf("Valid variable!\n"); return 0;};
    | LET NL {printf("Valid variable!\n"); return 0;};
A : LET A
  | NUM A
  | LET
  | NUM
  ;
%%

int yyerror(char *s)
{
	printf("Invalid variable!\n");
	return 1;
}

int main()
{
	printf("Enter variable: ");
	yyparse();
	return 0;
}
