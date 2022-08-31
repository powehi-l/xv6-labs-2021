#include"kernel/types.h"
#include"kernel/stat.h"
#include"user/user.h"
#include"kernel/fs.h"

void find(char *path, char *filename){
    char buf[512], *p;
    int fd;
    struct stat st;
    struct dirent de;

    if((fd = open(path, 0)) < 0){
        fprintf(2, "find: can not open %s\n", path);
        return;
    }

    if(fstat(fd, &st) < 0){
        fprintf(2, "find: can not stat %s\n", path);
        return;
    }

    switch(st.type){
        case T_FILE:
            for(p=path+strlen(path); p >= path && *p != '/'; p--);
            p++;
            if(strcmp(p, filename) == 0){
                printf("%s\n", path);
            }
            break;
        case T_DIR:
            if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
                printf("ls: path too long\n");
                break;
            }
            strcpy(buf, path);
            p = buf+strlen(buf);
            *p++ = '/';
            while(read(fd, &de, sizeof(de)) == sizeof(de)){
                if(de.inum == 0)
                    continue;
                if(de.name[0] == '.' && de.name[1] == '\0')
                    continue;
                if(de.name[0] == '.' && de.name[1] == '.' && de.name[2] == '\0')
                    continue;
                memmove(p, de.name, DIRSIZ);
                p[DIRSIZ] = 0;
                find(buf, filename);
            }
            break;
    }
    close(fd);
}

int
main(int argc, char *argv[])
{
    if(argc != 3){
        fprintf(2, "wrong argument!\n");
        exit(1);
    }  
    find(argv[1], argv[2]);
    exit(0);
}