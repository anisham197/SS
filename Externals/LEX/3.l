%{
    //identifiers and operators
    #include <stdio.h>
    int flags[4]={0,0,0,0};
    int num_operators = 0,num_identifiers = 0;
    char stack[100];
    int top = -1;
    int opflag = 0;
    void validate();
%}

%x operand_active

%%

[_a-zA-Z][_a-zA-Z0-9]*                    {num_identifiers++;  printf("Indentfier: %s\n",yytext);}
<operand_active>[_a-zA-Z][_a-zA-Z0-9]*    {num_identifiers++;  printf("Indentfier: %s\n",yytext); opflag=0; BEGIN(0);}
[0-9]+                                    {num_identifiers++;  printf("Constant: %s\n",yytext);}
<operand_active>[0-9]+                    {num_identifiers++;  printf("Constant: %s\n",yytext); opflag=0; BEGIN(0);}
[+]                                       {flags[0]++;      opflag=1; BEGIN(operand_active);}
[-]                                       {flags[1]++;     opflag=1; BEGIN(operand_active);}
[*]                                       {flags[2]++;       opflag=1; BEGIN(operand_active);}
[/]                                       {flags[3]++;       opflag=1; BEGIN(operand_active);}
[ \t]                                     {;}
[(]                                       {stack[++top]='(';}
[)]                                       {
                                            if(stack[top] == '(')
                                            {
                                                top--;
                                            }
                                          }
[;]                                       {validate(); return 0 ; }
<operand_active>[;]                       {validate(); BEGIN(0); return 0;}

%%

int main()
{
    printf("Enter an expression(eg a-b;)\n");
    yylex();
    return 0;
}

void validate()
{
    num_operators = flags[0]+flags[1]+flags[2]+flags[3];
    if(num_identifiers==num_operators+1 && top == -1 && !opflag)
    {
        printf("\nExpression is valid and contains the following operators: ");
        if(flags[0]) printf("+ ");
        if(flags[1]) printf("- ");
        if(flags[2]) printf("* ");
        if(flags[3]) printf("/ ");
        printf("\n");
    }
    else
    {
        printf("\nInvalid Expression");
    }
}
