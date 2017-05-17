%{
#include <stdio.h>
int yylex();
int yyerror(char *s);
%}

%token A B NL

%%
S : C D NL {printf("Valid string!\n"); return 0;}

C : A C |

D : B D |
  ;
%%

int yyerror(char *s)
{
	printf("Invalid string!\n");
	return 0;
}

int main()
{
	printf("Enter the string: ");
	yyparse();
	return 0;
}
