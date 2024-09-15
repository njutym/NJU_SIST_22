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

typedef struct token//�洢�� 
{
	int type;//1--operator;2--variable;3--integer;4--double;5--error;
	char str[32];
}Token; 
Token tokens[2222];

typedef struct assignment
{
	char name[32];
	int ival;
}Assignment;
Assignment assignments[2222];



void copy_str(int l, int r, char *str, int cnt)
{
    for (int i = l, j = 0; i <= r; i++, j++)
    {
        tokens[cnt].str[j] = *(str + i);
    }
}

void kindjudge(int cnt)//���������ж� 
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

int check_parentheses(int l,int r)//�ж������Ƿ�ƥ�� 
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

char find_op(int l,int r)//Ѱ��������� 
{
	int lenhh = r - l + 1;
	int judgeop[lenhh + 1];
	for(int i = l;i <= r;i++)
	{
		if(tokens[i].type == 1)
		judgeop[i] = 1;
	}
	int leftkuohao = 0;
	int rightkuohao = 0;
	for(int i = l;i < r;i++)
	{
		for(int j = 0;j < i;j++)
		{
			if(judgeop[i] == 1 && tokens[i].str[0] == '(') leftkuohao++;
		}
		for(int j = i + 1;j < lenhh;j++)
		{
			if(judgeop[i] == 1 && tokens[i].str[0] == ')') rightkuohao++;
		}
		if(leftkuohao > 0 && rightkuohao > 0) judgeop[i] = 0; 
	}
	for(int i = lenhh - 1;i >= 0;i--)
	{
		if(judgeop[i] == 1)
		{
			char opp = tokens[cnt].str[i];
			return opp;
		}
	}
}

void reset()//��ʼ��
{
	
}

int ivalue(int l)//�������� 
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
		sumi += pow(10,lengthi-1-i) * (tokensjisuan[i]);
	}
	return sumi;
}


void assign()//��ֵ
{
	int valuevalue;
	valuevalue = ivalue(cnt-1);
	for(int i = 0;i < cnt - 1;i++)
	{
		if(!strchr(tokens[i].str,'='))
		{
			int lentoken = strlen(tokens[i].str);
			for(int j = 0;j < lentoken;j++)
			{
				assignments[namecnt].name[j] = tokens[i].str[j];	
			}
				assignments[namecnt].ival = valuevalue;
		}
	}
	namecnt++;
}


int eval(int l,int r)//������ֵ 
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
			 return	assignments[location].ival;
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
		printf("%d",eval(0,cnt-1));
		printf("\n");
	    getchar();
    }
}



