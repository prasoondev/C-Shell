#ifndef __GETINFO_H__
#define __GETINFO_H__
const char *getUserName();
const char *getSystemName();
void getDirectoryName();
void checkForInidir(char *curdir,char *inidir,char *temp);
#endif