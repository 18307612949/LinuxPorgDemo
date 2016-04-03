#include "common.h"
using namespace std;

struct Student
{
    char name[4];
    int age;
};

//发送消息
int main()
{
    int shmid=shm_open("/xyz",O_RDWR|O_CREAT,0666);
    if(shmid==-1)
        ERR_EXIT("shm_open error");

    ftruncate(shmid,sizeof(Student));
    struct stat buf;
    if(fstat(shmid,&buf)==-1)
        ERR_EXIT("fstat error");

    Student *p=(Student*)mmap(NULL,buf.st_size,PROT_WRITE|PROT_READ,MAP_SHARED,shmid,0);
    if(p==MAP_FAILED)
        ERR_EXIT("mmap error");
    strcpy(p->name,"Hadoop");
    p->age=5;

    munmap(p,buf.st_size);
    close(shmid);
}
