#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

int main() {
    const char* buf_err[] = {"", "EPERM", "ENOENT",  "ESRCH", "EINTR", "EIO", "ENXIO", "E2BIG", "ENOEXEC", "EBADF", "ECHILD",
                            "EAGAIN", "ENOMEM", "EACCES", "EFAULT", "ENOTBLK", "EBUSY", "EEXIST", "EXDEV", "ENODEV", "ENOTDIR",
                            "EISDIR", "EINVAL", "ENFILE", "EMFILE", "ENOTTY", "ETXTBSY", "EFBIG", "ENOSPC", "ESPIPE", "EROFS",
                            "EMLINK", "EPIPE", "EDOM", "ERANGE"};
    int fd;

    if((fd = open("test.txt", O_RDWR)) == -1) {
        printf("Returned error = %d\n", errno);
        printf("Errno = %s\n", buf_err[errno]);
        printf("sys_errlist = %s\n", sys_errlist[errno]);
        perror("perror");

	    return errno;
    }

    close(fd);
    
    return 0;
}
