#include "common.h"

using namespace std;
//信号屏蔽和解除
void sigHandler(int signo)
{
    switch(signo)
    {
        case SIGINT:
            cout<<"catch a signal SIGINT "<<endl;
            break;
        case SIGQUIT:
            sigset_t unblockSet;
            sigemptyset(&unblockSet);
            sigaddset(&unblockSet,SIGINT);
            if(sigprocmask(SIG_UNBLOCK,&unblockSet,NULL)==-1)
                ERR_EXIT("sigprocmask unblock error");
            else
                cout<<"sigprocmask success"<<endl;
            break;
        default:
            cerr<<"unknown signal "<<endl;
            break;

    }
}

void printSigSet(const sigset_t &sigset)
{
     for(int i=1;i<NSIG;i++)
     {
         if(sigismember(&sigset,i))
             putchar('1');
         else
             putchar('0');
     }
     putchar('\n');

}
int main(int argc,char *argv[])
{
    if(signal(SIGINT,sigHandler)==SIG_ERR)
        ERR_EXIT("signal SIGINT error");
    if(signal(SIGQUIT,sigHandler)==SIG_ERR)
        ERR_EXIT("signal SIGQUIT error");

    sigset_t addset;
    sigemptyset(&addset);
    sigaddset(&addset,SIGINT);
    if(sigprocmask(SIG_BLOCK,&addset,NULL)==-1)
        ERR_EXIT("sigprocmask error");

    sigset_t sigset;
    while(1)
    {
        sigpending(&sigset);
        printSigSet(sigset);
        sleep(1);
    }
}
