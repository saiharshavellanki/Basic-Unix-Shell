#ifndef _commands_H
#define _commands_H
void prompt(char* a,char* b,char* start);
void command_pwd();
void command_echo(char *commands);
void commands_cd(char *a,char *first[1000],char *start,int i);
void command_pinfo(char *first[1000]);
node* command_jobs(node* head);
node* command_kjob(char *first[1000],node* head);
node* command_fg(char *first[1000],node* head);
node* command_killallbg(node* head);
#endif
