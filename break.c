#include "headers.h"
#include "commands.h"
#include "list.h"

//printing error message
void check_error(int ret)
{
	if(ret==-1)
		perror("Error");
}

//conversion of string to integer
int convert_to_int(char a[100005])
{
	int n,i,s=0;
	n=strlen(a);
	for(i=0;i<n;i++)
	{
		s=s*10;
		s=s+(int)(a[i]-48);

	}
	return s;
}

//checking whether command has &,<,>,>> and replacing & by null value,placing spaces before and after <,>,>>
void check_bg_red(int *background,int *leftarrow,int *rightarrow,int *append,char ar[100][1000],int k,char piped[100][1000])
{
	if(piped[k][strlen(ar[k])-1]=='&')
	{
		piped[k][strlen(ar[k])-1]='\0';
		*background=1;
	}
	int i;
	char b[1000][1000];
	int pos=0;
	for(i=0;i<strlen(piped[k]);i++)
	{
		if(piped[k][i]=='<')
		{
			b[k][pos]=' ';
			b[k][pos+1]='<';
			b[k][pos+2]=' ';
			pos=pos+3;
		}
		else if(piped[k][i]=='>' && piped[k][i+1]!='>' && i+1<strlen(piped[k]))
		{
			b[k][pos]=' ';
			b[k][pos+1]='>';
			b[k][pos+2]=' ';
			pos+=3;
		}
		else if(piped[k][i]=='>' && piped[k][i+1]=='>' && i+1<strlen(piped[k]))
		{
			b[k][pos]=' ';
			b[k][pos+1]='>';
			b[k][pos+2]='>';
			b[k][pos+3]=' ';
			pos+=4;
			i++;
		}
		else
		{
			b[k][pos]=piped[k][i];
			pos++;
		}
	}
	for(i=0;i<pos;i++)
	{
		piped[k][i]=b[k][i];
	}
	piped[k][i]='\0';
}

//break command at semicolon,further breaking at pipe,further breaking it at spaces
node* break_at_semicolon(char *a,char *commands,char ar[100][1000],char *first[1000],char piped[100][1000],char *start,int *last,int pushed[1000],char name[100][1000],node* head,int *exit_status)
{
	char *pch;
	char *ps;
	char *pi;
	int c=0,ret,k;
	//strtok with ;
	pch=strtok(commands,";");
	while(pch!=NULL)
	{
		strcpy(ar[c],pch);
		pch=strtok(NULL,";");
		c++;
	}
	for(k=0;k<c;k++)
	{
		int piping[2],fd_in=0;
		//strtok with pipe(|)
		pi=strtok(ar[k],"|");
		int p=0,l;
		while(pi!=NULL)
		{
			strcpy(piped[p],pi);
			pi=strtok(NULL,"|");
			p++;
		}
		for(l=0;l<p;l++)
		{
			int background=0,fg=0,rightarrow=0,append=0,leftarrow=0,flag1=0,flag2=0,execution=0;
			char* output;
			for(int z=0;z<strlen(piped[l]);z++)
			{
				if(piped[l][z]=='>' && z+1<strlen(piped[l]) && piped[l][z+1]=='<')
					execution=2;
				if(piped[l][z]=='<' && z+1<strlen(piped[l]) && piped[l][z+1]=='>' )
					execution=1;
				if(piped[l][z]=='<' && z+1<strlen(piped[l]) && piped[l][z+1]=='<' )
					execution=1;
				if(piped[l][z]=='\'' || piped[l][z]=='"')
					piped[l][z]=' ';
			}
			check_bg_red(&background,&leftarrow,&rightarrow,&append,ar,l,piped);
			int i=0,fd,fd1=0,getout=0;
			int flag=0,instance=0,app=0,ap[2]={0};
			char *filename;
			//strtok with spaces and tabs
			ps=strtok(piped[l]," \t");
			stop=0;
			char* copy;
			while(ps!=NULL)
			{
				copy=ps;
				//found >
				if(strcmp(copy,">")==0 && execution==0)
				{
					first[i]=NULL;
					i++;
					rightarrow=1;
					ps=strtok(NULL," ");
					first[i]=ps;
					i++;
					fd = open(first[i-1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
					flag1=1;
					app=i-1;
				}
				//found >>
				else if(strcmp(copy,">>")==0 && execution==0)
				{
					rightarrow=1;
					append=1;
					first[i]=NULL;
					i++;
					ps=strtok(NULL," ");
					first[i]=ps;
					i++;
					fd = open(first[i-1],O_WRONLY | O_CREAT | O_APPEND, 0644);
					flag1=1;
					app=i-1;
				}
				//found <
				else if(strcmp(copy,"<")==0 && execution==0)
				{
					instance=i;
					first[i]=NULL;
					i++;
					leftarrow=1;
					ps=strtok(NULL," ");
					first[i]=ps;
					i++;
					fd1 = open(first[i-1], O_RDONLY, 0644);
					filename=first[i-1];

					if(flag2==0)
						ap[0]=i-1;
					else
						ap[1]=i-1;
					flag2=1;
				}
				//found &
				else if(strcmp(copy,"&")==0)
				{
					background=1;
				}
				else
				{
					first[i]=ps;
					i++;
				}
				ps=strtok(NULL," \t");
			}
			first[i]=NULL;
			for(int w=0;w<i-1;w++)
			{
				if(first[w]==NULL && first[w+1]==NULL)
					getout=1;
			}
			str[0]=first[0];
			if(app+1<i && first[app+1]==NULL && flag1==1) 
				execution=1;
			if(ap[0]+1==ap[1])
			{
				execution=2;
			}
			if(first[0]!=NULL && execution==0 && fd1!=-1 && getout==0)
			{
				if(strcmp(first[0],"quit")==0)
				{
					head=command_killallbg(head);
					printf("Good Bye\n");
					exit(0);
				}
				if(leftarrow==1 && i-2<0)
					printf(ANSI_COLOR_RED "Command not found\n" ANSI_COLOR_RESET);
				else
				{
					if(strcmp(first[0],"cd")==0)
					{
						commands_cd(a,first,start,i);
					}
					else
					{
						if(strcmp(first[0],"pwd")==0 && rightarrow!=1)
						{
							command_pwd();
						}
						else if(strcmp(first[0],"echo")==0 && rightarrow!=1 && append!=1 && leftarrow!=1) 
						{
							command_echo(commands);
						}
						else if(strcmp(first[0],"pinfo")==0)
						{
							command_pinfo(first);
						}

						else if(strcmp(first[0],"jobs")==0)
						{
							head=command_jobs(head);
						}
						else if(strcmp(first[0],"kjob")==0)
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

						}
						else if(strcmp(first[0],"fg")==0)
						{
							head=command_fg(first,head);
						}
						else if(strcmp(first[0],"killallbg")==0)
						{
							head=command_killallbg(head);
						}
						else
						{
							int wa,return_value,rr;
							rr=pipe(piping);
							int pid=fork();

							if(pid==-1)
								printf(ANSI_COLOR_RED "Fork failed\n"ANSI_COLOR_RESET);
							//child process
							else if(pid==0)
							{
								//for <
								if(flag2!=1)
									dup2(fd_in,0);
								else
								{
									dup2(fd1,0);	
								}
								//for > or >>(opened the files above as only one case at  a time is possible,return file descriptor is fd
								if(flag1==1)
								{
									dup2(fd,1);
								}
								else
								{
									dup2(fd_in,0);
								}
								//length of pipe is greater than 1 and present pipe position is not last
								if(l!=p-1 && p>=2)
								{
									rr=dup2(piping[1],1);
								}
								close(piping[0]);
								//closing the required files
								if(flag2==1)
									close(fd1);
								if(flag1==1)
									close(fd);
								flag2=0;
								flag1=0;
								//executing command using execvp
								return_value=execvp(first[0],first);
								printf(ANSI_COLOR_RED "%s :Command not found\n" ANSI_COLOR_RESET,first[0]);
								exit(127);
							}
							else
							{
								processid=pid;
								//there process must not run in background
								if(background==0)
								{
									int w;
									waitpid(pid,&w,WUNTRACED);
								//	while(wait(NULL)!=pid)
								//		continue;
								}
								//inorder for a process to run in background
								else
								{
									head=addnode(head,first,pid,0);
								}
								close(piping[1]);
								fd_in=piping[0];

							}
							if(return_value!=0)
								printf(ANSI_COLOR_RED "Command not found\n" ANSI_COLOR_RESET);

						}
					}
				}
			}
			else if(fd1==-1)
			{
				printf(ANSI_COLOR_RED "-bash: %s: No such file or directory\n" ANSI_COLOR_RESET,filename);
			}
			else if(execution==1)
			{
				printf(ANSI_COLOR_RED "-bash: syntax error near unexpected token `newline'\n" ANSI_COLOR_RESET);
			}
			else if(execution==2)
			{
				printf(ANSI_COLOR_RED "-bash: syntax error near unexpected token `<'\n" ANSI_COLOR_RESET);
			}
			else
				printf(ANSI_COLOR_RED "Command not found\n" ANSI_COLOR_RESET);
			//if ctrl-z is pressed,add process in linked list with type as 1	
			if(stop==1)
			{
				head=addnode(head,str,processid,1);
				stop=0;
			}
		}
		if(*exit_status==1)
		{
			break;
		}
	}
	return head;
}

	

void handle_int(int sig)
{
	if (sig==SIGINT)
	{
		printf ("\nBreak Encountered...Exiting\n");
	}
	//for ctrl-d
	if (sig==SIGMINE)
	{
		printf ("\nExiting the shell\n");
	}
		exit(1);
}
void ctrl_z_handler(int sig)
{
	//when ctrl z is pressed, make stop as 1
	stop=1;
}
void ctrl_c_handler(int sig)
{
	printf("\n");
	//printing prompt
	if(singleline==0)
	prompt(a,b,start);
	fflush(stdout);
}

