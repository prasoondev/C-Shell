#include<stdio.h>
#include<string.h>
#include<dirent.h>
#include<stdlib.h>
#include<sys/wait.h>
#include <fcntl.h>
#include"hop.h"
#include<unistd.h>
#include"getinfo.h"
#include"execute.h"
#include <sys/stat.h>
#define max 4096
char pathtoe[max];
int count2=0;
int checkfile(char *target,char *file){
    if(strlen(target)>strlen(file)){
        return 1;
    }
    int i;
    for(i=0;i<strlen(target);i++){
        if(target[i]!=file[i]){
            return 1;
        }
    }
    if(strlen(target)==strlen(file)){
        return 0;
    }
    // if(file[i]=='.'){
    //     return 0;
    // }
    return 0;
}
void updatepath(char *caller,char *target){
    char temp[max];
    int j=0;
    for(int i=0;i<strlen(target);i++){
        if(target[i]!=caller[i]){
            return;
        }
    }
    temp[j]='.';
    j++;
    // temp[j]='/';
    // j++;
    for(int i=strlen(target);i<strlen(caller);i++){
        temp[j]=caller[i];
        j++;
    }
    temp[j]=0;
    strcpy(caller,temp);
}
void listfiles(int f,int d,int e,char *target,char *path,char *callpath){
    // printf("f=%d d=%d e=%d target=%s path=%s\n",f,d,e,target,path);
    char path2[max];
    struct dirent *dp;
    DIR *dir=opendir(path);
    if(!dir){
        return;
    }
    while((dp=readdir(dir))!=NULL){
        if(strcmp(dp->d_name,".")!=0&&strcmp(dp->d_name,"..")){
            if(checkfile(target,dp->d_name)==0){
                // printf("%s/%s\n",path,dp->d_name);
                char temp[max];
                char temp2[max];
                struct stat file_stat;
                sprintf(temp,"%s/%s",path,dp->d_name);
                strcpy(temp2,temp);
                if(stat(temp,&file_stat)==0){            
                    updatepath(temp,callpath);    
                    if(d==1&&S_ISDIR(file_stat.st_mode)){
                        strcpy(pathtoe,temp2);
                        count2++;
                        printf("\x1b[34m%s \x1b[0m\n",temp);
                    }
                    else if(f==1&&!(file_stat.st_mode & S_IXUSR)){
                        strcpy(pathtoe,temp2);
                        count2++;
                        printf("\x1b[32m%s \x1b[0m\n",temp);
                    }
                    else if(d==0&&f==0){
                        // printf("%s/%s\n",path,dp->d_name);
                        if(S_ISDIR(file_stat.st_mode)){
                            strcpy(pathtoe,temp2);
                            count2++;
                            printf("\x1b[34m%s \x1b[0m\n",temp);
                        }
                        else if(file_stat.st_mode & S_IXUSR){
                            strcpy(pathtoe,temp2);
                            count2++;
                            printf("%s\n",temp);
                        }
                        else{
                            strcpy(pathtoe,temp2);
                            count2++;
                            printf("\x1b[32m%s \x1b[0m\n",temp);
                        }
                    }
                }
            }
            strcpy(path2,path);
            strcat(path2,"/");
            strcat(path2,dp->d_name);
            listfiles(f,d,e,target,path2,callpath);
        }
    }
    closedir(dir);
}
int seeksearch(char **arr,int n,char *dir){
    count2=0;
    strcpy(arr[n],"");
    int dirf=0;
    int file=0;
    int exe=0;
    if(n==1){
        printf("Invalid arguments.\n");
        return 1;
    }
    int check=1;
    int i;
    for(i=1;i<n&&check==1;i++){
        if(arr[i][0]=='-'&&(strcmp("-",arr[i])!=0)){
            for(int j=1;j<strlen(arr[i]);j++){
                if(arr[i][j]!='d'&&arr[i][j]!='f'&&arr[i][j]!='e'){
                    i--;
                    check=0;
                    break;
                }
                else if(arr[i][j]=='d'){
                    dirf=1;
                }
                else if(arr[i][j]=='f'){
                    file=1;
                }
                else if(arr[i][j]=='e'){
                    exe=1;
                }
            }
        }
        else{
            break;
        }
    }
    if(check==0||(dirf==1&&file==1)){
        file=0;
        dirf=0;
        exe=0;
        i--;
        printf("Invalid flags.\n");
        return 1;
    }
    // printf("%d %d\n",i+2,n);
    if(i+2<n||i==n){
        printf("Invalid number of arguments.\n");
        return 1;
    }
    char target[max];
    strcpy(target,arr[i]);
    i++;
    char path[max];
    strcpy(path,arr[i]);
    if(strcmp(path,"")==0){
        getDirectoryName(path);
    }
    else{
        int pipefd[2];
        char cwd[max];
        if(pipe(pipefd)==-1){
            perror("pipe");
            return 1;
        }
        int rc=fork();
        if(rc<0){
            printf("Internal error.\n");
            return 1;
        }
        else if(rc==0){
            freechild();
            close(pipefd[0]);
            int n=hopchild(path,dir);
            if(n==0){
                getDirectoryName(cwd);
                write(pipefd[1],cwd,strlen(cwd)+1);
                close(pipefd[1]);
                exit(0);
            }
            else{
                exit(1);
            }
        }
        else{
            close(pipefd[1]);
            read(pipefd[0],cwd,sizeof(cwd));
            close(pipefd[0]);
            int status=0;
            int w=waitpid(rc,&status,0);
            // printf("%s",cwd);
            if(status==0){
                strcpy(path,cwd);
            }
            else{
                printf("Invalid path.\n");
                return 1;
            }
        }
    }
    listfiles(file,dirf,exe,target,path,path);
    if(exe==1&&count2==1){
        // printf("%s\n",pathtoe);
        struct stat file_stat;
        if(stat(pathtoe,&file_stat)==0){
            if(S_ISDIR(file_stat.st_mode)){
                char *arrhop[max];
                arrhop[0]=NULL;
                arrhop[1]=pathtoe;
                int n=hop(arrhop,2,dir);
            }
            else if((!(file_stat.st_mode & S_IXUSR))){
                // printf("here");
                int fd=open(pathtoe,O_RDONLY,0644);
                if(fd==-1){
                    perror(pathtoe);
                }
                int bytes=0;
                char arrtoprint[max];
                bytes=read(fd,arrtoprint,max);
                if(bytes<0){
                    printf("Error reading file.\n");
                }
                arrtoprint[bytes]=0;
                printf("%s\n",arrtoprint);
            }
        }
    }
    if(count2==0){
        printf("No match found.\n");
    }
    return 0;
}