#include<stdio.h>
#include"getinfo.h"
#include<string.h>
#include<stdlib.h>
#include"getcommand.h"
#include"execute.h"
#include"hop.h"
#include"log.h"
#include"systemcommand.h"
#define max 4096
int main(){
    char initialdir[max];
    getDirectoryName(initialdir);
    initialiseprev();
    createlogfile();
    char curdir[max];
    char temp[max];
    char input[max];
    char sysname[max];
    char towrite[max];
    char username[max];
    char temp2[max];
    int count=0;
    char buffer;
    long final=0;
    int skip;
    // char forprocess[100][max];
    struct sttoprint *forprocess=NULL;
    while(1){
        skip=0;
        getDirectoryName(curdir);
        checkForInidir(curdir,initialdir,temp);
        getSystemName(sysname);
        getUserName(username);
        if(count==0){
            printf("\e[1m\x1b[32m<%s@%s\x1b[0m:\x1b[33m%s\x1b[0m\x1b[32m>\x1b[0m\e[m",username,sysname,temp);
        }
        else{
            printf("\e[1m\x1b[32m<%s@%s\x1b[0m:\x1b[33m%s \x1b[0m\e[m",username,sysname,temp);
            struct sttoprint *forprocess2=forprocess;
            for(int i=0;i<count;i++){
                printf("%s : %ld",forprocess2->query,forprocess2->num);
                if(i!=count-1){
                    printf("; ");
                }
                else{
                    printf(": ");
                }
                final=final+forprocess2->num;
                forprocess2=forprocess2->next;
            }
            printf("\e[1m%lds\x1b[32m>\x1b[0m\e[m",final);
            // printf("\e[1m\x1b[32m<%s@%s\x1b[0m:\x1b[33m%s\x1b[0m %s : %lds\x1b[32m>\x1b[0m\e[m",getUserName(),sysname,temp,forprocess,forcheck);
            count=0;
            final=0;
            forprocess2=forprocess;
            while(forprocess!=NULL){
                forprocess2=forprocess;
                forprocess=forprocess->next;
                free(forprocess2->query);
                free(forprocess2);
            }
            forprocess=NULL;
        }
        scanf("%[^\n]",input);
        scanf("%c",&buffer);
        strcpy(towrite,input);
        int indexnew=0;
        struct stquerylist *commands=getCommands(input);
        struct stcommand *head=commands->command;
        while(head!=NULL){
            // printf("%s %d\n",commands->command->query,commands->command->type);
            executeCommands(head->query,head->type,initialdir,&forprocess,&count,&skip,head,indexnew,commands);
            indexnew++;
            head=head->next;
        }
        if(skip==0&&strlen(input)>0){
            writefile(towrite);
        }
        freecommand(commands);
        // if(strcmp(input,"end")==0){
        //     break;
        // }
        input[0]=0;
    }
    return 0;
}