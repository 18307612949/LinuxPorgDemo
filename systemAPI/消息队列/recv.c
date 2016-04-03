#include "common.h"
using namespace std;

struct msg_st
{
    long int msg_type;
    char text[BUFSIZ];
};

//消息接收
int main()
{
    int running=1;
    int msgid=-1;
    struct msg_st data;
    long int msgtype=0;

    msgid=msgget((key_t)1235,0666|IPC_CREAT);
    if(msgid==-1)
        ERR_EXIT("msgid error");

    while(running)
    {
        if(msgrcv(msgid,(void*)&data,BUFSIZ,msgtype,0)==-1)
            ERR_EXIT("msgrcv error");

        printf("You wrote: %s\n",data.text);

        //end结束
        if(strncmp(data.text,"end",3)==0)
            running=0;
    }
    if(msgctl(msgid,IPC_RMID,0)==-1)
        ERR_EXIT("msgctl error");

    return 0;
}
