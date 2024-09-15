#include<stdio.h>

typedef struct Token
{
	enum 
	{
		INT,
		DOUBLE,
		ERROR
	}type;
	int value;
}token;
token tokens[100];

int main(void)
{
	tokens[1].value = 1;
	tokens[1].type = token::INT;
	
} 
