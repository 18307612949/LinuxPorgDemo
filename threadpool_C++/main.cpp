#include "commen.h"
#include "threadpool.h"

void *myTask(void *args)
{
    int number = *(int *)args;
    delete (int *)args;

    printf("thread 0x%lx is working on task %d ...\n",
           (unsigned long)pthread_self(), number);
    sleep(1);

    return NULL;
}

int main()
{
    int nThread;
    cout << "input the number to start thread: ";
    cin >> nThread;
    ThreadPool pool(nThread);
    for (int i = 0; i < 10; ++i)
    {
        pool.addTask(myTask, new int(i));
        sleep(1);
    }

}

