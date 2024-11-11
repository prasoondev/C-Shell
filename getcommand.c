#include<stdlib.h>
#include<string.h>
#include<stdio.h>
#include"getcommand.h"
#define max 4096
struct stcommand *createCommand(char *arr,int del){
    struct stcommand *s=(struct stcommand*)malloc(sizeof(struct stcommand));
    // printf("%ld",sizeof(struct stcommand));
    s->query=(char*)malloc(sizeof(char)*max);
    strcpy(s->query,arr);
    s->type=del;
    s->next=NULL;
    return s;
}
struct stquerylist *createList(){
    struct stquerylist *s=(struct stquerylist*)malloc(sizeof(struct stquerylist));
    s->numcommands=0;
    s->command=NULL;
    return s;
}
struct stcommand *insert(char *command,int del,struct stcommand *temp){
    if(temp==NULL){
        return createCommand(command,del);
    }
    struct stcommand *head=temp;
    while(head->next!=NULL){
        head=head->next;
    }
    head->next=createCommand(command,del);
    return temp;
}
// void freecommand(struct stquerylist *commands){
//     struct stcommand *temp;
//     struct stcommand *head=commands->command;
//     while(head!=NULL){
//         temp=head;
//         head=head->next;
//         free(temp->query);
//         free(temp);
//     }
//     // free(commands->command);
//     free(commands);
// }
struct stquerylist *getCommands(char *input){
    struct stquerylist *toret=createList();
    char temp[max];
    strcpy(temp,input);
    char *token=strtok(input,"&;");
    int count=0;
    toret->numcommands++;
    while(token!=NULL){
        count=count+strlen(token);
        if(temp[count]=='&')
        {
            toret->command=insert(token,1,toret->command);
        }
        else{
            toret->command=insert(token,0,toret->command);
        }
        token=strtok(NULL,"&;");
        count++;
        toret->numcommands++;
    }
    return  toret;
}