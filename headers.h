#ifndef _headers_H
#define _headers_H
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <pwd.h>
#include <netdb.h>
#include <limits.h>
#include <string.h>
#include <wait.h>
#include <stdlib.h>
#include <signal.h>
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

#define SIGMINE 30
char *start;
char buff[2000],a[1000],b[1000];
typedef struct stu{
	char name[100005];
	int pid;
	int type;
	struct stu* next;
}node;
char *str[100005];
node* head;
int processid,stop,overalltype,singleline;
#endif
