#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <string>
#include <cstdio>
#include <assert.h>
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
//任务链表结构
typedef struct worker
{
    void *(*process)(void *arg);
    void *arg;
    struct worker *next;
}CThread_worker;

//线程池结构
typedef struct
{
     pthread_mutex_t queue_lock;
     pthread_cond_t queue_ready;
     //线程池中所有等待的任务
     CThread_worker *queue_head;
     //是否销毁线程池
     int shutdown;
     pthread_t *threadID;
     //线程池中允许的活动线程数目
     int max_thread_num;
     //当前等待队列中的任务数目
     int cur_queue_size;
}CThread_pool;

static CThread_pool *pool;
void *thread_routine(void *arg)
{
    printf("starting thread 0x%x\n",pthread_self());
    while(1)
    {
        pthread_mutex_lock(&(pool->queue_lock));
        //如果等待队列为0并且不销毁线程池，则处于阻塞状态
        if(pool->cur_queue_size==0 && !pool->shutdown)
        {
            printf("thread 0x%x is waiting\n",pthread_self());
            pthread_cond_wait(&(pool->queue_ready),&(pool->queue_lock));
        }

        //线程池要销毁了
        if(pool->shutdown)
        {
            //遇到break,continue,return等跳转语句，千万不要忘记先解锁
            pthread_mutex_unlock(&(pool->queue_lock));
            printf("thread 0x%x will exit\n",pthread_self());
            pthread_exit(NULL);
        }

        //运行
        printf("thread 0x%x is starting to work\n",pthread_self());

        pool->cur_queue_size--;
        CThread_worker *worker=pool->queue_head;
        pool->queue_head=worker->next;
        pthread_mutex_unlock(&(pool->queue_lock));

        //调用回调函数，执行任务
        (*(worker->process))(worker->arg);
        free(worker);
        worker=NULL;
    }
    //这句应该不可达
    pthread_exit(NULL);

}
void pool_init(int max_thread_num)
{
    pool=(CThread_pool*)malloc(sizeof(CThread_pool));
    pthread_mutex_init(&(pool->queue_lock),NULL);
    pthread_cond_init(&(pool->queue_ready),NULL);

    pool->queue_head=NULL;
    pool->max_thread_num=max_thread_num;
    pool->cur_queue_size=0;
    pool->shutdown=0;
    pool->threadID=(pthread_t*)malloc(max_thread_num*sizeof(pthread_t));

    for(int i=0;i<max_thread_num;i++)
        pthread_create(&(pool->threadID[i]),NULL,thread_routine,NULL);
}

//向线程池中加入任务
int pool_add_worker(void *(*process)(void *arg),void *arg)
{
    //一个新的任务
    CThread_worker* newworker=(CThread_worker*)malloc(sizeof(CThread_worker));
    newworker->process=process;
    newworker->arg=arg;
    newworker->next=NULL;

    pthread_mutex_lock(&(pool->queue_lock));
    //将任务加入等待队列
    CThread_worker *member=pool->queue_head;
    if(member!=NULL)
    {
         while(member->next!=NULL)
             member=member->next;
         member->next=newworker;
    }
    else
    {
         pool->queue_head=newworker;
    }
    assert(pool->queue_head!=NULL);

    pool->cur_queue_size++;

    pthread_cond_signal(&(pool->queue_ready));
    pthread_mutex_unlock(&(pool->queue_lock));

    return 0;
}


//销毁线程池，等待队列中的任务不会再被执行，正在运行的线程会一直把任务完成再退出
int pool_destory()
{
     if(pool->shutdown)
         return -1;
     pool->shutdown=1;

     //唤醒所有等待进程，线程池将要销毁了
     pthread_cond_broadcast(&(pool->queue_ready));
     //阻塞等待线程退出
     for(int i=0;i<pool->max_thread_num;i++)
     {
          pthread_join(pool->threadID[i],NULL);
     }
     free(pool->threadID);

     //销毁等待队列
     CThread_worker *head=NULL;
     while(pool->queue_head!=NULL)
     {
         head=pool->queue_head;
         pool->queue_head=pool->queue_head->next;
         free(head);
     }

     //销毁条件变量和互斥量
     pthread_mutex_destroy(&(pool->queue_lock));
     pthread_cond_destroy(&(pool->queue_ready));

     free(pool);
     pool=NULL;
     return 0;

}

void *myprocess(void *arg)
{
    printf("thread is 0x%x,worrking on task %d\n",pthread_self(),*(int*)arg);
    sleep(1);
    return NULL;
}
int main()
{
    pool_init(3);

    int *workingnum=(int*)malloc(sizeof(int)*10);
    for(int i=0;i<10;i++)
    {
         workingnum[i]=i;
         pool_add_worker(myprocess,&workingnum[i]);
    }

    sleep(5);
    pool_destory();
    free(workingnum);
    return 0;
}
