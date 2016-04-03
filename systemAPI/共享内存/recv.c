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
    int shmid=shm_open("/xyz",O_RDONLY,0);
    if(shmid==-1)
        ERR_EXIT("shm_open error");

    struct stat buf;
    if(fstat(shmid,&buf)==-1)
        ERR_EXIT("fstat error");

    Student *p=(Student*)mmap(NULL,buf.st_size,PROT_READ,MAP_SHARED,shmid,0);
    if(p==MAP_FAILED)
        ERR_EXIT("mmap error");

    cout<<"name: "<<p->name<<",age: "<<p->age<<endl;

    munmap(p,buf.st_size);
    close(shmid);
}
