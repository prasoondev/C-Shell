#ifndef __GETCOMMAND_H__
#define __GETCOMMAND_H__
typedef struct stcommand{
    char *query;
    int type;               //0 = ;   & = 1
    struct stcommand *next;
}stcommand;
typedef struct stquerylist{
    int numcommands;
    struct stcommand *command;
}stquerylist;
struct stquerylist *getCommands(char *input);
// void freecommand(struct stquerylist *commands);
#endif