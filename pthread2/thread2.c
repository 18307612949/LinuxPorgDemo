#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <string>
#include <cstdio>
#include <iostream>
#include <errno.h>
#include <stdlib.h>

using namespace std;

//错误检查函数
inline void ERR_EXIT(const string &msg,int retnum)
{
    if(retnum!=0)
    {
        cerr<<msg<<": "<<strerror(retnum)<<endl;
        exit(EXIT_FAILURE);
    }
}

void * threadFunction(void *args)
{
    for(int i=0;i<3;i++)
        cout<<"This is a pthread"<<endl;

    pthread_exit((void *)&"I am thread one\n");
}
int main()
{
    pthread_t threadID;
    void * thread_res;
    int ret=pthread_create(&threadID,NULL,threadFunction,NULL);
    ERR_EXIT("pthread_create",ret);

    pthread_join(threadID,&thread_res);
    printf("thread joined,it returns:%s ",(char*)thread_res);
    return 0;
}

