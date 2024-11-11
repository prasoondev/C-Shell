#include"execute.h"
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include"hop.h"
#include"getinfo.h"
#include"proclore.h"
#include"systemcommand.h"
#include"reveal.h"
#include"seek.h"
#include"log.h"
#include"getcommand.h"
#define max 4096
struct sttoprint *forprocessglobal;
struct stquerylist *commandsglobal;
struct stsplitcommand *newglobal;
void freecommand(struct stquerylist *commands){
    struct stcommand *temp;
    struct stcommand *head=commands->command;
    while(head!=NULL){
        temp=head;
        head=head->next;
        free(temp->query);
        free(temp);
    }
    // free(commands->command);
    free(commands);
}
void freefore(struct sttoprint *forprocess){
    struct sttoprint *forprocess2=forprocess;
    while(forprocess!=NULL){
        forprocess2=forprocess;
        forprocess=forprocess->next;
        free(forprocess2->query);
        free(forprocess2);
    }
}
void freen(struct stsplitcommand *new){
    for(int i=0;i<max;i++){
        free(new->arr[i]);
    }
    free(new->arr);
    free(new);
}
void freechild(){
    freefore(forprocessglobal);
    freecommand(commandsglobal);
    freen(newglobal);
}
struct sttoprint *create(char *query,long num){
    struct sttoprint *new=(struct sttoprint*)malloc(sizeof(struct sttoprint));
    new->query=(char*)malloc(sizeof(char)*max);
    strcpy(new->query,query);
    new->num=num;
    new->next=NULL;
    return new;
}
struct sttoprint *insertin(struct sttoprint *fore,long n,char *query){
    if(fore==NULL){
        return create(query,n);
    }
    struct sttoprint *head=fore;
    while(head->next!=NULL){
        head=head->next;
    }
    head->next=create(query,n);
    return fore;
}
struct stsplitcommand *createC(){
    struct stsplitcommand *new=(struct stsplitcommand*)malloc(sizeof(struct stsplitcommand));
    new->n=0;
    new->arr=(char**)malloc(sizeof(char*)*max);
    for(int i=0;i<max;i++){
        new->arr[i]=(char*)malloc(sizeof(char)*max);
        strcpy(new->arr[i],"");
    }
    return new;
}
void executeCommands(char *query,int type,char *defaultdir,struct sttoprint **forename,int *count,int *skip,struct stcommand *commands,int indexnew,struct stquerylist *commandstofree){
    forprocessglobal=*forename;
    commandsglobal=commandstofree;
    struct stsplitcommand *new=createC();
    char tolog[max];
    strcpy(tolog,query);
    char *token=strtok(query," \t");
    while(token!=NULL){
        strcpy(new->arr[new->n],token);
        new->n++;
        token=strtok(NULL," \t");
    }
    newglobal=new;
    if(strcmp("hop",new->arr[0])==0){
        int n=hop(new->arr,new->n,defaultdir);
    }
    else if(strcmp("reveal",new->arr[0])==0){
        int n=getdir(new->arr,new->n,defaultdir);
    }
    else if(strcmp("log",new->arr[0])==0){
        int n=logcommand(new->arr,new->n,forename,count,skip,commands,indexnew);
        *skip=1;
    }
    else if(strcmp("proclore",new->arr[0])==0){
        proc(new->arr,new->n);
    }
    else if(strcmp("seek",new->arr[0])==0){
        int n=seeksearch(new->arr,new->n,defaultdir);
    }
    else{
        if(type==0){
            long n=foreground(tolog);
            if(n!=0){
                *forename=insertin(*forename,n,new->arr[0]);
                *count=*count+1;
            }
        }
        else{
            background(tolog,new->arr[0]);
            //background
        }
    }
    freen(new);
}