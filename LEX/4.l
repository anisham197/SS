%{
        #include <stdio.h>
        int count = 0;
%}

%%
[a-zA-Z_][_0-9a-zA-Z]*  { printf("%s\n", yytext); count++;}
.*  {;}
\n  {;}
%%

int main()
{
    yyin = fopen("input4.txt", "r");
    printf("List of Valid Identifiers\n");
    yylex();
    printf("No. of Valid Identifiers = %d\n", count);
    return 0;
}

int yywrap(){
	return 1;
}

int yyerror(char* s){
	printf("%s\n",s);
}
