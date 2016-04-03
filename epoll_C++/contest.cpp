#include "commen.h"


int main()
{
    if (signal(SIGSEGV, SIG_IGN) == SIG_ERR)
        err_exit("signal error");
    //最好不要修改: 不然会产生段溢出(stack-overflow)
    struct rlimit rlim;
    rlim.rlim_cur = 2048;
    rlim.rlim_max = 2048;
    if (setrlimit(RLIMIT_NOFILE, &rlim) == -1)
        err_exit("setrlimit error");

    int count = 0;
    while (true)
    {
        int sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd == -1)
        {
            sleep(5);
            err_exit("socket error");
        }

        struct sockaddr_in serverAddr;
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = htons(8001);
        serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
        int ret = connect(sockfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr));
        if (ret == -1)
            err_exit("connect error");

        //获取并打印对端信息
        struct sockaddr_in peerAddr;
        socklen_t peerLen = sizeof(peerAddr);
        if (getpeername(sockfd, (struct sockaddr *)&peerAddr, &peerLen) == -1)
            err_exit("getpeername");
        cout << "Server information: " << inet_ntoa(peerAddr.sin_addr)
             << ", " << ntohs(peerAddr.sin_port) << endl;
        cout << "count = " << ++count << endl;
    }
}
