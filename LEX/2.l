%{
        #include<stdio.h>
        int count = 0;
%}

%x C
%%
"/*"[^"*/"]*"*/"	{
                        int i, line = 1, length = strlen(yytext);
                        for (i = 0; i < length; i++)
                            if (yytext[i] == '\n')
                                line++;
                        count += line;
                    }

\/\/[^\n]*\n        {count++;}
%%

int main()
{
    yyin = fopen("input2.txt", "r");
    yylex();
    printf("No. of comment lines = %d\n", count);
    return 0;
}
