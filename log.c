#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include"hop.h"
#include <sys/wait.h>
#include<stdlib.h>
#include"getinfo.h"
#include"getcommand.h"
#include"execute.h"
#include"getcommand.h"
#include<string.h>
#define loglimit 15
#define max 4096
char location[max];
char queue[loglimit][max];
int count=0;
int l=0;
int r=0;
void clearfile(){
    int fd=open(location,O_WRONLY|O_TRUNC|O_CREAT,0644);
    if(fd==-1){
        printf("Problem opening file. <Recreating the file>\n");
        return;
    }
    close(fd);
}
void clearqueue(){
    l=0;
    r=0;
    count=0;
}
void addqueue(char *command){
    if(count>0&&(strcmp(queue[(r-1+loglimit)%loglimit],command)==0)){
        return;
    }
    // printf("%ld %ld %d\n",strlen(command),strlen(queue[(r-1+loglimit)%loglimit]),count);
    strcpy(queue[r],command);
    r=(r+1)%loglimit;
    if(count<loglimit){
        count++;
    }
    else{
        l=(l+1)%loglimit;
    }
}
void writefile(char *command){
    addqueue(command);
    int cur=l;
    clearfile();
    int fd=open(location,O_CREAT|O_WRONLY,0644);
    if(fd==-1){
        printf("Error while opening log file.\n");
    }
    for(int i=0;i<count;i++){
        write(fd,queue[cur],strlen(queue[cur]));
        write(fd,"\n",1);
        cur=(cur+1)%loglimit;
    }
    close(fd);
}
void initialread(){
    int fd=open(location,O_RDONLY|O_CREAT,0644);
    if(fd==-1){
        printf("Problem opening file. <Recreating the file>\n");
        return;
    }
    int bytes=0;
    char arr[max*loglimit];
    while((bytes=read(fd,arr,max*loglimit))>0){
        arr[bytes-1]=0;
    }
    char *token=strtok(arr,"\n");
    while(token!=NULL){
        strcpy(queue[r],token);
        token=strtok(NULL,"\n");
        r=(r+1)%loglimit;
        if(count<loglimit){
            count++;
        }
        else{
            l=(l+1)%loglimit;
        }
    }
    close(fd);
}
void createlogfile(){
    getDirectoryName(location);
    char temp[max]="/logfile.txt";
    int k=strlen(location);
    for(int i=0;i<strlen(temp);i++){
        location[k]=temp[i];
        k++;
    }
    location[k]=0;
    int fd=open(location,O_CREAT,0644);
    if(fd==-1){
        printf("Error while opening log file.\n");
    }
    close(fd);
    initialread();
}
void readfile(){
    int fd=open(location,O_RDONLY|O_CREAT,0644);
    if(fd==-1){
        printf("Problem opening file. <Recreating the file>\n");
        return;
    }
    int bytes=0;
    char arr[max*loglimit];
    while((bytes=read(fd,arr,max*loglimit))>0){
        arr[bytes]=0;
        printf("%s\n",arr);
    }
    close(fd);
}
int logcommand(char **arr,int n,struct sttoprint **forename,int *count1,int *skip,struct stcommand *command,int index){
    if(n==1){
        readfile();
    }
    else if(strcmp(arr[1],"purge")==0){
        if(n==2){
            clearfile();
            clearqueue();
        }
        else{
            printf("Too many arguments.\n");
            return 1;
        }
    }
    else if(strcmp(arr[1],"execute")==0){
        if(n==3){
            int check=0;
            int number=0;
            for(int i=0;i<strlen(arr[2]);i++){
                if(arr[2][i]>'9'||arr[2][i]<'0'){
                    check=1;
                    break;
                }
                else{
                    number=number*10+arr[2][i]-'0';
                }
            }
            if(check==1||number==0||number>count){
                printf("Invalid argument.\n");
                return 1;
            }
            else{
                // printf("%s\n",queue[l+number-1]);
                char temp[max];
                getDirectoryName(temp);
                char temp2[max];
                int rf=r;
                while(number){
                    rf=rf-1;
                    if(rf==-1){
                        rf=loglimit-1;
                    }
                    number--;
                }
                strcpy(temp2,queue[rf]);
                struct stquerylist *commands=getCommands(temp2);
                struct stcommand *test=commands->command;
                free(commands);
                if(test!=NULL){
                    struct stcommand *tempc=command->next;
                    command->next=test;
                    struct stcommand *temphead=test;
                    while(temphead->next!=NULL){
                        temphead=temphead->next;                    
                    }
                    temphead->next=tempc;
                }
            }
        }
        else{
            printf("Invalid number of arguments.\n");
            return 1;
        }
    }
    else{
        printf("Invalid argument\n");
        return 1;
    }
    return 0;
}