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


typedef struct value
{
	enum
	{
		INT = 8888,
		DOUBLE = 9999,
		ERROR = 10000
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
    //double
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
}

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
	int lengthi = 5;
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
        else
            return assignments[index].value_of_assignment;
    }
    case 1:
    case 5:
        valid = false;
        return value_of_error;
    }
}

Value meetValue(Value v1,Value v2,char op)
{
Value meetvalue;
    if (v1.type == Value::ERROR || v2.type == Value::ERROR)
    {
        return value_of_error;
    }
    else if (v1.type == Value::DOUBLE || v2.type == Value::DOUBLE)
    {//类型升级
        double v1value;
        double v2value;
        v1value = (double)((v1.type == Value::INT) ? v1.val.iVal : v1.val.fVal);
        v2value = (double)((v2.type == Value::INT) ? v2.val.iVal : v2.val.fVal);
        meetvalue.type = Value::DOUBLE;
        switch (op)
        {
        case '+':
            meetvalue.val.fVal = v1value + v2value;
            break;
        case '-':
            meetvalue.val.fVal = v1value - v2value;
            break;
        case '*':
            meetvalue.val.fVal = v1value * v2value;
            break;
        case '/':
            if (v2value == -v2value)
            {
                return value_of_error;
            }
            meetvalue.val.fVal = v1value / v2value;
            break;
        default:
            break;
        }
        return meetvalue;
    }
    else if (v1.type == Value::INT && v2.type == Value::INT)
    {

        meetvalue.type = Value::INT;

        switch (op)
        {
        case '+':
            meetvalue.val.iVal = v1.val.iVal + v2.val.iVal;
            break;
        case '-':
            meetvalue.val.iVal = v1.val.iVal - v2.val.iVal;
            break;
        case '*':
            meetvalue.val.iVal = v1.val.iVal * v2.val.iVal;
            break;
        case '/':
            if (v2.val.iVal == 0)
            {
    
                return value_of_error;
            }
            meetvalue.val.iVal = v1.val.iVal / v2.val.iVal;
            break;
        default:
            break;
        }
      
     
     
        return meetvalue;
    }
    return value_of_error;//写了也没用不想报错
}

Value eval(int l,int r)
{
	if(l > r) return value_of_error;
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
			return meetValue(v1,v2,op);
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

bool check_equal(int l,int r)
{
	for(int i = l;i < r;i++)
	{
		if(tokens[i].str[0] == '=')
		{
			return true;
		}
	}
	return false;
}

Value evalAssign(int l,int r)
{
	if(check_equal(l,r) == true)
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
		
		Value ans = evalAssign(0,len);
		if(ans.type == Value::INT || ans.type == Value::DOUBLE || ans.type == Value::ERROR)
        printf("shit");
        printValue(ans);
		
       reset();
    }
}







































