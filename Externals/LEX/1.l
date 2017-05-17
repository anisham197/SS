%{
    #include<stdio.h>
    int chars = 0, words = 0, spaces = 0, lines = 0;
%}

%%
[^ \n\t]+    { words++; chars += yyleng;}
\n          {lines++; }
\t          {spaces++; }
" "         {spaces++; }
%%

int main()
{
    yyin = fopen("input1.txt", "r");
    yylex();
    printf("Characters = %d\nWords = %d\nSpaces = %d\nLines = %d\n", chars, words, spaces, lines);
    return 0;
}
