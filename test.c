#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
int main(){
    printf("%d",getpid());
    fflush(stdout);
    while(1){
    }
    return 0;
}