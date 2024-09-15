#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// structure
typedef struct token // token们
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

typedef struct value // 值们
{
    enum
    {
        INT,
        FLOAT,
        ERROR
    } type;
    union
    {
        int ival;
        double fval;
    } val;
} Value;

typedef struct op // 符号们
{
    enum
    {
        PLUS,
        MINUS,
        MUL,
        DIV,
        EQUAL,
        LBRACKET,
        RBRACKET
    } type;
} Op;

typedef struct assignment // 变量们
{
    char name[32];
    Value val;
} Assignment;
Assignment varies[128] = {0};

// Input part
char str[1024] = {0};                            // 读取一行
void copy_str(int l, int r, char *str, int num); // 从str复制到对应token

// judge
char *ops = "+-*)/(=";
char *chs = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_";
char *nnumbers = "123456789";
int word(int num); // 判断词法
int Is_Error = 0;  // 全局错误判断
int time = 0;

// evaluate
Value eval(int l, int r);                   // 计算
Value evalAssign(int l, int r);             // 赋值
int loadnum = 0;                            // 储存变量下标
int check_parentheses(int l, int r);        // 检查括号
int check_minus(int l, int r);              // 检查负号
Value meetValue(Value v1, Value v2, Op op); // 不同相碰
int principle(int l, int r);                // 主运算符
Op opp(int i);                              // 读取运算符类型

// print
void printValue(Value v); // 打印

int main()
{
    while (scanf("%[^\n]", str) != EOF)
    {
        // Input
        int num = 0; // tokens[num]
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
                i++;
            }
        }
        // Wordlegal
        Is_Error = 0;
        for (int i = 0; i < num; i++)
        {
            if (word(i))
            {
                Is_Error = 1;
            };
        }
        // Sentencelegal

        // Evaluate and store variable
        Value ans = evalAssign(0, num - 1);
        // Print
        if (Is_Error)
        {
            printf("Error\n");
        }
        else
        {
            printValue(ans);
        }
        // Empty
        memset(str, 0, sizeof(str));
        memset(tokens, 0, sizeof(tokens));
    }

    return 0;
}
// input

void copy_str(int l, int r, char *str, int num)
{
    for (int i = l, j = 0; i <= r; i++, j++)
    {
        tokens[num].str[j] = str[i];
    }
    tokens[num].str[r - l + 1] = '\0';
}

// judge

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
        return 0;
    }
    else if (strchr(nnumbers, tokens[num].str[0]) && !strchr(tokens[num].str, '.'))
    {
        for (int i = 0; i < len; i++)
        {
            if (tokens[num].str[i] <= '9' && tokens[num].str[i] >= '0')
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
    // float
    else if (len >= 3 && dot)
    {
        if ((dot != strrchr(tokens[num].str, '.')) ||
            (tokens[num].str[0] == '0' && tokens[num].str[1] != '.') ||
            (tokens[num].str[0] == '.') ||
            (tokens[num].str[len - 1] == '.'))
        {
            return 1;
        }
        for (int i = 0; i < len; i++)
        {
            if ((tokens[num].str[i] >= '0' && tokens[num].str[i] <= '9') || tokens[num].str[i] == '.')
            {
                continue;
            }
            else
            {
                return 1;
            }
        }
        tokens[num].type = FLOATNUM;
        return 0;
    }
    // elsewrong
    return 1;
}

// evaluate

Value eval(int l, int r)
{
    struct value error = {.type = ERROR};
    if (Is_Error)
    {
        return error;
    }
    struct value ans;
    if (l > r)
    {
        return error;
    }
    else if (l == r)
    {
        if (tokens[l].type == INTEGERNUM)
        {
            ans.type = INT;
            ans.val.ival = atoi(tokens[l].str);
            return ans;
        }
        if (tokens[l].type == FLOATNUM)
        {
            ans.type = FLOAT;
            ans.val.fval = atof(tokens[l].str);
            return ans;
        }
        if (tokens[l].type == VARIABLE)
        {
            for (int i = 0; i < loadnum; i++)
            {
                if (strcmp(varies[i].name, tokens[l].str) == 0)
                {
                    ans = varies[i].val;
                    return ans;
                }
            }
            Is_Error = 1;
            return error;
        }
        Is_Error = 1;
        return error;
    }
    else if (check_parentheses(l, r))
    {
        return eval(l + 1, r - 1);
    }
    else if (check_minus(l, r))
    {
        if (Is_Error)
        {
            return error;
        }
        Value tmp = eval(l + 1, r);
        if (tmp.type == INT)
        {
            ans.type = INT;
            ans.val.ival = -tmp.val.ival;
            return ans;
        }
        if (tmp.type == FLOAT)
        {
            ans.type = FLOAT;
            ans.val.fval = -tmp.val.fval;
            return ans;
        }
        Is_Error = 1;
        return error;
    }
    else
    {
        if (Is_Error)
        {
            return error;
        }

        int pos = principle(l, r);
        if (!pos)
        {
            Is_Error = 1;
            return error;
        }
        Op op = opp(pos);
        Value val1 = eval(l, pos - 1);
        Value val2 = eval(pos + 1, r);
        return meetValue(val1, val2, op);
    }
}

Value evalAssign(int l, int r)
{
    Value error = {.type = ERROR};
    if (Is_Error)
    {
        return error;
    }
    if (tokens[l + 1].str[0] == '=')
    {
        if (tokens[l].type == VARIABLE)
        {
            Value val = evalAssign(l + 2, r);
            if (val.type == ERROR)
            {
                return val;
            }
            for (int i = 0; i < loadnum; i++)
            {
                if (strcmp(varies[i].name, tokens[l].str) == 0)
                {
                    varies[i].val = val;
                    return val;
                }
            }
            strcpy(varies[loadnum].name, tokens[l].str);
            varies[loadnum].val = val;
            loadnum++;
            return val;
        }
        else
        {
            Is_Error = 1;
            return error;
        }
    }
    else
    {
        return eval(l, r);
    }
}

int check_parentheses(int l, int r)
{
    if (tokens[l].str[0] == '(' && tokens[r].str[0] == ')')
    {
        int left = 0, right = 0;
        for (int i = l + 1; i <= r - 1; i++)
        {
            if (tokens[i].str[0] == '(')
            {
                if (!left)
                {
                    left = i;
                }
            }
            if (tokens[i].str[0] == ')')
            {
                if (!right)
                {
                    right = i;
                }
            }
        }
        if ((!left && !right) || (left < right))
        {
            return 1;
        }
    }
    return 0;
}

int check_minus(int l, int r)
{
    if (tokens[l].str[0] == '-')
    {
        if (l + 1 == r)
        {
            return 1;
        }
        else if (tokens[l + 1].str[0] == '-')
        {
            return check_minus(l + 1, r);
        }
        else if (check_parentheses(l + 1, r))
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
    return 0;
}

Value meetValue(Value v1, Value v2, Op op)
{
    Value ans;
    if (v1.type == ERROR || v2.type == ERROR)
    {
        Is_Error = 1;
        ans.type = ERROR;
        return ans;
    }
    if (v1.type != v2.type || (v1.type == FLOAT && v2.type == FLOAT))
    {
        ans.type = FLOAT;
        if (v1.type == INT)
        {
            switch (op.type)
            {
            case PLUS:
                ans.val.fval = v1.val.ival + v2.val.fval;
                break;
            case MINUS:
                ans.val.fval = v1.val.ival - v2.val.fval;
                break;
            case MUL:
                ans.val.fval = v1.val.ival * v2.val.fval;
                break;
            case DIV:
                ans.val.fval = v1.val.ival / v2.val.fval;
                break;
            default:
                break;
            }
        }
        else if (v2.type == INT)
        {
            switch (op.type)
            {
            case PLUS:
                ans.val.fval = v1.val.fval + v2.val.ival;
                break;
            case MINUS:
                ans.val.fval = v1.val.fval - v2.val.ival;
                break;
            case MUL:
                ans.val.fval = v1.val.fval * v2.val.ival;
                break;
            case DIV:
                ans.val.fval = v1.val.fval / v2.val.ival;
                break;
            default:
                break;
            }
        }
        else
        {
            switch (op.type)
            {
            case PLUS:
                ans.val.fval = v1.val.fval + v2.val.fval;
                break;
            case MINUS:
                ans.val.fval = v1.val.fval - v2.val.fval;
                break;
            case MUL:
                ans.val.fval = v1.val.fval * v2.val.fval;
                break;
            case DIV:
                ans.val.fval = v1.val.fval / v2.val.fval;
                break;
            default:
                break;
            }
        }
    }
    if (v1.type == INT && v2.type == INT)
    {
        ans.type = INT;
        switch (op.type)
        {
        case PLUS:
            ans.val.ival = v1.val.ival + v2.val.ival;
            break;
        case MINUS:
            ans.val.ival = v1.val.ival - v2.val.ival;
            break;
        case MUL:
            ans.val.ival = v1.val.ival * v2.val.ival;
            break;
        case DIV:
            ans.val.ival = v1.val.ival / v2.val.ival;
            break;
        default:
            break;
        }
    }
    return ans;
}

int principle(int l, int r)
{
    int i = r;
    int brac = 0;
    int cc = 0;

    for (; i > l; i--)
    {
        if (tokens[i].str[0] == ')')
        {
            for (; i > l + 1; i--)
            {
                if (tokens[i].str[0] == ')')
                {
                    brac++;
                }
                if (tokens[i].str[0] == '(')
                {
                    brac--;
                }
                if (!brac)
                {
                    break;
                }
            }
            i--;
        }
        if (tokens[i].str[0] == '-')
        {
            for (; i > l + 1; i--)
            {
                if (tokens[i - 1].str[0] != '-')
                {
                    break;
                }
            }
            if (i == l + 1 && tokens[l].str[0] == '-')
            {
                if (cc)
                {
                    return cc;
                }
                else
                {
                    Is_Error = 1;
                    return 0;
                }
            }
            if (tokens[i - 1].str[0] == '*' || tokens[i - 1].str[0] == '/')
            {
                if (!cc)
                {
                    cc = i - 1;
                    continue;
                }
                else
                {
                    return cc;
                }
            }
            else if (tokens[i - 1].str[0] == '+')
            {
                return i - 1;
            }
            else
            {
                return i;
            }
        }
        if (tokens[i].str[0] == '+')
        {
            return i;
        }
        if ((tokens[i].str[0] == '*' || tokens[i].str[0] == '/') && !cc)
        {

            cc = i;
            continue;
        }
    }
    if (cc)
    {
        return cc;
    }
    return 0;
}
Op opp(int i)
{
    Op ans;
    switch (tokens[i].str[0])
    {
    case '+':
        ans.type = PLUS;
        break;
    case '-':
        ans.type = MINUS;
        break;
    case '*':
        ans.type = MUL;
        break;
    case '/':
        ans.type = DIV;
        break;
    default:
        break;
    }
    return ans;
}

void printValue(Value v)
{
    switch (v.type)
    {
    case INT:
        printf("%d\n", v.val.ival);
        break;
    case FLOAT:
        printf("%.6f\n", v.val.fval);
        break;
    case ERROR:
        printf("Error\n");
        break;
    }
}

