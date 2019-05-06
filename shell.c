#include "headers.h"
#include "break.h"
#include "list.h"
#include "commands.h"
int main()
{
	char* cwd;
	char piped[100][1000];
	char *first[1000];

	char ar[100][1000];
	char commands[1000];
	char name[100][1000];
//	node* head=NULL;
	int i,top=-1,pushed[1000];
	cwd = getcwd( buff, 2000 );

	start=cwd;
	int exit_status=0;
	signal(SIGINT,ctrl_c_handler);
	signal(SIGTSTP,ctrl_z_handler);
	signal(SIGMINE,handle_int);
	while(1)
	{
		singleline=0;
		//printing prompt
		prompt(a,b,start);
		//for ctrl-d
		if(fgets(commands,200,stdin)==NULL)
			break;
		if(strlen(commands)!=0)
			singleline=1;
		commands[strlen(commands)-1]='\0';
		//breaking commands
		head=break_at_semicolon(a,commands,ar,first,piped,start,&top,pushed,name,head,&exit_status);
		//exiting the process
		head=exit_bg(head);
		if(exit_status==1)
			break;
	}
	printf("Exiting the shell\n");
	return 0;
}
