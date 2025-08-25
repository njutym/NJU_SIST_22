#include "myos.h"

int main(void)
{   
    if(fork()==0)
    {
        while(1)
        {
            write("Ping!",5);
            sleep(1);
        }
    }
    else
    {
        while(1)
        {
            write("Pong!",5);
            sleep(2);
        }
    }
}