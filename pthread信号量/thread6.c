#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <string>
#include <cstdio>
#include <iostream>
#include <errno.h>
#include <stdlib.h>
#include <semaphore.h>

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

//信号量实现线程间的同步
sem_t sem,semm;
char msg[512];
void * mythread(void *args)
{
    sem_wait(&sem);
    char *str=(char *)msg;
    while(strcmp("end\n",msg)!=0)
    {
        for(int i=0;str[i]!='\0';i++)
        {
            if(str[i]>='a' && str[i]<='z')
                str[i]=str[i]-('a'-'A');
        }
        cout<<"全部转换为大写字母："<<endl;
        cout<<str<<endl;
        sem_post(&semm);
        sem_wait(&sem);
    }
    sem_post(&semm);
    pthread_exit(NULL);
}
int main()
{
    int res;
    pthread_t thread;
    res=sem_init(&sem,0,0);
    //信号量为0时，代表期待的事件没有发生，不为0时代表已经发生
    ERR_EXIT("sem_init",res);

    res=sem_init(&semm,0,1);

    res=pthread_create(&thread,NULL,mythread,msg);
    ERR_EXIT("pthread_create",res);

    cout<<"Input some test.Enter 'end' to finish"<<endl;
    while(strcmp("end\n",msg)!=0)
    {
        sem_wait(&semm);
        fgets(msg,512,stdin);
        sem_post(&sem);
    }

    cout<<"waiting for thread to finish..."<<endl;
    res=pthread_join(thread,NULL);
    sem_destroy(&sem);
    sem_destroy(&semm);
    return 0;

}
