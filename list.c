#include "headers.h"
#include "list.h"
//adding node to the linked list
node* addnode(node* head,char *first[100005],int pid,int type)
{
	if(head==NULL)
	{
		head=malloc(sizeof(node));
		head->pid=pid;
		head->type=type;
		int i=0;
		strcpy(head->name,first[0]);
		head->next=NULL;
	}
	else
	{
		node* temp;
		temp=malloc(sizeof(node));
		temp->pid=pid;
		temp->type=type;
		int i=0;
		strcpy(temp->name,first[0]);
		node* temp1;
		temp1=head;
		while(temp1->next!=NULL)
		{
			temp1=temp1->next;
		}
		temp1->next=temp;
		temp->next=NULL;
	}
	return head;
}
//traversing through linked list
int traverse(node* head,int pd)
{
	int c=0,found=0,p;
	node* t;
	t=head;
	t=head;
	while(t!=NULL)
	{
		c++;
		if(c==pd)
		{
			found=1;
			p=t->pid;
			overalltype=t->type;
		}
		t=t->next;
	}
	if(found==0)
		return -1;
	else
		return p;
}
node* exit_bg(node* head)
{
	int stat;
	node* temp;
	temp=head;
	while(temp!=NULL)
	{
		//killing process when it is exited
		if(waitpid(temp->pid,&stat,WNOHANG))
		{
			kill(temp->pid,SIGKILL);
			head=delete(head,temp);
		}
		temp=temp->next;
	}
	return head;
}

//delete a node in linked list
node* delete(node* head,node* temp)
{
	node* temp1;
	temp1=head;
	if(head==temp)
	{
		printf(ANSI_COLOR_BLUE "%s with pid %d exited normally\n" ANSI_COLOR_RESET,head->name,head->pid);
		head=head->next;
	}
	else
	{
		while(temp1->next!=temp)
			temp1=temp1->next;
		printf(ANSI_COLOR_BLUE "%s with pid %d exited normally\n" ANSI_COLOR_RESET,temp->name,temp->pid);
		temp1->next=temp->next;
	}
	return head;
}
