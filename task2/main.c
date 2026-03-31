#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

#define MAX_READ 256

int main(int argc, char *argv[]) {
    size_t input_len = 0, n = 0;
    char *input = NULL;
    char output[MAX_READ];
    int fd;
    pid_t pid;

    if(argc != 2) {
        printf("Invalid arguments\n");
        return -1; 
    }

    if ((fd = open(argv[1], O_RDWR)) == -1) {
        perror("Open");
        return -1;
    }

    pid = fork();
    if(pid == -1) {
        close(fd);
        perror("Fork");
        return -1;
    }

    if(pid == 0) {
        while(true) {
            if((n = getline(&input, &input_len, stdin)) > 0) write(fd, input, n);
        }
    }
    else {
        while(true) {
            if(read(fd, output, MAX_READ - 1) > 0) puts(output);
        }
    }

    return 0;
}