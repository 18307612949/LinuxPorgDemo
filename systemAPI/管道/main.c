#include "common.h"

using namespace std;

int main()
{
    int pipefd[2];
    if(pipe(pipefd)==-1)
        ERR_EXIT("pipe error");

    pid_t pid=fork();
    if(pid==-1)
        ERR_EXIT("fork error");
    if(pid==0)
    {
        close(pipefd[0]);
        dup2(pipefd[1],STDOUT_FILENO);//ls输出至管道
        close(pipefd[1]);

        execlp("/bin/ls","ls",NULL);
        cerr<<"child execlp error"<<endl;
        exit(EXIT_FAILURE);
    }

    close(pipefd[1]);
    dup2(pipefd[0],STDIN_FILENO);
    close(pipefd[0]);
    execlp("/usr/bin/wc","wc","-w",NULL);
    cerr<<"parent execlp error"<<endl;
    exit(EXIT_FAILURE);
}
