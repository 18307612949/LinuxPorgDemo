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

//示例1，多线程的基本创建
void * thread_routine(void *args)
{
    for(int i=0;i<5;i++)
    {
         printf("B");
         fflush(stdout);
         usleep(20);
    }
    pthread_exit(NULL);
}

//示例2,主线程和子线程之间传递数据
typedef struct _Student
{
    char name[20];
    unsigned int age;
}Student;

void * threadFunction(void *args)
{
    cout<<"In thread: "<<pthread_self()<<endl;
    Student tmp=*(Student*)(args);
    cout<<"Name: "<<tmp.name<<endl;
    cout<<"Age: "<<tmp.age<<endl;

    pthread_exit(NULL);
}
int main()
{
    pthread_t threadID;
    int ret=pthread_create(&threadID,NULL,thread_routine,NULL);
    ERR_EXIT("pthread_create",ret);

    for(int i=0;i<5;i++)
    {
         printf("A");
         fflush(stdout);
         usleep(20);
    }

    ret=pthread_join(threadID,NULL);
    ERR_EXIT("pthread_join",ret);

    cout<<endl;


    Student stu={"Tach",22};
    pthread_t thread2;
    pthread_create(&thread2,NULL,threadFunction,&stu);
    pthread_join(thread2,NULL);

    return 0;

}

