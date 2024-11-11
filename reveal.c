#include<stdio.h>
#include<dirent.h>
#include"hop.h"
#include <sys/wait.h>
#include <sys/types.h>
#include"execute.h"
#include <unistd.h>
#include<fcntl.h>
#include <string.h>
#include"getinfo.h"
#include<stdlib.h>
#include<grp.h>
#include<pwd.h>
#include"log.h"
#include<time.h>
#include <sys/stat.h>
#define max 4096
#define max_files 1024
void colorcoding(char *file){
    struct stat file_stat;
    if(stat(file,&file_stat)==0){
        if(S_ISDIR(file_stat.st_mode)){
            printf("\x1b[34m%s \x1b[0m",file); // Blue for directories
        }else if (file_stat.st_mode & S_IXUSR){
            printf("\x1b[32m%s \x1b[0m",file);// Green for executables
        }else{
            printf("%s ",file);// Default color for regular files
        }
    }
}
int compare(const void *a, const void *b){
    struct dirent *dir_a=*(struct dirent**)a;
    struct dirent *dir_b=*(struct dirent**)b;
    return strcasecmp(dir_a->d_name,dir_b->d_name);
}
void printwithdetails(struct dirent *file_list[],int file_count){
        for (int i=0;i<file_count;i++) {
        struct stat file_stat;
        if (stat(file_list[i]->d_name,&file_stat)==0) {
            printf((S_ISDIR(file_stat.st_mode))?"d" : 
                   (S_ISLNK(file_stat.st_mode))?"l":"-");
            printf((file_stat.st_mode & S_IRUSR)?"r":"-");
            printf((file_stat.st_mode & S_IWUSR)?"w":"-");
            printf((file_stat.st_mode & S_IXUSR)?"x":"-");
            printf((file_stat.st_mode & S_IRGRP)?"r":"-");
            printf((file_stat.st_mode & S_IWGRP)?"w":"-");
            printf((file_stat.st_mode & S_IXGRP)?"x":"-");
            printf((file_stat.st_mode & S_IROTH)?"r":"-");
            printf((file_stat.st_mode & S_IWOTH)?"w":"-");
            printf((file_stat.st_mode & S_IXOTH)?"x":"-");
            printf(" %lu", file_stat.st_nlink);
            struct passwd *pw=getpwuid(file_stat.st_uid);
            struct group *gr=getgrgid(file_stat.st_gid);
            printf(" %s %s",pw->pw_name,gr->gr_name);
            printf(" %5ld",file_stat.st_size);
            char time_str[20];
            strftime(time_str,sizeof(time_str),"%b %d %H:%M",localtime(&file_stat.st_mtime));
            printf(" %s ",time_str);
            colorcoding(file_list[i]->d_name);
            printf("\n");
        }
    }
}
void searchdir(int l,int a){
    struct dirent *de;
    struct dirent *file_list[max_files];
    DIR *dr=opendir(".");
    int file_count=0;
    if(dr==NULL){
        printf("Could not open directory.\n");
        return;
    }
    while((de=readdir(dr))!=NULL&&file_count<max_files){
        if(a==1){
            file_list[file_count]=de;
            file_count++;
        }
        else{
            if(de->d_name[0]!='.'){
                file_list[file_count]=de;
                file_count++;
            }
        }
    }
    rewinddir(dr);
    qsort(file_list,file_count,sizeof(struct dirent*),compare);
    if(l==1){
        off_t total_size=0;
        while((de=readdir(dr))!=NULL){
            if(a==1||de->d_name[0]!='.'){
                struct stat file_stat;
                if(stat(de->d_name,&file_stat)==0){
                    total_size=total_size+file_stat.st_blocks;
                }
            }
        }
        printf("total %ld\n",total_size/2);
        rewinddir(dr);
        printwithdetails(file_list,file_count);
    }
    else{
        for(int i=0;i<file_count;i++){
            colorcoding(file_list[i]->d_name);
            printf("\n");
        }
        // printf("\n");
    }
    closedir(dr);
}
void concated(char *path,char *toret0,char *toret1){
    int i;
    for(i=strlen(path)-1;i>=0;i--){
        if(path[i]=='/'){
            break;
        }
    }
    int count=0;
    for(int j=i+1;j<strlen(path);j++){
        toret1[count]=path[j];
        count++;
    }
    toret1[count]=0;
    count=0;
    for(int j=0;j<i;j++){
        toret0[count]=path[j];
        count++;
    }
    toret0[count]=0;
    return;
}
int searchfile(int l,int a,char *filename){
    struct dirent *de;
    struct dirent *file_list[max_files];
    DIR *dr=opendir(".");
    int file_count=0;
    if(dr==NULL){
        printf("Could not open directory.\n");
        return 1;
    }
    while((de=readdir(dr))!=NULL&&file_count<max_files){
        if(strcmp(filename,de->d_name)==0){
            if(a==1){
                file_list[file_count]=de;
                file_count++;
            }
            else{
                if(de->d_name[0]!='.'){
                    file_list[file_count]=de;
                    file_count++;
                }
            }
        }
    }
    rewinddir(dr);
    if(l==1){
        printwithdetails(file_list,file_count);
    }
    else{
        for(int i=0;i<file_count;i++){
            colorcoding(file_list[i]->d_name);
            printf("\n");
        }
        // printf("\n");
    }
    closedir(dr);
    if(file_count==0){
        return 1;
    }
    return 0;
}
int printdir(int details,int hidden,char *path,char *initialdir){
    int pipefd[2];
    if(pipe(pipefd)==-1){
        printf("Internal error.\n");
        return 0;
    }
    int rc=fork();
    if(rc<0){
        printf("Internal error.\n");
        return 0;
    }
    else if(rc==0){
        close(pipefd[0]);
        int n=hopchild(path,initialdir);
        freechild();
        if(n==1){
            char newpath0[max];
            char newpath1[max];
            concated(path,newpath0,newpath1);
            // char **newpath=concated(path);
            // printf("%s %s\n",newpath[0],newpath[1]);
            if(strcmp(newpath0,"")==0||strcmp(newpath0," ")==0){
                int n2=searchfile(details,hidden,newpath1);
                if(n2==1){
                    printf("No such file.\n");
                    write(pipefd[1],"0",1);
                }
                else{
                    write(pipefd[1],"1",1);
                }
            }
            else{
                int n1=hopchild(newpath0,initialdir);
                if(n1==1){
                    printf("No such directory.\n");
                    write(pipefd[1],"0",1);
                }
                else{
                    int n2=searchfile(details,hidden,newpath1);
                    if(n2==1){
                        printf("No such file.\n");
                        write(pipefd[1],"0",1);
                    }
                    else{
                        write(pipefd[1],"1",1);
                    }
                }
            }
        }
        else{
            searchdir(details,hidden);
            write(pipefd[1],"1",1);
        }
        close(pipefd[1]);
        exit(0);
    }
    else{
        close(pipefd[1]);
        int w=waitpid(rc,NULL,0);
        char buffer[2];
        // concated(path);
        read(pipefd[0],buffer,1);
        return (buffer[0]-48);
    }
}
int getdir(char **arr,int n,char *defaultdir){
    int details=0;
    int hidden=0;
    // if(n>3){
    //     printf("Invalid command only takes three arguments.\n");
    //     return;
    // }
    if(n==1){
        char temp[max];
        getDirectoryName(temp);
        int n=printdir(details,hidden,temp,defaultdir);
        if(n==1){
            return 1;
        }
        return 0;
    }
    int check=1;
    int i;
    for(i=1;i<n&&check==1;i++){
        if(arr[i][0]=='-'&&(strcmp("-",arr[i])!=0)){
            for(int j=1;j<strlen(arr[i]);j++){
                if(arr[i][j]!='a'&&arr[i][j]!='l'){
                    i--;
                    check=0;
                    break;
                }
                else if(arr[i][j]=='a'){
                    hidden=1;
                }
                else if(arr[i][j]=='l'){
                    details=1;
                }
            }
        }
        else{
            break;
        }
    }
    if(check==0){
        hidden=0;
        details=0;
        i--;
        printf("Invalid flags.\n");
        return 1;
    }
    if(strcmp(arr[i],"")==0){
        getDirectoryName(arr[i]);
    }
    int n1=printdir(details,hidden,arr[i],defaultdir);
    if(n1==1){
        return 1;
    }
    return 0;
    // printf("%s  details:%d  hidden:%d\n",arr[i],details,hidden);
}