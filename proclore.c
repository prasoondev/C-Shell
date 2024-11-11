#include<stdio.h>
#include<unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>
#include<string.h>
#include"execute.h"
#include<stdlib.h>
#include"hop.h"
#define max 4096
void getstate(char *buffer){
    int fd=open("stat",O_RDONLY,0644);
    if(fd==-1){
        printf("Problem opening file.\n");
        return;
    }
    int bytes=0;
    bytes=read(fd,buffer,max-1);
    if(bytes<=0){
        printf("Error reading the file.\n");
    }
    else{
        char *token=strtok(buffer," ");
        int i=1;
        while(token!=NULL){
            token=strtok(NULL," ");
            i++;
            if(i==3){
                printf("process status : %s",token);
                return;
            }
        }
        printf("Error getting state.\n");
    }
}
void getgroup(char *buffer){
    int fd=open("stat",O_RDONLY,0644);
    if(fd==-1){
        printf("Problem opening file.\n");
        return;
    }
    int bytes=0;
    bytes=read(fd,buffer,max-1);
    if(bytes<=0){
        printf("Error reading the file.\n");
    }
    else{
        char *token=strtok(buffer," ");
        int i=1;
        while(token!=NULL){
            token=strtok(NULL," ");
            i++;
            if(i==5){
                int fg_pgid=tcgetpgrp(STDIN_FILENO);
                char fg_indicator=(atoi(token)==fg_pgid)?'+':'\0';
                printf("%c\n",fg_indicator);
                printf("Process Group : %s\n",token);
                return;
            }
        }
        printf("Error getting group.\n");
    }
}
void getmemory(char *buffer){
    int fd=open("status",O_RDONLY,0644);
    if(fd==-1){
        printf("Problem opening file.\n");
        return;
    }
    int bytes=0;
    bytes=read(fd,buffer,max-1);
    if(bytes<=0){
        printf("Error reading the file.\n");
    }
    else{
        char *token=strtok(buffer,"\n");
        int i=1;
        while(token!=NULL){
            token=strtok(NULL,"\n");
            i++;
            if(i==19){
                char *token2=strtok(token," ");
                token2=strtok(NULL," ");
                printf("Virtual memory : %s\n",token2);
                return;
            }
        }
        printf("Error getting memory size.\n");
    }
}
void getpath(char *buffer){
    int bytes=0;
    bytes=readlink("exe",buffer,max-1);
    buffer[bytes]=0;
    if(bytes<0){
        printf("Error reading path.\n");
    }
    else{
        printf("executable Path : %s\n",buffer);
    }
}
void printinfo(int id){
    printf("pid : %d\n",id);
    char buffer[max];
    getstate(buffer);
    getgroup(buffer);
    getmemory(buffer);
    getpath(buffer);
}
void sendchild(int id){
    int rc=fork();
    if(rc<0){
        printf("Internal error.\n");
        return;
    }
    else if(rc==0){
        freechild();
        char path[max];
        sprintf(path,"/proc/%d",id);
        int n=hopchild(path,path);
        if(n==1){
            printf("Invalid argument.\n");
        }
        else{
            printinfo(id);
        }
        exit(0);
    }
    else{
        int w=waitpid(rc,NULL,0);
    }
}
void proc(char **arr,int n){
    if(n==1){
        sendchild(getpid());
    }
    else{
        int temp;
        temp=atoi(arr[1]);
        sendchild(temp);
        // for(int i=1;i<n;i++){
        //     temp=atoi(arr[i]);
        //     sendchild(temp);
        //     // if(temp<=0){
        //     //     printf("Invalid PID");
        //     // }
        //     // else{
        //     //     printinfo(temp);
        //     // }
        // }
    }
}