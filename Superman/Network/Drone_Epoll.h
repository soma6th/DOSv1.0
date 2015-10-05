#include "TCP_Server.h"  /* TCP echo server includes */
#include <sys/time.h>       /* for struct timeval {} */
#include <sys/wait.h>		//wait pid function
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>          /* for fcntl() */
#include <unistd.h>
#include <signal.h>			//signal handling

#define EPOLL_SIZE 5

int epoll_init();
int epoll_run();
