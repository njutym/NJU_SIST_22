#include<stdio.h>
#include<ctype.h>
#include<string.h>
#include<stdlib.h>
#include<assert.h>
#include<stdbool.h>
#include<time.h>
#include<math.h>

int namecnt = 0;
int cnt = 0;
char str[32];
const char *ops = "+-*/()=";
const char *chs = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_";
const char *nums = "123456789";
const char *nums_2 = "0123456789";
int valid = 1;

typedef struct token//存储器 
{
	int type;//1--operator;2--variable;3--integer;4--double;5--error;
	char str[32];
}Token; 
Token tokens[2222];

typedef struct assignment
{
	char name[32];
	int ival;
	double dval;
}Assignment;
Assignment assignments[2222];



void copy_str(int l, int r, char *str, int cnt)
{
    for (int i = l, j = 0; i <= r; i++, j++)
    {
        tokens[cnt].str[j] = *(str + i);
    }
}

void kindjudge(int cnt)//输入类型判断 
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

int check_parentheses(int l,int r)//判断括号是否匹配 
{
	if(tokens[cnt].str[l] != '(' || tokens[cnt].str[r] != ')')
	{
		return 0;
	} 
	else
	{
		int left_fight_right = 0;
		for(int i = l + 1;i <= r - 1;i++)
		{
			if(tokens[i].str[0] == '(') left_fight_right++;
			else if(tokens[i].str[0] == ')') left_fight_right--;
			if(left_fight_right == -1) 
			{
				valid = 0;
				return 0;
			} 
		}
		return 1;
	}
}

int find_op(int l,int r)//寻找主运算符 
{
	int lenhh = r - l + 1;
	int judgeop[lenhh + 1];
	for(int i = 0;i < lenhh;i++)
	{
		if(tokens[cnt].type == 1)
		judgeop[i] = 1;
	}
	int leftkuohao = 0;
	int rightkuohao = 0;
	for(int i = 0;i < lenhh;i++)
	{
		for(int j = 0;j < i;j++)
		{
			if(judgeop[i] == 1 && tokens[cnt].str[0] == '(') leftkuohao++;
		}
		for(int j = i + 1;j < lenhh;j++)
		{
			if(judgeop[i] == 1 && tokens[cnt].str[0] == ')') rightkuohao++;
		}
		if(leftkuohao > 0 && rightkuohao > 0) judgeop[i] = 0; 
	}
	for(int i = lenhh - 1;i >= 0;i--)
	{
		if(judgeop[i] == 1)
		{
			int opp = tokens[cnt].str[0];
			return opp;
		}
	}
}

void reset()//初始化
{
}

int ivalue(int l)//整数计算 
{	
	int lengthi = strlen(tokens[l].str);
	int tokensjisuan[2222];
	for(int i = 0;i < lengthi;i++)
	{
		tokensjisuan[i] = tokens[l].str[i] - '0';
	}
	int sumi = 0;
	for(int i = 0;i < lengthi;i++)
	{
		sumi += pow(10,lengthi-1-i) * tokensjisuan[i];
	}
	return sumi;
}

double dvalue(int l)//浮点数计算
{
	int lengthd = strlen(tokens[l].str);
	double sumd = 0.0;
	int findpoint = 0;
	for(int i = 0;i < lengthd;i++)
	{
		if(tokens[l].str[i] == '.')
		findpoint = i;
	}
	for(int i = 0;i < findpoint;i++)
	{
		sumd += tokens[l].str[i] * pow(10,findpoint-1-i);
	}
	int j = -1;
	for(int i = findpoint + 1;i < lengthd;i++,j--)
	{
		sumd += tokens[l].str[i] * pow(10,j);
	}
	return sumd;
} 

int valtype(int l)//判断赋值的类型 
{
	if(strchr(tokens[l].str,'.'))
	{
		return 1;
	}
	else return 0;
}


void assign()//赋值
{
	int valuevalue;
	if(valtype(cnt-1) == 0) valuevalue = ivalue(cnt-1);
	else valuevalue = dvalue(cnt-1);
	for(int i = 0;i < cnt - 1;i++)
	{
		if(!strchr(tokens[i].str,'='))
		{
			int lentoken = strlen(tokens[i].str);
			for(int j = 0;j < lentoken;j++)
			{
				assignments[namecnt].name[j] = tokens[i].str[j];	
			}
			if(valtype(cnt-1) == 0)
			{
				assignments[namecnt].ival = valuevalue;
			}
			else assignments[namecnt].dval = valuevalue;
		}
	}
	namecnt++;
}


double eval(int l,int r)//最终求值 
{
	if(l > r)
	{
		valid = 0;
		return 0;
	}
	
	else if(l == r)
	{
		if(tokens[l].type == 3)
		{
			return ivalue(l);
		}
		else if(tokens[l].type == 4)
		{
			return dvalue(l);
		}
		else if(tokens[l].type == 2)
		{
			int nameright = 0;
			int location = 0;
			for(int i = 0;i < namecnt;i++)
			{
				int lenname = strlen(assignments[i].name);
				for(int j = 0;j < lenname;j++)
				{
					if(tokens[l].str[j] == assignments[i].name[j])
					nameright ++;
				}
				if(nameright == lenname)
				{
					location = i;
					nameright = 1;
				}
			}
			if(nameright == 0)
			{
				valid = 0;
				return 0;
			}
			else
			{
				if(assignments[location].ival != 114514)
				{
					return assignments[location].ival;
				}
				else return assignments[location].dval;
			}
		}
	}

	else if(check_parentheses(l,r) == 1)
	{
		return eval(l+1,r-1);
	}
	
	else
	{
		char fop = find_op(l,r);
		int value_1 = eval(l,fop - 1);
		int value_2 = eval(fop + 1,r);
		if(fop == '+') return (value_1 + value_2);
		if(fop == '-') return (value_1 - value_2);
		if(fop == '*') return (value_1 * value_2);
		if(fop == '/') return (value_1 / value_2);
	}
}


void output()
{
	if(valid = 0)
	{
		printf("Error\n");
		return;
	}
	
}




int main(void)
{
	 while (scanf("%[^\n]", str) != EOF)
    {
        cnt = 0; 
        getchar();
        int len = strlen(str);
        int l = 0, r = 0;
        for (int i = 0; i < len; i++)
        {
            if (str[i + 1] == ' ' || i == len - 1)
            {
                r = i;
                copy_str(l, r, str, cnt);
                cnt++;
                l = i + 2;
            }
        }
        
        for(int i = 0;i < cnt;i++)
		{
			kindjudge(i);
		}
        printf("%d",ivalue(0));
    }
}

