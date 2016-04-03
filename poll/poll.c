#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>
#include <poll.h>

#define ERR_EXIT(m) \
    do{ \
        perror(m); \
        exit(EXIT_FAILURE); \
    }while(0)

int main()
{
    //一次响应RST,二次SIGPIPE,防止退出
    signal(SIGPIPE,SIG_IGN);
    int listenfd; //监听套接字
    int count=0;
    if((listenfd=socket(PF_INET,SOCK_STREAM,0))<0)
        ERR_EXIT("socket error!");
    struct sockaddr_in servaddr;
    memset(&servaddr,0,sizeof(servaddr));
    servaddr.sin_family=AF_INET;
    servaddr.sin_port=htons(5188);
    servaddr.sin_addr.s_addr=htons(INADDR_ANY);//不指定特定IP

    int on=1;
    if(setsockopt(listenfd,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on))<0)
        ERR_EXIT("setsockopt error");
    if(bind(listenfd,(struct sockaddr*)&servaddr,sizeof(servaddr))<0)
        ERR_EXIT("bind error");
    if(listen(listenfd,SOMAXCONN)<0)
        ERR_EXIT("listen error");

    struct sockaddr_in peeraddr;
    socklen_t peerlen=sizeof(peeraddr);

    int conn;//已连接套接字
    int i;
    struct pollfd client[1024];
    int maxi=0;//client[]中最大不空闲位置的下标

    for(i=0;i<1024;i++)
        client[i].fd=-1;

    int nready;
    //加入监听套接字
    client[0].fd=listenfd;
    client[0].events=POLLIN;
    //events等待的事件，revents实际发生的事件

    while(1)
    {
        //poll检测[0,maxi+1),-1永不超时
        nready=poll(client,maxi+1,-1);
        if(nready==-1)
        {
            if(errno==EINTR)
                 continue;
             ERR_EXIT("poll error");
        }

        if(nready==0)
            continue;
        //如果监听套接口发生了可读事件
        if(client[0].revents & POLLIN)
        {
            //peeraddr接收连接方的信息
             conn=accept(listenfd,(struct sockaddr*)&peeraddr,&peerlen);
             if(conn==-1)
                 ERR_EXIT("accept error");
             for(i=1;i<1024;i++)
             {
                  if(client[i].fd<0)
                  {
                      client[i].fd=conn;
                      if(i>maxi)
                          maxi=i;
                      break;
                  }
             }

             if(i==1024)
             {
                 fprintf(stderr,"too many clients\n");
                 exit(EXIT_FAILURE);
             }
             printf("count = %d\n",++count);
             printf("recv connect ip=%s port=%d\n",inet_ntoa(peeraddr.sin_addr),ntohs(peeraddr.sin_port));

             client[i].events=POLLIN; //关注已连接套接字的数据到来事件
             //至少有一个已连接事件发生了可读，才去处理
             if(--nready<=0)
                 continue;
        }

        for(i=1;i<=maxi;i++)
        {
             conn=client[i].fd;
             if(conn==-1)
                 continue;
             if(client[i].revents & POLLIN)
             {
                 char buf[1024]={0};
                  int readBytes;
                  readBytes=read(conn,buf,sizeof(buf));
                       printf("%s",buf);
                  if(readBytes==-1)
                      ERR_EXIT("read error");
                  else if(readBytes==0)//客户端关闭
                  {
                      printf("client close\n");
                      client[i].fd=-1;
                      close(conn);
                  }

                  if(write(conn,buf,readBytes)==-1)
                      ERR_EXIT("write socket error");
                  memset(buf,0,sizeof(buf));
                  if(--nready<=0)//没有了已连接的可读事件
                      break;
             }
        }

    }
    return 0;

}
