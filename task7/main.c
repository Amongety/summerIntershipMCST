#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    int fd, pid, n;
    struct stat infoFile;

    if(argc != 2) {
        printf("Invalid arguments\n");
        return -1;
    }

    fd = open(argv[1], O_RDONLY);
     if(fd == -1) {
        perror("File open");
        return errno;
    }

    if(fstat(fd, &infoFile) == -1) {
        perror("Funcrtion fstat");
        close(fd);
        return errno;
    }

    pid = fork();
    if(pid == -1) {
        perror("Function fork");
        return errno;
    }

    if(pid == 0) {
        lseek(fd, 0, SEEK_SET);

        int fdChild = open("child_copy", O_WRONLY | O_CREAT | O_TRUNC, 0777);
        if(fdChild == -1) {
            perror("Child function open");
            exit(errno);
        }

        char* buffCh = (char*)malloc(infoFile.st_size);
        for(int i = 0; i < infoFile.st_size;) {
            i += read(fd, buffCh + i, infoFile.st_size - i);
        }
        for(int i = 0; i < infoFile.st_size;) {
            i += write(fdChild, buffCh + i, infoFile.st_size - i);
        }

        close(fdChild);
        free(buffCh);
        exit(0);
    }
    else {
        wait(NULL);

        lseek(fd, 0, SEEK_SET);

        int fdParent = open("parent_copy", O_WRONLY | O_CREAT | O_TRUNC, 0777);
        if(fdParent == -1) {
            perror("Parent function open");
            return errno;
        }

        char* buffPr = (char*)malloc(infoFile.st_size);
        for(int i = 0; i < infoFile.st_size;) {
            i += read(fd, buffPr + i, infoFile.st_size - i);
        }
        for(int i = 0; i < infoFile.st_size;) { 
            i += write(fdParent, buffPr + i, infoFile.st_size - i);
        }

        close(fdParent);
        free(buffPr);
    }

    int fd1, fd2;
    fd1 = open("parent_copy", O_RDONLY);
    if(fd1 == -1) {
        perror("open parent_copy");
        close(fd1);
        return errno;
    }
    fd2 = open("child_copy", O_RDONLY);
    if(fd2 == -1) {
        perror("open child_copy");
        return errno;
    }

    char buff[1024];
    printf("Parent:\n");
    while((n = read(fd1, buff, sizeof(buff))) > 0) fwrite(buff, sizeof(char), n, stdout);

    printf("Child:\n");
    while((n = read(fd2, buff, sizeof(buff))) > 0) fwrite(buff, sizeof(char), n, stdout);

    close(fd);
    close(fd1);
    close(fd2);

    return 0;
}