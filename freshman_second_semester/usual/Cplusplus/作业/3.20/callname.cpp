#include <string>
#include <iostream>
using namespace std;
#include<stdio.h>
#include<stdlib.h>
#include<string.h>


int funcMyAdd(int a,int b);
int funcMySub(int a,int b);
int funcMyHash(int a,int b);
void* functions[3]={(void*)funcMyAdd,(void*)funcMySub,(void*)funcMyHash};

class CallMyName
{
    void** all_functions;
public:
    CallMyName(void* af[3]){
        all_functions=af;
    }
    int call(const string &my_call)
    {
        const char *s;
        s = (&my_call).c_str();
        if(my_call[6] == 'H')
        {
            int a = atoi(&s[my_call.find('(')+1]);
            int b = atoi(&s[my_call.find(',')+1]);
            return funcMyAdd(a,b);
        }
        else if(my_call[6] == 'S')
        {
            int aa = atoi(&s[my_call.find('(')+1]);
            int bb = atoi(&s[my_call.find(',')+1]);
            return funcMySub(aa,bb);
        }
        else
        {
            int aaa = atoi(&s[my_call.find('(')+1]);
            int bbb = atoi(&s[my_call.find(',')+1]);
            return funcMyHash(aaa,bbb);
        }
    }
};

