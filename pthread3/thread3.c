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
//线程取消
void * threadFunction(void *args)
{
    int ret;
    ret=pthread_setcancelstate(PTHREAD_CANCEL_ENABLE,NULL);//默认就是这个
    ERR_EXIT("pthread_setcancelstate",ret);
    //ret=pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS,NULL);
    for(int i=0;i<100;i++)
    {
        cout<<"this is a thread.."<<endl;
    }
    //pthread_exit((void *)&"I am thread one\n");
    //
    cout<<"end"<<endl;
    pthread_exit(NULL);
}
int main()
{
    pthread_t threadID;
    void * thread_res;
    int ret=pthread_create(&threadID,NULL,threadFunction,NULL);
    ERR_EXIT("pthread_create",ret);

    ret=pthread_cancel(threadID);
    ERR_EXIT("pthread_cancel",ret);

    pthread_join(threadID,NULL);
   // printf("thread joined,it returns:%s ",(char*)thread_res);
    return 0;
}

