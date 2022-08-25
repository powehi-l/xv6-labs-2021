#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
    if(argc != 1){
        fprintf(2, "Usage: pingpong\n");
        exit(1);
    }
    int pipe_[2];
    pipe(pipe_);//create a pipe
    //create a child process
    if(fork() == 0){
        char buf[2];
        if(read(pipe_[0], buf, 1) != 1){
            fprintf(2, "fail to read from pipe in child\n");
            exit(1);
        }
        close(pipe_[0]);
        fprintf(2, "%d: received ping\n", getpid());

        if(write(pipe_[1], buf, 1) != 1){
            fprintf(2, "fail to write in child\n");
            exit(1);
        }
        close(pipe_[1]);
        exit(0);
    }
    else{
        char origin[2] = "a";
        char buf[2];
        // buf[1] = 0;

        if(write(pipe_[1], origin, 1) != 1){
            fprintf(2, "fail to write in the parent\n");
            exit(1);
        }
        close(pipe_[1]);
        wait(0);
        if(read(pipe_[0], buf, 1) != 1){
            fprintf(2, "fail to read from pipe in the parent\n");
            exit(1);
        }
        fprintf(2, "%d: received pong\n", getpid());
        close(pipe_[0]);
        exit(0);
        
    }
}
