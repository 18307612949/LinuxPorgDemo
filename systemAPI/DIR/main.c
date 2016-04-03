#include "common.h"

using namespace std;

int main(int argc,char *argv[])
{

    //列出文件目录，类似"ls"
    if(argc<2)
    {
        cerr<<"Usage: "<<argv[0]<<" <directory> "<<endl;
        exit(EXIT_FAILURE);
    }

    DIR* dir=opendir(argv[1]);
    if(dir==NULL)
        ERR_EXIT("opendir error");
    struct dirent *ent;
    while((ent=readdir(dir))!=NULL)
    {
         if(ent->d_name[0]=='.')
             continue;
         cout<<ent->d_name<<"\ti-node: "<<ent->d_ino
             <<", length: "<<ent->d_reclen<<endl;
    }
    closedir(dir);

    int fd=open("text.txt",O_WRONLY|O_TRUNC);
    int dupfd=dup2(fd,1);
    cout<<"dupfd= "<<dupfd<<endl;

    return 0;

}
