#include"kernel/types.h"
#include"kernel/stat.h"
#include"user/user.h"

#define maxnum 35

// int prime(int p[], int count){
//     int number;
//     read(p[0], &number, 4);
//     close(p[0]);

//     if(count * count > number){
//         fprintf(2, "prime %d\n", number);
//         exit(0);
//     }

//     if(number % count == 0){
//         exit(1);
//     }
    
//     int pipe_[2];
//     pipe(pipe_);
//     write(pipe_[1], &number, 4);
//     close(pipe_[1]);
//     if(fork() == 0){
//         prime(pipe_, count+1);
//     }
//     else{
//         wait(0);
//     }
//     exit(0);
// }

// int
// main(int argc, char *argv[])
// {
//     if(argc != 1){
//         fprintf(2, "Usage: primes\n");
//         exit(1);
//     }

//     for(int i = 2; i <= maxnum; i++){
//         int number = i;
//         int p[2];
//         pipe(p);
//         if(fork() == 0){
//             printf("prime %d begin\n", i);
//             write(p[1], &number, 4);
//             close(p[1]);
//             prime(p, 2);
//             exit(0);
//         }
//         else{
//             wait(0);
//         }
//     }
//     while(wait(0) != -1);
//     exit(0);
// }

void prime(int *fd){
    int p, m;
    if(read(fd[0], &p, sizeof(p)) != sizeof(p)){
        fprintf(2, "fail to read from pipe\n");
        exit(1);
    }

    printf("prime %d\n", p);

    if(read(fd[0], &m, 4)){
        int fd1[2];
        pipe(fd1);
        if(fork() == 0){
            close(fd[0]);
            close(fd1[1]);
            prime(fd1);
        }
        else{
            close(fd1[0]);
            do{
                if(m % p != 0){
                    write(fd1[1], &m, sizeof(m));
                }
            }while(read(fd[0], (void*)&m, sizeof(m)));
            close(fd[0]);
            close(fd1[1]);
            wait(0);
        }
    }
    exit(0);
}

int
main(int argc, char *argv[])
{
    if(argc != 1){
        fprintf(2, "Usage: primes\n");
        exit(1);
    }

    int fd[2];
    pipe(fd);

    if(fork() == 0){
        close(fd[1]);
        prime(fd);
    }
    else{
        close(fd[0]);
        for(int i = 2; i < maxnum; i++){
            if(write(fd[1], (void*)&i, sizeof(i)) != sizeof(i)){
                fprintf(2, "fail to write into pipe\n");
                exit(1);
            }
        }
        close(fd[1]);
        wait(0);
    }
    return 0;
}

