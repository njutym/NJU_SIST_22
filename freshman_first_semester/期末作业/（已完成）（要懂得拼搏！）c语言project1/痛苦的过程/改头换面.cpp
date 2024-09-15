#include<stdio.h>
#include<ctype.h>
#include<string.h>
#include<stdlib.h>
#include<assert.h>
#include<stdbool.h>
#include<time.h>
#include<math.h>
#define Each_Len_LenMaxN 1300 //ÿ���
#define Each_Token_LenMaxN 40 //ÿtoken�
#define Variable_MaxN 150 //�����������
#define Num_of_operator 7 //�������������������

const char operator_reserved[Num_of_operator + 2] = { '+', '-', '*', '/', '(', ')', '=' };
int cnt_of_token_eachlen = 0;//ÿ�а�����token��Ŀ
int cnt_of_variable = 0;//���еı�����
bool valid = true;//ÿ���Ƿ�Ϸ�
int judge_end;//�Ƿ�����л��߳������

//ö�� token����
enum Type_of_token
{
    Type_of_Error,
    Type_of_Integer,
    Type_of_Double,
    Type_of_Variable,
    Type_of_Operator
};

//ö�� expr ��ֵ
enum Value_of_expr
{
    Value_of_ERROR,
    Value_of_INT,
    Value_of_DOUBLE
};

//��¼���ʽ��ֵ
typedef struct
{
    int type_of_value; //���ʽ����

    union //���ʽ��ֵ
    {
        int iVal;
        double fVal;
    }val;
} Value;
Value value_of_error;//���� error

//��¼token �� char������
typedef struct
{
    //Value value_of_token;
    int type;                      //ֵΪType_of_sth
    char char_of_token[Each_Token_LenMaxN]; //������ɶ
}Token;
//��¼ÿ��ÿ��token
Token tokens[Each_Len_LenMaxN];

//��¼����
typedef struct
{
    Value value_of_assignment;
    char name[Each_Token_LenMaxN];
}Assignment;
Assignment assignments[Variable_MaxN];

//��Ϊȫ�ֱ�������Ҫ��ʼ��
void Reset();
//�����ʽ ����ȷ����
bool Check_parentheses(int l, int r);
//�����ʽ ����ȷ����
bool Check_Minus(int l, int r);
//�ҵ�������� �����±�
int Findop(int l, int r);
//��������
Value MeetValue(Value v1, Value v2, char op);
//���и�ֵ����
Value Load(Value variable, int l);
//��ӡ
void PrintValue(Value v);
void fPrintValue(Value v, FILE *fp);
/*�жϱ����Ƿ��м�¼ ��ֵ�ʹ�����¼���±� û��ֵ�ʹ���0
���Ұ� token ��ֵΪ �Ѽ�¼�ı���*/
int Find_Value_of_Variable(Token *token_is_varible);
/* �ж� token������ ���ʷ����� ����ֵΪtoken��type
���Ҷ� ��ֵ ���м�¼��ֵ */
int Judge_Type_of_Token(Token *nowtoken);
Value single_eval(int l);
//�Ƿ��﷨���� ���ʽ��ֵ
Value eval(int l, int r);
//�����ֵ
Value Asseval(int l, int r);

int main() {

    value_of_error.type_of_value = Value_of_ERROR;
    judge_end = 1;
    while (judge_end != EOF)
    {
        //cnt = 0 valid = 1 all token 0
        Reset();
        //���� ���� �ж��Ƿ�ʷ�����
 
        do
        {
            if (scanf("%s", tokens[cnt_of_token_eachlen].char_of_token) == EOF)
                goto end;
            judge_end = getchar();
            //��� �ʷ�����
            if (valid)
            {
                tokens[cnt_of_token_eachlen].type = Judge_Type_of_Token(&tokens[cnt_of_token_eachlen]);
                if (tokens[cnt_of_token_eachlen].type == Type_of_Error)
                    valid = false;
            }
            cnt_of_token_eachlen++;
        } while (judge_end != EOF && judge_end != '\n');
        //fp
       
        for (int i = 0; i < cnt_of_token_eachlen; i++)
          
        
        for (int i = 0; i < cnt_of_token_eachlen; i++)
        
     

        Value hhh = Asseval(0, cnt_of_token_eachlen - 1);
        
   
     
      
    }
end:;
   
    
    

    return 0;
}

void Reset() {
    cnt_of_token_eachlen = 0;
    valid = true;
    //value tokens
    for (int i = 0; i < Each_Len_LenMaxN; i++)
    {
        memset(tokens[i].char_of_token, 0, sizeof(char) * Each_Token_LenMaxN);
        tokens[i].type = -1;
    }
}

bool Check_parentheses(int l, int r) {
    if (tokens[l].char_of_token[0] != '('
        || tokens[r].char_of_token[0] != ')')
    {
       
        return false;
    }
    else //�����ǣ���
    {
       
        int num_of_left = 1;
        for (int i = l + 1; i < r; i++)
        {
            if (tokens[i].char_of_token[0] == '(')
                num_of_left++;
            else if (tokens[i].char_of_token[0] == ')')
                num_of_left--;
            if (num_of_left == 0)
            {
                
                return false;
            }
            if (num_of_left < 0)
            {
                
                valid = false;
                return false;
            }
        }
       
        return true;
    }
}

bool Check_Minus(int l, int r) {
    if (tokens[l].char_of_token[0] == '-')
    {
      
        return true;
    }return false;
}

int Findop(int l, int r) {
    int oploc = -1;
    bool optype = 0;//1�������Ѿ��ҵ��˸߹��+-��
    for (int i = l; i <= r; i++)
    {
        //��Ծ����
        if (tokens[i].char_of_token[0] == '(')
        {
            int left = 0;
            left++;
            while (left > 0 && i <= r)
            {
                i++;
                if (tokens[i].char_of_token[0] == '(')
                    left++;
                else if (tokens[i].char_of_token[0] == ')')
                    left--;
            }
        }
        else if (tokens[i].type == Type_of_Operator)
        {
            if (tokens[i].char_of_token[0] == '+')
            {
                optype = true;
                oploc = i;
            }
            else if (tokens[i].char_of_token[0] == '-')
            {
                if (i != l &&
                    (tokens[i - 1].char_of_token[0] == ')' || tokens[i - 1].type != Type_of_Operator))
                {
                    optype = true;
                    oploc = i;
                }
            }
            else if (optype == 0
                && (tokens[i].char_of_token[0] == '*' ||
                    tokens[i].char_of_token[0] == '/'))
            {
                oploc = i;
            }
            // = ��ʱδ����
            // else if (tokens[i].char_of_token[0] == '=')
            // {
            //     oploc = i;
            //     break;
            //}
        }
    }
   
    return oploc;
}

Value MeetValue(Value v1, Value v2, char op) {
    
    
    Value meetvalue;
    if (v1.type_of_value == Value_of_ERROR || v2.type_of_value == Value_of_ERROR)
    {
      
        return value_of_error;
    }
    else if (v1.type_of_value == Value_of_DOUBLE || v2.type_of_value == Value_of_DOUBLE)
    {//��������
      
        double v1value;
        double v2value;
        v1value = (double)((v1.type_of_value == Value_of_INT) ? v1.val.iVal : v1.val.fVal);
        v2value = (double)((v2.type_of_value == Value_of_INT) ? v2.val.iVal : v2.val.fVal);
        meetvalue.type_of_value = Value_of_DOUBLE;
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
    else if (v1.type_of_value == Value_of_INT && v2.type_of_value == Value_of_INT)
    {
      
        meetvalue.type_of_value = Value_of_INT;
        
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
    return value_of_error;//д��Ҳû�ò��뱨��
}

Value Load(Value assval, int l) {
    
    switch (assval.type_of_value)
    {
    case Value_of_INT:
    case Value_of_DOUBLE:
    {
        int index = Find_Value_of_Variable(&tokens[l]);
        if (index == -1)
        {
          
            strcpy(assignments[cnt_of_variable].name, tokens[l].char_of_token);
            assignments[cnt_of_variable].value_of_assignment = assval;
            cnt_of_variable++;
        }
        else
        {
           
            assignments[index].value_of_assignment = assval;
        }
        return assval;
    }
    break;
    case Value_of_ERROR:
        valid = false;
        return value_of_error;
        break;
    default:
       
        return value_of_error;
        break;
    }
    return value_of_error; //д��Ҳû��
}

void PrintValue(Value v) {
    switch (v.type_of_value)
    {
    case Value_of_INT:
        printf("%d\n", v.val.iVal);
        break;
    case Value_of_DOUBLE:
        printf("%.6lf\n", v.val.fVal);
        break;
    case Value_of_ERROR:
        printf("Error\n");
        break;
    default:
        break;
    }
}

void fPrintValue(Value v, FILE *fp) {
    switch (v.type_of_value)
    {
    case Value_of_INT:
        fprintf(fp, "%d\n", v.val.iVal);
        break;
    case Value_of_DOUBLE:
        fprintf(fp, "%.6lf\n", v.val.fVal);
        break;
    case Value_of_ERROR:
        fprintf(fp, "Error\n");
        break;
    default:
        break;
    }
}

int Find_Value_of_Variable(Token *token_is_varible) {
    for (int i = 0; i < cnt_of_variable; i++)
    {
        //�������
        if (!strcmp(token_is_varible->char_of_token, assignments[i].name))
        {
           
            
            return i;
        }
    }
   
    return -1;
}

int Judge_Type_of_Token(Token *nowtoken) {
    
    int len_token = strlen(nowtoken->char_of_token);
    char *nowchar = nowtoken->char_of_token;
    //���ֻ�� 1
    if (len_token == 1)
    {//no double
        //int
        if (isdigit(nowchar[0]))
        {
           
            return Type_of_Integer;
        }
        //var
        else if (isalpha(nowchar[0]) || nowchar[0] == '_')
        {
           
            return Type_of_Variable;
        }//indeed oper is wrong too but we do not process it now 
        for (int i = 0; i < Num_of_operator; i++)
        {
            if (nowchar[0] == operator_reserved[i])
            {
             
                return Type_of_Operator;
            }
        }
        //other type is absorlutely wrong
        return Type_of_Error;
    }
    else //not only one char
    {
        bool point_num = 0;
        int start = 0;
        //if it starts with a 0, it is double or wrong
        if (nowchar[0] == '0')
        {
            //0a 01 || 0. 
            if (len_token == 2 || nowchar[1] != '.')
            {
                //fprintf(fp, "%s error meaning\n", nowchar);
                return Type_of_Error;
            }
            else //0.?
            {
                for (int i = 2; valid && i < len_token; i++)
                    valid = isdigit(nowchar[i]);
                if (valid)//�� ������
                {
                    //fprintf(fp, "%s is a double\n", nowchar);
                    return Type_of_Double;
                }
                //there is something that is not digit in this 0.? expr
                else
                {
                    //fprintf(fp, "%s error meaning\n", nowchar);
                    return Type_of_Error;
                }
            }
        }
        //start with 1 ~ 9 must be int double or error
        else if ('1' <= nowchar[0] && nowchar[0] <= '9')
        {
            int num_of_point = 0;

            for (int i = 1; valid && i < len_token; i++)
            {
                if (nowchar[i] == '.')
                {
                    //if we find a . before 
                    //or it is the last char of the token
                    if (num_of_point || (i == len_token - 1))
                    {
                       
                        return Type_of_Error;
                    }
                    num_of_point++;
                }
                else
                    valid = isdigit(nowchar[i]);
            }
            //�Ϸ�
            if (valid)
            {
                if (num_of_point == 1)
                {
                    
                    return Type_of_Double;
                }
                else if (num_of_point == 0)
                {
                    
                    return Type_of_Integer;
                }
                else
                {
                    
                    return Type_of_Error;
                }
            }
            else//there is char which is not digit
            {
               
                return Type_of_Error;
            }
        }
        // start with _ or alpha
        else if (isalpha(nowchar[0]) || nowchar[0] == '_')
        {
            for (int i = 1; valid && i < len_token; i++)
                valid = (isalnum(nowchar[i]) || nowchar[i] == '_');
            if (valid)
            {
               
                return Type_of_Variable;
            }
            else
            {
               
                return Type_of_Error;
            }
        }
        else // start with an error
        {
            
            return Type_of_Error;
        }
    }
   
    return Type_of_Error;
}

Value single_eval(int l) {
    Value sval;
    switch (tokens[l].type)
    {
    case Type_of_Integer:
        sval.type_of_value = Value_of_INT;
        sval.val.iVal = atoi(tokens[l].char_of_token);
        return sval;
    case Type_of_Double:
        sval.type_of_value = Value_of_DOUBLE;
        sval.val.fVal = atof(tokens[l].char_of_token);
        return sval;
    case Type_of_Variable:
    {
        int index = Find_Value_of_Variable(&tokens[l]);
        if (index == -1)
        {
            valid = false;
            return value_of_error;
        }
        else
            return assignments[index].value_of_assignment;
    }
    case Type_of_Operator:
    case Type_of_Error:
       
        valid = false;
        return value_of_error;
    default:
       
        valid = false;
        return value_of_error;
        break;
    }
}

Value eval(int l, int r) {
  
    if (valid == false)
    {
      
        return value_of_error;
    }
    if (l > r)
    {
       
        valid = false;
        return value_of_error;
    }
    else if (l == r)
        return single_eval(l);
    else if (Check_parentheses(l, r) == true)
        return eval(l + 1, r - 1);
    else
    {
        int op;
        op = Findop(l, r);
        if (op == -1)
        {
            if (Check_Minus(l, r) == false)
            {
                
                valid = false;
                return value_of_error;
            }
            else
            {
                Value minus_val = eval(l + 1, r);
                switch (minus_val.type_of_value)
                {
                case Value_of_INT:
                    minus_val.val.iVal *= -1;
                    break;
                case Value_of_DOUBLE:
                    minus_val.val.fVal *= -1;
                    break;
                default:
                    break;
                }
                return minus_val;
            }
        }
        //else
        Value v1 = eval(l, op - 1);
        Value v2 = eval(op + 1, r);
        return MeetValue(v1, v2, tokens[op].char_of_token[0]);
    }
    return value_of_error;//��ʵд��û��,���ǲ�ϲ������...
}

Value Asseval(int l, int r) {
    if (l > r)
    {
        valid = false;
        return value_of_error;
    }
    if (l == r)
    {
       
        return eval(l, r);
    }
    //�Ǳ�
    int thereis_equal = -10;
    for (int i = l; i <= r; i++)
    {
        if (tokens[i].char_of_token[0] == '=')
        {
           
            thereis_equal = i;
            break;
        }
    }
    if (thereis_equal >= 0)
    {
        // ���� = <expr>
        if (tokens[l].type == Type_of_Variable
            && tokens[l + 1].char_of_token[0] == '=')
        {
            
                
            return Load(Asseval(l + 2, r), l);
        }
        else
        {
            
            valid = false;
            return value_of_error;
        }
    }
    else // without '='
    {
   
    return eval(l, r);
    }
    return value_of_error;//��ʵд��û��,���ǲ�ϲ������
}

