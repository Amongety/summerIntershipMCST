
#define _GNU_SOURCE
#define _ATFILE_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <errno.h>
#include <time.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <linux/stat.h>
#include <linux/fcntl.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>

char typeFile(__u16);
void convert_time(struct statx_timestamp);
void convert_mode(__u16, char*);

int main(int argc, char *argv[]) {
    struct statx info_file;
    char strMode[10];
    
    if(argc != 2) {
        printf("Invalid arguments\n");
        return -1;
    }
    if((syscall(__NR_statx, AT_FDCWD, argv[1], AT_SYMLINK_NOFOLLOW,  STATX_BASIC_STATS | STATX_BTIME, &info_file)) < 0) {
        perror("In function statx");
        return errno;
    };
    
    printf("File: %s\n", argv[1]);

    printf("Size: %-7lld\tBlocks: %-7lld\tIO Block: %-7d\t", info_file.stx_size, info_file.stx_blocks, info_file.stx_blksize);
    strMode[0] = typeFile(info_file.stx_mode);

    printf("Device: %d/%d\tInode: %-7lld\tLinks: %-7d\n", info_file.stx_dev_major, info_file.stx_dev_minor, info_file.stx_ino, info_file.stx_nlink);

    convert_mode(info_file.stx_mode, strMode);
    printf("Access: ( %04o/%s )\tUid: ( %d/%s )\tGid: ( %d/%s )\n", info_file.stx_mode & 0777, strMode, info_file.stx_uid, getpwuid(info_file.stx_uid)->pw_name, info_file.stx_gid, getgrgid(info_file.stx_gid)->gr_name);

    printf("Access: ");
    convert_time(info_file.stx_atime);

    printf("Modify: ");
    convert_time(info_file.stx_mtime);

    printf("Change: ");
    convert_time(info_file.stx_ctime);

    printf("Brith: ");
    convert_time(info_file.stx_btime);
    
    return 0;
}

char typeFile(__u16 m) {
    if(S_ISREG(m)) {
        printf("regular file\n");
        return '-';
    }
    else if(S_ISDIR(m)) {
        printf("directory\n");
        return 'd';
    }
    else if(S_ISLNK(m)) {
        printf("symbolic link\n");
        return 'l';
    }
    else if(S_ISBLK(m)) {
        printf("block device\n");
        return 'b';
    }
    else if(S_ISCHR(m)) {
        printf("character device\n");
        return 'c';
    }
    else if(S_ISFIFO(m)) {
        printf("fifo file\n");
        return 'f';
    }
    else if(S_ISSOCK(m)) {
        printf("socket\n");
        return 's';
    }

    return '0';
}

void convert_time(struct statx_timestamp t) {
    struct tm *time = localtime((const time_t*)&t.tv_sec);

    char timeZone[6];
    strftime(timeZone, sizeof(timeZone), "%z", time);

    printf("%d-%02d-%02d %02d:%02d:%02d.%d %s\n", time->tm_year + 1900, time->tm_mon + 1, time->tm_mday, time->tm_hour, time->tm_min, time->tm_sec, t.tv_nsec, timeZone);
}

void convert_mode(__u16 mode, char* strMode) {
    for(int i = 2; i >= 0; --i) {
        switch((mode >> (i * 3)) & 0x7) {
            case 7:
                strcat(strMode, "rwx");
                break;
            case 6:
                strcat(strMode, "rw-");
                break;
            case 5:
                strcat(strMode, "r-x");
                break;
            case 4:
                strcat(strMode, "r--");
                break;
            case 3:
                strcat(strMode, "-wx");
                break;
            case 2:
                strcat(strMode, "-w-");
                break;
            case 1:
                strcat(strMode, "--x");
            case 0:
                strcat(strMode, "---");
                break;
        } 
    }
}