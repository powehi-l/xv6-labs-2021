#include"kernel/types.h"
#include"kernel/stat.h"
#include"kernel/param.h"
#include"user/user.h"
#define MAXCHAR 1024

int copy(char **des, char *ori){
    if((*des = (char*)malloc(strlen(ori) + 1)) != 0)
        strcpy(*des, ori);
    else
        return -1;
    return 0;
}

int readline(char** parameter, int start){
    char buf[MAXCHAR];
    int length = 0, cursor = 0;
    while(read(0, buf+length, 1)){
        if(buf[length] == '\n'){
            buf[length] = 0;
            break;
        }
        length++;
    }

    if(length == MAXCHAR){
        fprintf(2, "too much argument!\n");
        exit(1);
    }
    if(length == 0){
        return -1;
    }

    while(cursor < length){
        while(cursor < length && buf[cursor] == ' ');
        int argustart = cursor;
        while(cursor < length && buf[cursor] != ' ')
            cursor++;
        buf[cursor++] = 0;
        copy(&parameter[start], buf+argustart);
        start++;
    }
    return start;
}

int
main(int argc, char *argv[])
{
    if(argc < 2){
        fprintf(2, "argument is too less\n");
        exit(1);
    }
    int i, end;
    char* par[MAXARG];
    for(i = 1; i < argc; i++){
        if(copy(&par[i-1], argv[i]) == -1){
            printf("fail to excute\n");
            exit(1);
        }
    }

    while((end = readline(par, argc-1)) != -1){
        par[end] = 0;
        if(fork() == 0){
            exec(par[0], par);
            exit(1);
        }
        else{
            wait(0);
        }
    }
    exit(0);
}