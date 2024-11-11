#include<unistd.h>
#include<pwd.h>
#include<string.h>
#include<stdlib.h>
#define max 4096
void getUserName(char *username){
    uid_t uid=geteuid();
    struct passwd *pw=getpwuid(uid);
    if(pw){
        strcpy(username,pw->pw_name);
    }
}
const char *getSystemName(char *arr){
    arr[0]=0;
    gethostname(arr,max);
    return arr;
}
void getDirectoryName(char *arr){
    arr[0]=0;
    getcwd(arr,max);
    return;
}
void checkForInidir(char *curdir,char *inidir,char *temp){
    temp[0]=0;
    if(strlen(curdir)<strlen(inidir)){
        strcpy(temp,curdir);
        return;
    }
    for(int i=0;i<strlen(inidir);i++){
        if(curdir[i]!=inidir[i]){
            strcpy(temp,curdir);
            return;
        }
    }
    temp[0]='~';
    temp[1]=0;
    if(strcmp(curdir,inidir)==0){
        return;
    }
    int j=1;
    for(int i=strlen(inidir);i<strlen(curdir);i++){
        temp[j]=curdir[i];
        j++;
    }
    temp[j]=0;
    return;
}