#include<stdio.h>
#include<ctype.h>
#include<string.h>
#include<stdlib.h>
#include<assert.h>
#include<stdbool.h>
#include<time.h>
#include<math.h>

int cnt = 0;
char str[32];
const char *ops = "+-*/()=";
const char *chs = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_";
const char *nums = "123456789";
const char *nums_2 = "0123456789";

typedef struct token
{
	int type;//1--operator;2--variable;3--integer;4--double;5--error;
	char str[32];
}Token; 

Token tokens[2222];

void copy_str(int l, int r, char *str, int cnt)
{
	int j = 0;
	for (int i = l; i <= r; i++)
    {
        tokens[cnt].str[j++] = *(str + i);
    }
}

void kindjudge(int cnt)
{
    int lenlen = strlen(tokens[cnt].str);
    // operator
    if (lenlen == 1 && strchr(ops, tokens[cnt].str[0]))
    {
        tokens[cnt].type = 1;
        return;
    }
    // variable
    else if (strchr(chs, tokens[cnt].str[0]))
    {
        if (strchr(tokens[cnt].str, '+') || strchr(tokens[cnt].str, '-') ||
            strchr(tokens[cnt].str, '*') || strchr(tokens[cnt].str, '/') ||
            strchr(tokens[cnt].str, '=') ||strchr(tokens[cnt].str, '(') ||
            strchr(tokens[cnt].str, ')') ||strchr(tokens[cnt].str, '.'))
        {
            tokens[cnt].type = 5;
            return;
        }
        tokens[cnt].type = 2;
        return; 
	}
    // integer
    else if(lenlen == 1 && tokens[cnt].str[0] == '0')
    {
        tokens[cnt].type = 3;
        return;
    }
    else if(strchr(nums, tokens[cnt].str[0]) && !strchr(tokens[cnt].str, '.'))
    {
        for(int i = 0;i < lenlen;i++)
        {
        	if(!isdigit(tokens[cnt].str[i]))
        	{
        	 tokens[cnt].type = 5;
             return;
			}
        }
        tokens[cnt].type = 3;
        return;
    }
    //float
	else if(strchr(nums_2, tokens[cnt].str[0]) && strchr(tokens[cnt].str, '.'))
	{
		int temporarycnt = 0;
		for(int i = 0;i < lenlen;i++)
		{
			if(!isdigit(tokens[cnt].str[i]) && tokens[cnt].str[i] != '.') 
			{
				tokens[cnt].type = 5;
				return;
			}
			if(tokens[cnt].str[i] == '.')
			{
				temporarycnt++;
			}
		}
		if(temporarycnt > 1)
		{
			tokens[cnt].type = 5;
			return;
		}
		tokens[cnt].type = 4;
		return;
	}
	//error
	else
	{
		tokens[cnt].type = 5;
		return;
	}
}


int main(void)
{
	while(scanf("%s",str) != EOF)
	{
		int len = strlen(str);
		for (int i = 0; i < len; i++)
   		{
        	tokens[cnt].str[i] = *(str + i);
   		}
		kindjudge(cnt);
		printf("%d",tokens[cnt].type);
		printf("\n"); 
		cnt++;
	}


		
}

