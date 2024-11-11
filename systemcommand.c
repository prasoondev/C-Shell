#include <sys/wait.h>
#include <sys/types.h>
#include<stdio.h>
#include"execute.h"
#include<unistd.h>
#include<time.h>
#include<string.h>
#include<stdlib.h>
#include <sys/time.h>
#define max 4096
// char** gettoken(char* command){
//     char** toret;
//     toret=(char**)malloc(sizeof(char*)*max);
//     for(int i=0;i<max;i++){
//         toret[i]=(char*)malloc(sizeof(char)*max);
//     }
//     char* token=strtok(command," ");
//     int i=0;
//     while(token!=NULL){
//         strcpy(toret[i],token);
//         i++;
//         token=strtok(NULL," ");
//     }
//     toret[i]=NULL;
//     return toret;
// }
long foreground(char *tolog){
    long timer=0;
    int rc=fork();
    int s=1;
    struct timeval start;
    gettimeofday(&start, NULL);
    if(rc<0){
        printf("Internal error.\n");
    }
    else if(rc==0){
        freechild();
        // char temp[max];
        // strcpy(temp,tolog);
        // char *token=strtok(temp," ");
        char *com[max];
        int i=0;
        char *token=strtok(tolog," ");
        while(token!=NULL){
            com[i]=token;
            i++;
            token=strtok(NULL," ");
        }
        com[i]=NULL;
        // char** com=gettoken(tolog);
        // char *args[]={"sh","-c",tolog,NULL};
        execvp(com[0],com);
        exit(1);
    }
    else{
        int w=waitpid(rc,&s,0);
    }
    if(s==1){
        printf("Command failed.\n");
    }
    struct timeval end;
    gettimeofday(&end, NULL);
    long seconds,useconds;
    seconds  = end.tv_sec  - start.tv_sec;
    useconds = end.tv_usec - start.tv_usec;
    timer = seconds + (useconds >= 500000 ? 1 : 0);
    if(timer>=2){
        return timer;
    }
    return 0;
}
void background(char *tolog,char *c){
    int rc=fork();
    if(rc<0){
        printf("Internal error.\n");
    }
    if(rc==0){
        int rc1=fork();
        int status;
        if(rc1<0){
            printf("Internal error.\n");
        }
        if(rc1==0){
            freechild();
            // printf("hereback %d\n",getpid());
            // fflush(stdout); 
            // char *args[]={"sh","-c",tolog,NULL};
            // execvp("sh",args);
            // char** com=gettoken(tolog);
            // char *args[]={"sh","-c",tolog,NULL};
            char *com[max];
            int i=0;
            char *token=strtok(tolog," ");
            while(token!=NULL){
                com[i]=token;
                i++;
                token=strtok(NULL," ");
            }
            com[i]=NULL;
            execvp(com[0],com);
            exit(1);
        }
        else{
            int w=waitpid(rc1,&status,0);
            if(status==0){
                printf("%s exited normally (%d)\n",c,getpid());
            }
            else{
                printf("%s did not exit normally (%d)\n",c,getpid());
            }
        }
        freechild();
        exit(0);
    }
    else{
        printf("%d\n",rc);
    }
}