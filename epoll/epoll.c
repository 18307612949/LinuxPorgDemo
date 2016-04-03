#include <unistd.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/epoll.h>

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#include <algorithm>
#include <vector>
using namespace std;

#define ERR_EXIT(m) \
    do{ \
        perror(m); \
        exit(EXIT_FAILURE); \
    }while(0)

typedef vector<struct epoll_event> EventList;

int main()
{
    int count=0;
    int listenfd;
    if((listenfd=socket(AF_INET,SOCK_STREAM,0))<0)
        ERR_EXIT("Socket");

    struct sockaddr_in servaddr;
    memset(&servaddr,0,sizeof(servaddr));
    servaddr.sin_family=AF_INET;
    servaddr.sin_port=htons(5188);
    servaddr.sin_addr.s_addr=htonl(INADDR_ANY);

    int on=1;
    if(setsockopt(listenfd,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on))<0)
        ERR_EXIT("setsockopt");
    if(bind(listenfd,(struct sockaddr *)&servaddr,sizeof(servaddr))<0)
        ERR_EXIT("bind");
    if(listen(listenfd,SOMAXCONN)<0)
        ERR_EXIT("listen");

    vector<int> clients;
    int epollfd;
    epollfd=epoll_create1(EPOLL_CLOEXEC); //epoll实例句柄

    struct epoll_event event;
    event.data.fd=listenfd;
    event.events=EPOLLIN | EPOLLET; //使用边沿触发
    epoll_ctl(epollfd,EPOLL_CTL_ADD,listenfd,&event);

    //用于保存epoll_wait返回事件数组
    EventList events(16);
    struct sockaddr_in peeraddr;
    socklen_t peerlen;
    int conn;
    int i,nready;
    while(1)
    {
        //已经发生的事件被复制到events中
        nready=epoll_wait(epollfd,&*events.begin(),static_cast<int>(events.size()),-1);
        if(nready==-1)
        {
            if(errno==EINTR)
                continue;
            ERR_EXIT("epoll_wait");
        }
        if(nready==0)
            continue;
        if((size_t)nready==events.size())
            events.resize(events.size()*2);
        for(i=0;i<nready;i++)
        {
            //如果是监听套接字发生了可读事件
            if(events[i].data.fd==listenfd)
            {
                peerlen=sizeof(peeraddr);
                conn=accept(listenfd,(struct sockaddr *)&peeraddr,&peerlen);
                if(conn==-1)
                    ERR_EXIT("accept");
                printf("ip=%s port=%d\n",inet_ntoa(peeraddr.sin_addr),ntohs(peeraddr.sin_port));
                printf("count=%d\n",++count);
                clients.push_back(conn);

                //设置IO为非阻塞模式
                int flags=fcntl(conn,F_GETFL);
                  flags|=O_NONBLOCK;
                  fcntl(conn,F_SETFL,flags);

                event.data.fd=conn;
                event.events=EPOLLIN | EPOLLIN;
                epoll_ctl(epollfd,EPOLL_CTL_ADD,conn,&event);

            }
            //如果是已连接套接字发生了可读事件
            else if(events[i].events & EPOLLIN)
            {
                 conn=events[i].data.fd;
                 if(conn<0)
                     continue;
                 char buf[1024]={0};
                 int ret=read(conn,buf,sizeof(buf));
                 if(ret==-1)
                     ERR_EXIT("read");
                 if(ret==0)
                 {
                     printf("client close");
                     close(conn);

                     event=events[i];
                     epoll_ctl(epollfd,EPOLL_CTL_DEL,conn,&event);
                     clients.erase(remove(clients.begin(),clients.end(),conn),clients.end());

                 }
                 fputs(buf,stdout);
                 write(conn,buf,strlen(buf));

            }

        }

    }
    return 0;
}



