#include "headers.h"
#include "commands.h"
#include "list.h"
#include "break.h"

//printing the prompt
void prompt(char* a,char* b,char* start)
{
	strcpy(a,"<");
	struct passwd *passwd;
	passwd = getpwuid ( getuid()); 
	strcpy(b,passwd->pw_name);
	strcat(a,b);
	char hostname[HOST_NAME_MAX];
	strcat(a,"@");
	gethostname(hostname,100);
	strcat(a,hostname);
	strcat(a,":");
	char* cwd;
	char buff[2000];
	cwd = getcwd( buff, 2000 );
	int c=0,i,flag=0,count=0,f=0;
	for(i=0;i<strlen(buff);i++)
	{
		if(start[i]!=buff[i])
		{
			flag=1;
		}
		else
		{
			count++;
		}
	}
	if(count!=strlen(buff))
	{
		f=1;
		flag=0;
	}
	if(strlen(buff)<strlen(start))
		flag=1;
	if(flag==1)
	{
		strcat(a,buff);
	}
	else
	{
		strcat(a,"~");
		if(f==1)
		{
			strcat(a,"/");
			strcat(a,&buff[strlen(start)+1]);
		}
	}
	strcat(a,">");
	printf(ANSI_COLOR_GREEN "%s" ANSI_COLOR_RESET,a);
}
void command_pwd()
{
	char* cwd;
	char buff[2000];
	cwd = getcwd( buff, 2000 );
	printf("%s\n",buff);
}
void command_echo(char *commands)
{
	int j=4,state=0;
	while(commands[j]==' ' && j<strlen(commands))
		j++;
	while(j<strlen(commands))
	{
		if(state==1)
		{
			while(commands[j]!='"' && j<strlen(commands))
			{
				printf("%c",commands[j]);
				j++;
				if(commands[j]=='"')
					printf(" ");
			}
			if(commands[j]=='"' && j<strlen(commands))
			{
				state=0;
				j++;
			}
		}
		if(commands[j]=='"' && j<strlen(commands) && state==0)
		{
			state=1;
			j++;
		}
		if(state==0)
		{
			while(commands[j]==' ')
			{
				j++;
				if(commands[j]!=' ' && commands[j]!='"')
					printf(" ");
			}
			if(commands[j]=='"')
			{
				state=1;
				j++;
			}
			else
			{
				printf("%c",commands[j]);
				j++;
			}
		}
	}
	printf("\n");					
}
void commands_cd(char *a,char *first[1000],char *start,int i)
{
	int ret;

	if(i>1)
	{
		strcpy(a,"");
		int fl=0;
		if(first[1][0]=='~' && strlen(first[1])>1)
		{
			fl=1;
			strcat(a,start);
			strcat(a,&first[1][1]);
		}
		if(fl==1)
		{
			ret=chdir(a);
		}
		else if(first[1][0]=='~')
		{
			ret=chdir(start);
		}
		else
		{
			ret=chdir(first[1]);
		}
		if(ret<0)
			perror(ANSI_COLOR_RED "Error" ANSI_COLOR_RESET);
	}
	else
	{
		chdir(start);
	}
}
void command_pinfo(char *first[1000])
{
	if(first[1]!=NULL)
	{
		//process status is present in /proc/pid/status
		printf("pid -- %s\n",first[1]);
		char presentpath[1000],lines[1000],executable[1000];
		int i,pos;
		strcat(presentpath,"/proc/");
		strcat(presentpath,first[1]);
		strcat(presentpath,"/status");

		FILE *fp=fopen(presentpath,"rt");
		if(fp!=NULL)
		{
			fgets(lines,1000,fp);
			fgets(lines,1000,fp);

			printf("Process Status -- ");
			for(i=7;i<strlen(lines);i++)
			{
				if(lines[i]==' ')
					break;
				else
					printf("%c",lines[i]);
			}
			printf("\n");
			//executable path is present in /proc/pid/exe
			strcpy(presentpath,"/proc/");
			strcat(presentpath,first[1]);
			strcat(presentpath,"/exe");
			for(i=0;i<1000;i++)
				executable[i]='\0';
			readlink(presentpath,executable,1000);
			printf("Executable Path -- %s\n",executable);
		}
		else
			printf(ANSI_COLOR_YELLOW "No process found\n" ANSI_COLOR_RESET);
	}
	else
		printf(ANSI_COLOR_YELLOW "pass pid for process\n" ANSI_COLOR_RESET);
}
node* command_jobs(node* head)
{

	int len=1;
	node* t;
	t=head;
	while(t!=NULL)
	{
		printf(ANSI_COLOR_BLUE "[%d] %s %d\n" ANSI_COLOR_RESET,len,t->name,t->pid);
		len++;
		t=t->next;
	}
	return head;
}
node* command_kjob(char *first[1000],node* head)
{

	int pd,sig,retstatus,p;
	char a[10005];
	if(first[1]!='\0' && first[2]!='\0')
	{
		strcpy(a,first[1]);
		pd=convert_to_int(a);
		p=traverse(head,pd);

		strcpy(a,first[2]);
		sig=convert_to_int(a);

		if(p!=-1)
		{
			retstatus=kill(p,sig);
			{
				if(retstatus==-1)
					perror(ANSI_COLOR_RED "Error" ANSI_COLOR_RESET);
			}
		}
		else
			printf(ANSI_COLOR_YELLOW "No job found\n" ANSI_COLOR_RESET);
	}
	else
		printf(ANSI_COLOR_YELLOW "Command must have 2 arguments\n" ANSI_COLOR_RESET);
	return head;
}
node* command_fg(char *first[1000],node* head)
{
	int stat,pd,p;
	char b[10005];
	strcpy(b,first[1]);
	pd=convert_to_int(b);
	p=traverse(head,pd);
	//when process is directly sent to background
	if(p!=-1 && overalltype==0)
		waitpid(p,&stat,0);
	//getting process which is stopped by ctrl-z
	else if(p!=-1 && overalltype==1)
	{
		kill(p,SIGCONT);
		waitpid(p,&stat,0);
	}
	else
		printf(ANSI_COLOR_YELLOW "No job found\n" ANSI_COLOR_RESET);
	return head;
}
node* command_killallbg(node* head)
{

	int l=0;
	node* killbg;
	killbg=head;
	while(killbg!=NULL)
	{
		l++;
		kill(killbg->pid,9);
		printf(ANSI_COLOR_BLUE "[%d] %s %d killed\n" ANSI_COLOR_RESET,l,killbg->name,killbg->pid);
		killbg=killbg->next;
	}
	head=NULL;
	return head;
}

