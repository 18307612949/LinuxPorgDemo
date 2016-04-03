#ifndef _COMMON_H_
#define _COMMON_H_

#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <shadow.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/mman.h>
#include <sys/socket.h>
#include <crypt.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <pwd.h>
#include <signal.h>
#include <linux/capability.h>
#include <sys/syscall.h>
#include <sys/sendfile.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <sys/stat.h>
#include <dirent.h>
#include <sys/time.h>


#define ERR_EXIT(m) \
    do\
    {\
       perror(m);\
       exit(EXIT_FAILURE);\
    } \
    while(0)
#endif
