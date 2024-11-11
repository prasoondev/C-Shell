#ifndef __LOG_H__
#define __LOG_H__
#define max 4096
typedef struct sttoprint sttoprint;
typedef struct stsplitcommand stsplitcommand;
void createlogfile();
typedef struct stquerylist stquerylist;
typedef struct stcommand stcommand;
int logcommand(char **arr,int n,struct sttoprint **forename,int *count1,int *skip,struct stcommand *command,int index);
void writefile(char *command);
#endif