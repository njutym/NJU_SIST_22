#include<stdio.h>
#include<ctype.h>
#include<string.h>
#include<stdlib.h>
#include<assert.h>
#include<stdbool.h>
#include<time.h>
#include<math.h>

typedef struct token
{
    enum
    {
        INTEGERNUM,
        FLOATNUM,
        VARIABLE,
        OPERATOR
    } type;
    char str[32];
} Token;
Token tokens[512];

void copy_str(int l, int r, char *str, int num)
{
    for (int i = l, j = 0; i <= r; i++, j++)
    {
        tokens[cnt].str[j] = *(str + i);
    }
}



int word(int num)
{
    int len = strlen(tokens[num].str);
    char *dot = strchr(tokens[num].str, '.');
    // op
    if (len == 1 && strstr(ops, tokens[num].str))
    {
        tokens[num].type = OPERATOR;
        return 0;
    }
    // variable
    else if (strchr(chs, tokens[num].str[0]))
    {
        if (strchr(tokens[num].str, '+') ||
            strchr(tokens[num].str, '-') ||
            strchr(tokens[num].str, '*') ||
            strchr(tokens[num].str, '/') ||
            strchr(tokens[num].str, '=') ||
            strchr(tokens[num].str, '(') ||
            strchr(tokens[num].str, ')') ||
            strchr(tokens[num].str, '.'))
        {
            return 1;
        }
        tokens[num].type = VARIABLE;
        return 0;
    }
    // integer
    else if (len == 1 && tokens[num].str[0] == '0')
    {
        tokens[num].type = INTEGERNUM;
    }
    else if (strchr(nnumbers, tokens[num].str[0]) && !strchr(tokens[num].str, '.'))
    {
        for (int i = 0; i < len; i++)
        {
            if (isdigit(tokens[num].str[i]))
            {
                continue;
            }
            else
            {
                return 1;
            }
        }
        tokens[num].type = INTEGERNUM;
        return 0;
    }
}


int main(void)
{
	while (scanf("%[^\n]", str) != EOF)
    {
        // Input
        int cnt = 0; // tokens[num]
        getchar();
        int len = strlen(str);
        int l = 0, r = 0;
        for (int i = 0; i < len; i++)
        {
            if (str[i + 1] == ' ' || i == len - 1)
            {
                r = i;
                copy_str(l, r, str, num);
                num++;
                l = i + 2;
            }
            i++;
        }
} 
 



