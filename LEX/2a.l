%{
#include<stdio.h>
int count = 0;
%}

%x C
%%
"/*"		{BEGIN C; }
<C>"*/"		{BEGIN 0; count++;}
<C>\n		{count++; /* Multi Line, Remove to consider multiline as single*/ }
<C>.		{;}
\/\/.*		{count++;}
%%

int main()
{
	yyin=fopen("input2.txt","r");
	yylex();
	printf("Count: %d\n", count);
	return 0;
}

int yywrap()
{
	return 1;
}


int yywrap(){
	return 1;
}

int yyerror(char* s){
	printf("%s\n",s);
}
