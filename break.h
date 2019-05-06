#ifndef _break_H
#define _break_H
void check_error(int ret);
int convert_to_int(char a[100005]);
void check_bg_red(int *background,int *leftarrow,int *rightarrow,int *append,char ar[100][1000],int k,char piped[100][1000]);
node* break_at_semicolon(char *a,char *commands,char ar[100][1000],char *first[1000],char piped[100][1000],char *start,int *last,int pushed[1000],char name[100][1000],node* head,int *exit_status);
void handle_int(int sig);
void ctrl_z_handler(int sig);
void ctrl_c_handler(int sig);
#endif
