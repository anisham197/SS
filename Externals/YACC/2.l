%{
#include "y.tab.h"
%}

%%
[0-9]+			return NUM;
[a-zA-Z]		return LET;
\n			    return NL;
.			    return yytext[0];
%%

int yywrap()
{
	return 1;
}
