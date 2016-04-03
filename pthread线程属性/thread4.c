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
int main()
{
     pthread_attr_t attr;
     pthread_attr_init(&attr);

     int state;
     //线程是否可以分离
     pthread_attr_getdetachstate(&attr,&state);
     if(state==PTHREAD_CREATE_JOINABLE)
         cout<<"detachstate:PTHREAD_CREATE_JOINABLE"<<endl;
     else if(state==PTHREAD_CREATE_DETACHED)
         cout<<"PTHREAD_CREATE_DATACHED"<<endl;

     size_t size;
     //新创建线程栈的最小尺寸
     pthread_attr_getstacksize(&attr,&size);
     cout<<"stacksize:"<<size<<endl;
     //新创建线程栈的溢出保护区的大小
     pthread_attr_getguardsize(&attr,&size);
     cout<<"guardsize:"<<size<<endl;

     int scope;
     //新创建线程的作用域
     pthread_attr_getscope(&attr,&scope);
     if(scope==PTHREAD_SCOPE_PROCESS)
         cout<<"scope: PTHREAD_SCOPE_PROCESS"<<endl;
     else if(scope==PTHREAD_SCOPE_SYSTEM)
         cout<<"scope: PTHREAD_SCOPE_SYSTEM"<<endl;

     int policy;
     //线程调度策略
     pthread_attr_getschedpolicy(&attr,&policy);
     if(policy==SCHED_FIFO)
         cout<<"policy :SCHED_FIFO"<<endl;
     else if(policy==SCHED_RR)
         cout<<"policy :SCHED_RR"<<endl;
     else if(policy==SCHED_OTHER)
         cout<<"policy :SCHED_OTHER"<<endl;

     int inheritsched;
     //怎样设置调度属性
     pthread_attr_getinheritsched(&attr,&inheritsched);
     if(inheritsched==PTHREAD_INHERIT_SCHED)
         cout<<"inheritsched:PTHREAD_INHERIT_SCHED"<<endl;
     else if(inheritsched==PTHREAD_EXPLICIT_SCHED)
         cout<<"inheritsched:PTHREAD_EXPLICIT_SCHED"<<endl;

     struct sched_param param;
     //线程优先级
     pthread_attr_getschedparam(&attr,&param);
     cout<<"sched_priority:"<<param.sched_priority<<endl;

     pthread_attr_destroy(&attr);
     return 0;
}
