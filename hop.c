#include<unistd.h>
#include<stdio.h>
#include"getinfo.h"
#include"log.h"
#include<string.h>
#include"execute.h"
#define max 4096
char prev[max];
void initialiseprev(){
    getDirectoryName(prev);
}
void changeloc(char *arr,int buff){
    char temp[max];
    temp[0]=0;
    int j=0;
    for(int i=1+buff;i<strlen(arr);i++){
        temp[j]=arr[i];
        j++;
    }
    temp[j]=0;
    strcpy(arr,temp);
}
int hop(char **arr,int n,char *initialdir){
    char curdir[max];
    getDirectoryName(curdir);
    char temp[max];
    int check=1;
    if(n==1){
        strcpy(temp,prev);
        getDirectoryName(prev);
        if(chdir(initialdir)==0){
            getDirectoryName(temp);
            printf("%s\n",temp);
        }
        else{
            check=0;
            strcpy(prev,temp);
            printf("%s: No such file or directory\n",initialdir);
        }
    }
    else{
        for(int i=1;i<n;i++){
            if(strcmp(arr[i],"~")==0){
                strcpy(temp,prev);
                getDirectoryName(prev);
                if(chdir(initialdir)==0){
                    getDirectoryName(temp);
                    printf("%s\n",temp);
                }
                else{
                    check=0;
                    strcpy(prev,temp);
                    perror(arr[i]);
                }
            }
            else if(strcmp(arr[i],".")==0){
                getDirectoryName(temp);
                strcpy(prev,temp);
                printf("%s\n",temp);
            }
            else if(strcmp(arr[i],"-")==0){
                strcpy(temp,prev);
                getDirectoryName(prev);
                if(chdir(temp)==0){
                    getDirectoryName(temp);
                    printf("%s\n",temp);
                }
                else{
                    check=0;
                    strcpy(prev,temp);
                    perror(arr[i]);
                }
            }
            else if(arr[i][0]=='~'){
                strcpy(temp,prev);
                getDirectoryName(prev);
                chdir(initialdir);
                changeloc(arr[i],1);
                if(chdir(arr[i])==0||(strcmp(arr[i],"")==0)){
                    getDirectoryName(temp);
                    printf("%s\n",temp);
                }
                else{
                    strcpy(prev,temp);
                    chdir(curdir);
                    check=0;
                    perror(arr[i]);
                }
            }
            else if(arr[i][0]=='-'){
                strcpy(temp,prev);
                getDirectoryName(prev);
                chdir(temp);
                changeloc(arr[i],1);
                if(chdir(arr[i])==0||(strcmp(arr[i],"")==0)){
                    getDirectoryName(temp);
                    printf("%s\n",temp);
                }
                else{
                    strcpy(prev,temp);
                    chdir(curdir);
                    check=0;
                    perror(arr[i]);
                }
            }
            else{
                strcpy(temp,prev);
                getDirectoryName(prev);
                // printf("%s\n",arr[i]);
                // chdir("/home/prasoon/Desktop/code/osn/test\n");
                if(chdir(arr[i])==0){
                    getDirectoryName(temp);
                    printf("%s\n",temp);
                    // writefile(arr[0],arr[i]);
                }
                else{
                    strcpy(prev,temp);
                    // chdir(curdir);
                    check=0;
                    perror(arr[i]);
                    // printf("%s: No such file or directory\n",arr[i]);
                    // return 1;
                }
            }
        }
    }
    return check;
}
int hopchild(char *arr,char *initialdir){
    char curdir[max];
    getDirectoryName(curdir);
    if(strcmp(arr,"~")==0){
        chdir(initialdir);
    }
    else if(strcmp(arr,".")==0){
        return 0;
    }
    else if(strcmp(arr,"-")==0){
        if(chdir(prev)==0){
            return 0;
        }
        else{
            return 1;
        }
    }
    else if(arr[0]=='~'){
        chdir(initialdir);
        changeloc(arr,1);
        if(chdir(arr)==0||(strcmp(arr,"")==0)){
            return 0;
        }
        else{
            return 1;
        }
    }
    else if(arr[0]=='-'){
        chdir(prev);
        changeloc(arr,1);
        if(chdir(arr)==0||(strcmp(arr,"")==0)){
            return 0;
        }
        else{
            return 1;
        }
    }
    else{
        if(chdir(arr)==0){
            return 0;
        }
        else{
            return 1;
        }
    }
    return 0;
}