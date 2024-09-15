#include<stdio.h>
#include<ctype.h>
#include<string.h>
#include<stdlib.h>
#include<assert.h>
#include<stdbool.h>
#include<time.h>
#include<math.h>

int oplocation = 0;
int namecnt = 0;
int cnt = 0;
char str[100];
const char *ops = "+-*/()=";
const char *chs = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_";
const char *nums = "123456789";
const char *nums_2 = "0123456789";
int valid = 1;

	enum
{
	op,
	va
}hhh;


typedef struct token//´æ´¢Æ÷ 
{
	int type;//1--operator;2--variable;3--integer;4--double;5--error;
	char str[100];

}Token; 
Token tokens[2500];

typedef struct assignment
{
	char name[100];
	int ival;
	double dval;
}Assignment;
Assignment assignments[2222];



int main(void)
{
	tokens[0].type = op;
}
