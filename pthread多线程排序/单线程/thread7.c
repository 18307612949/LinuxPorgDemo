#include <unistd.h>
#include <sys/time.h>
#include <pthread.h>
#include <string.h>
#include <string>
#include <cstdio>
#include <iostream>
#include <errno.h>
#include <stdlib.h>
#include <algorithm>
using namespace std;

//错误检查函数
inline void ERR_EXIT(const string &msg,int retnum)
{
    if(retnum!=0)
    {
        cerr<<msg<<": "<<strerror(retnum)<<endl;
        exit(EXIT_FAILURE);
    }
}
#define NUMMAX 8000000L
long int nums[NUMMAX];

int main()
{
    srandom(time(NULL));
    for(unsigned long i=0;i<NUMMAX;i++)
        nums[i]=random();

    struct timeval start,end;
    //计时开始
    gettimeofday(&start,NULL);
    sort(nums,nums+NUMMAX);
    gettimeofday(&end,NULL);

    //计算用时
    long long startusec=start.tv_sec*1000000+start.tv_usec;
    long long endusec=end.tv_sec*1000000+end.tv_usec;
    double elapsed=(double)(endusec-startusec)/1000000.0;
    printf("sort took %.4f seconds\n",elapsed);

    FILE *fp=fopen("save.txt","w+");
    for(unsigned long i=0;i<NUMMAX;i++)
        fprintf(fp,"%ld ",nums[i]);
    return 0;
}
