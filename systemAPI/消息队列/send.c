#include "common.h"
using namespace std;

struct msg_st
{
    long int msg_type;
    char text[512];
};
//发送消息
int main()
{
    int running=1;
    struct msg_st data;
    char buffer[BUFSIZ];
    int msgid=-1;

    msgid=msgget((key_t)1235,0666|IPC_CREAT);
    if(msgid==-1)
        ERR_EXIT("msgget failed with error");

    while(running)
    {
         printf("Enter some text: ");
         fgets(buffer,BUFSIZ,stdin);
         data.msg_type=1;
         strcpy(data.text,buffer);

         if(msgsnd(msgid,(void*)&data,512,0)==-1)
             ERR_EXIT("msgsnd error");
         if(strncmp(buffer,"end",3)==0)
             running=0;
         sleep(1);
    }
    return 0;
}
