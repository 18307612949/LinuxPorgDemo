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
//使用线程特定数据，全局变量，每个线程特有
pthread_key_t key;
pthread_once_t once_control=PTHREAD_ONCE_INIT;

typedef struct TSD
{
     pthread_t tid;
     char *str;
}tsd_t;

void dest_function(void *value)
{
    free(value);
    cout<<"destructor..."<<endl;
}

void init_routine()
{
    pthread_key_create(&key,dest_function);
    cout<<"init..."<<endl;
}
void *thread_routine(void *args)
{
    pthread_once(&once_control,init_routine);
    //设置线程特定数据
    tsd_t *value=(tsd_t*)malloc(sizeof(tsd_t));
    value->tid=pthread_self();
    value->str=(char *)args;
    pthread_setspecific(key,value);

    //获取线程特定数据
    value=(tsd_t *)pthread_getspecific(key);
    cout<<"tid: "<<value->tid<<"  str: "<<value->str<<endl;
    sleep(2);

    value=(tsd_t *)pthread_getspecific(key);
    cout<<"tid: "<<value->tid<<"  str: "<<value->str<<endl;

    pthread_exit(NULL);

}
int main()
{
    pthread_t tid1,tid2;
    pthread_create(&tid1,NULL,thread_routine,(void*)"thread1");
    pthread_create(&tid2,NULL,thread_routine,(void*)"thread2");

    pthread_join(tid1,NULL);
    pthread_join(tid2,NULL);

    pthread_key_delete(key);
    return 0;
}
