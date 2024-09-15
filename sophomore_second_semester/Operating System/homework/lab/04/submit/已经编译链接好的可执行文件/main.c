#include "myos.h"

int main(void)
{   
    while(1)
    {
        write("hello!",6);
        sleep(1);
        write("goodbye!",8);
        sleep(1);
    }
}