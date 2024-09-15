#include<stdio.h>
#include<ctype.h>
#include<string.h>
#include<stdlib.h>
#include<math.h>
#include<stdbool.h>

//1--operator;2--variable;3--integer;4--double;5--error;

char str[100];
int cnt_of_variable = 0;
int cnt = 0; 
int valid = 1;
const char *ops = "+-*/()=";
const char *chs = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_";
const char *nums = "123456789";
const char *nums_2 = "0123456789";
const char *kuohaos = "()";
const char *opop = "+-*/";

typedef struct value
{
	enum
	{
		INT,
		DOUBLE,
		ERROR
	}type;
	union
	{
		int iVal;
		double fVal;
	}val;
}Value;
Value value_of_error;


typedef struct assignment//存储变量数值和名字的关系 
{
	char name[100];
	Value value_of_assignment; 
}Assignment;
Assignment assignments[2222];


typedef struct token
{
	int type;
	char str[100];
}Token; 
Token tokens[2500];


void copy_str(int l, int r, char *str, int cnt)//存储到tokens中 
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
            valid = false;
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
        	 valid = false;
             return;
			}
        }
        tokens[cnt].type = 3;
        return;
    }
    //double
	else if(strchr(nums_2, tokens[cnt].str[0]) && strchr(tokens[cnt].str, '.'))
	{
		if(tokens[cnt].str[0] == '.' || tokens[cnt].str[lenlen-1] == '.')
		{
			tokens[cnt].type = 5;
				valid = false;
				return;
		} 
		int temporarycnt = 0;
		for(int i = 0;i < lenlen;i++)
		{
			if(!isdigit(tokens[cnt].str[i]) && tokens[cnt].str[i] != '.') 
			{
				tokens[cnt].type = 5;
				valid = false;
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
			valid = false;
			return;
		}
		tokens[cnt].type = 4;
		return;
	}
	//error
	else
	{
		tokens[cnt].type = 5;
		valid = false;
		valid = 0;
		return;
	}
}

void reset()
{
    valid = true;
    for (int i = 0; i < 2222; i++)
    {
        memset(tokens[i].str, 0, sizeof(char) * 2222);
        tokens[i].type = -1;
    }
}

bool check_parenthese(int l,int r)
{
	if(tokens[l].str[0] != '(' || tokens[r].str[0] != ')')
	{
		return false;
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
				return false;
			} 
		}
		return true;
	}
}


int find_op(int l,int r)
{
	int j1left = 0;
	int j1right = 0;
	int j2left = 0;
	int j2right = 0;
	int left = 0;
	int right = 0;
	int oplocation = -1;
	int remain[1000] = {0};
	for(int i = l;i <= r;i++)
	{
		if(tokens[i].type == 1 && !strchr(kuohaos,tokens[i].str[0]))
		{
			remain[i] = 1;
		}
		if(tokens[i].type == 1 && tokens[i].str[0] == '-')
		{
			if(i == l) remain[i] = 0;
			else if(strchr(opop,tokens[i-1].str[0])) remain[i] = 0;
		}
	}
	
	for(int i = l;i <= r;i++)
	{	
		if(remain[i] == 0) continue;
		left = 0;
		right = 0;
		j1left = 0;
		j1right = 0;
		j2left = 0;
		j2right = 0;
		for(int j1 = i;j1 >= l;j1--)
		{
			if(tokens[j1].str[0] == ')') j1right++;
			if(tokens[j1].str[0] == '(') j1left++;
			if(j1left > j1right) left = 1;
		}
		for(int j2 = i;j2 <= r;j2++)
		{
			if(tokens[j2].str[0] == ')') j2right++;
			if(tokens[j2].str[0] == '(') j2left++;
			if(j2left < j2right) right = 1;
		}
		if(left == 1 && right == 1)
		{
			remain[i] = 0;
		}
	}

	int jiajian = 0;
	for(int i = r;i >= l;i--)
	{
		if(remain[i] == 1)
		{
			if(tokens[i].str[0] == '+' || tokens[i].str[0] == '-')
			{
				jiajian = 1;
				break;
			}
		}
	}
	if(jiajian == 1)
	{
		for(int i = r;i >= l;i--)
		{
			if(remain[i] == 1 && (tokens[i].str[0] == '+' || tokens[i].str[0] == '-'))
			{
				oplocation = i;
				return oplocation;
			}
		}
	}
	else
	{
		for(int i = r;i >= l;i--)
		{
			if(remain[i] == 1)
			{
				oplocation = i;
				return oplocation;
			}
		}
	}
	return oplocation;
}


/*
int find_op(int l,int r)
{
	int oplocation = -1;
    int optype = 0;
    for (int i = l; i <= r; i++)
    {
        if (tokens[i].str[0] == '(')
        {
            int left = 0;
            left++;
            while (left > 0 && i <= r)
            {
                i++;
                if (tokens[i].str[0] == '(')
                    left++;
                else if (tokens[i].str[0] == ')')
                    left--;
            }
        }
        else if (tokens[i].type == 1)
        {
            if (tokens[i].str[0] == '+')
            {
                optype = 1;
                oplocation = i;
            }
            else if (tokens[i].str[0] == '-')
            {
                if (i != l && (tokens[i-1].str[0] == ')' || tokens[i-1].type != 1))
                {
                    optype = 1;
                    oplocation = i;
                }
            }
            else if (optype == 0 && (tokens[i].str[0] == '*') )
            {
                oplocation = i;
            }
            else if(optype == 0 && (tokens[i].str[0] == '/'))
            {
            	oplocation = i;
			}
        }
    }
    return oplocation;
}*/

int jisuani(int l)
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

double jisuanf(int l)
{
	int floatindex = -1;
	int lengthi = strlen(tokens[l].str);
	int tokensjisuan[2222];
	for(int i = 0;i < lengthi;i++)
	{
		if(tokens[l].str[i] == '.')
		{
			floatindex = i;
			continue;	
		} 
		tokensjisuan[i] = tokens[l].str[i] - '0';
	}
	double sumf = 0.0;
	for(int i = 0;i < floatindex;i++)
	{
		sumf += pow(10,floatindex-1-i) * tokensjisuan[i];
	}
	for(int i = floatindex + 1;i < lengthi;i++)
	{
		sumf += pow(10,floatindex - i) * tokensjisuan[i];
	}
	return sumf;
}

int search(int l)
{
	for(int i = 0;i < cnt_of_variable;i++)
	{
		if(strcmp(assignments[i].name,tokens[l].str) == 0)
		{
			return i;
		}
	}
	return -1;
}

Value oneeval(int l)
{
	Value valassist;
    switch (tokens[l].type)
    {
    case 3:
        valassist.type = Value::INT;
        valassist.val.iVal = jisuani(l);
        return valassist;
    case 4:
        valassist.type = Value::DOUBLE;
        valassist.val.fVal = jisuanf(l);
        return valassist;
    case 2:
    {
        int index = search(l);
        if (index == -1)
        {
            valid = false;
            return value_of_error;
        }
        else  return assignments[index].value_of_assignment;
    }
    case 1:
    case 5:
    	{
    		valid = false;
       		return value_of_error;
		}
    }
}

Value meetValue(Value v1,Value v2,char op)
{
	Value meet;
	if(v1.type == Value::ERROR || v2.type == Value::ERROR)
	{
		return value_of_error;
	}

		if(v1.type == Value::DOUBLE && v2.type == Value::DOUBLE)
		{
			meet.type = Value::DOUBLE;
			double v1value = v1.val.fVal;
			double v2value = v2.val.fVal;
			switch (op)
			{
			case '+' : meet.val.fVal = v1value + v2value; break;
			case '-' : meet.val.fVal = v1value - v2value; break;
			case '*' : meet.val.fVal = v1value * v2value; break;
			case '/' : meet.val.fVal = v1value / v2value; break;
			}
		}
		else if(v1.type == Value::DOUBLE && v2.type == Value::INT)
		{
			meet.type = Value::DOUBLE;
			double v1value = v1.val.fVal;
			double v2value = (double)v2.val.iVal;
			switch (op)
			{
			case '+' : meet.val.fVal = v1value + v2value; break;
			case '-' : meet.val.fVal = v1value - v2value; break;
			case '*' : meet.val.fVal = v1value * v2value; break;
			case '/' : meet.val.fVal = v1value / v2value; break;
			}
		}
		else if(v2.type == Value::DOUBLE && v1.type == Value::INT)
		{
			meet.type = Value::DOUBLE;
			double v2value = v2.val.fVal;
			double v1value = (double)v1.val.iVal;
			switch (op)
			{
			case '+' : meet.val.fVal = v1value + v2value; break;
			case '-' : meet.val.fVal = v1value - v2value; break;
			case '*' : meet.val.fVal = v1value * v2value; break;
			case '/' : meet.val.fVal = v1value / v2value; break;
			}
		}
		else if(v2.type == Value::INT && v1.type == Value::INT)
		{
			meet.type = Value::INT;
			int v1value = v1.val.iVal;
			int v2value = v2.val.iVal;
			switch (op)
			{
			case '+' : meet.val.iVal = v1value + v2value; break;
			case '-' : meet.val.iVal = v1value - v2value; break;
			case '*' : meet.val.iVal = v1value * v2value; break;
			case '/' : meet.val.iVal = v1value / v2value; break;
			}
		}
	return meet;
}

Value eval(int l,int r)
{
	if(l > r)
	{
		valid = false;
		return value_of_error;
	} 
	else if(l == r)
	{
		return oneeval(l);
	}
	else if(check_parenthese(l,r) == true)
	{
		return eval(l + 1,r - 1);
	}
	else
	{
		int op = find_op(l,r);
		if(op == -1)
		{
			if(tokens[l].str[0] != '-')
			{
				valid = 0;
				return value_of_error;
			}
			else
			{
				Value fu = eval(l + 1, r);
                switch (fu.type)
                {
                case Value::INT: fu.val.iVal *= -1; break;
                case Value::DOUBLE: fu.val.fVal *= -1; break;
                }
                return fu;
			}
		}
		else
		{
			Value v1 = eval(l,op-1);
			Value v2 = eval(op+1,r);
			return meetValue(v1,v2,tokens[op].str[0]);
		}
	}
}

Value load(Value variable,int l)
{
	switch(variable.type)
	{
		case Value::INT :
		{
			int index = search(l);
        	if (index == -1)
        	{
            	strcpy(assignments[cnt_of_variable].name, tokens[l].str);
            	assignments[cnt_of_variable].value_of_assignment = variable;
            	cnt_of_variable++;
        	}
    		else
       		{
            	assignments[index].value_of_assignment  = variable;
        	}
       		return variable;
		}
		break;
		case Value::DOUBLE :
		{
			int index = search(l);
        	if (index == -1)
        	{
            	strcpy(assignments[cnt_of_variable].name, tokens[l].str);
            	assignments[cnt_of_variable].value_of_assignment = variable;
            	cnt_of_variable++;
        	}
    		else
       		{
            	assignments[index].value_of_assignment = variable;
        	}
       		return variable;
		}
		break;
		case Value::ERROR:
        {
			valid = false;
        	return value_of_error;
		}
		break;
	} 
}

void printValue(Value v)
{
	switch (v.type)
	{
	case Value::INT:
        printf("%d\n", v.val.iVal);
        break;
    case Value::DOUBLE:
        printf("%.6lf\n", v.val.fVal);
        break;
    case Value::ERROR:
        printf("Error\n");
        break;
	}
}

int check_equal(int l,int r)
{
	int loclocloc = -1;
	for(int i = l;i <= r;i++)
	{
		if(tokens[i].str[0] == '=')
		{
			loclocloc = i;
			return loclocloc;
		}
	}
	return -1;
}

Value evalAssign(int l,int r)
{
	if(check_equal(l,r) != -1)
	{
       	if (tokens[l].type == 2 && tokens[l + 1].str[0] == '=')
        {
        	return load(evalAssign(l + 2, r), l);
        }
        else
        {
        	valid = false;
       		return value_of_error;
        }
	}
	else
	{
		return eval(l,r);
	}
}

int main(void)
{
	value_of_error.type = Value::ERROR;
	while(scanf("%[^\n]", str) != EOF)//一次读入一行 
    {
    	getchar();
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
		if(valid == false)
		{
			printf("Error\n");
		}
		else
		{
			Value ans = evalAssign(0,cnt-1);
        	printValue(ans);
		}
    	reset();
    }
}














































