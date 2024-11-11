#ifndef __EXECUTE_H__
#define __EXECUTE_H__
#define max 4096
typedef struct sttoprint{
    char *query;
    long num;
    struct sttoprint *next;
}sttoprint;
typedef struct stsplitcommand{
    char **arr;
    int n;
}stsplitcommand;
typedef struct stcommand stcommand;
typedef struct stquerylist stquerylist;
void freecommand(struct stquerylist *commands);
void freechild();
void executeCommands(char *query,int type,char *defaultdir,struct sttoprint **forename,int *count,int *skip,struct stcommand *commands,int indexnew,struct stquerylist *commandstofree);
#endif