#define _GNU_SOURCE
#include <stdio.h>
#include <stdbool.h>
#include <getopt.h>
#include <string.h>

#define MAX_STRING 256
#define SIZE_SHORT 4
#define SIZE_LONG 6

int main(int argc, char *argv[]) {
    if(argc < 2) {
        perror("Invalid argument");
        return -1;
    }

    bool vivsitedShort[SIZE_SHORT] = {false}, vivsitedLong[SIZE_LONG] = {false};
    const struct option long_options[] = {{"elbrus", required_argument, NULL, 'e'}, {0, 0, 0, 0}};
    const char *typeElbrus[] = {"1c+", "2c+", "2c3", "4c", "8c", "16c"};
    int shortInd = 0, longInd = 0, arg, longind;
    char shortOpt[SIZE_SHORT], longOpt[SIZE_LONG][MAX_STRING] = {0};

    opterr = 0;

    do {
        if(strncmp(argv[optind], "--", 2) == 0 || argv[optind][0] != '-') {
            ++optind;
            continue;
        }

        if((arg = getopt(argc, argv, "mcst")) == -1) break;

        switch(arg) {
            case 'm':
                if(!vivsitedShort[0]) {
                    shortOpt[shortInd++] = 'm';
                    vivsitedShort[0] = true;
                }

                break;
            case 'c':
               if(!vivsitedShort[1]) {
                    shortOpt[shortInd++] = 'c';
                    vivsitedShort[1] = true;
                }
                break;
            case 's':
                if(!vivsitedShort[2]) {
                    shortOpt[shortInd++] = 's';
                    vivsitedShort[2] = true;
                }

                break;
            case 't':
                if(!vivsitedShort[3]) {
                    shortOpt[shortInd++] = 't';
                    vivsitedShort[3] = true;
                }

                break;
            case '?':
                printf("Incorrect option: \'%c\'\n", optopt);
                return -1;
        }
    } while(argc > optind);

    optind = 1;
    while((arg = getopt_long(argc, argv, "", long_options, &longind)) != -1) {
        switch(arg) {
            case 'e':
                int i = 0;
                for(; i < SIZE_LONG; ++i) {
                    if(!strcmp(optarg, typeElbrus[i])) {
                        if(!vivsitedLong[i]) break;

                        strcat(longOpt[longInd], long_options[longind].name);
                        strcat(longOpt[longInd], "=");
                        strcat(longOpt[longInd++], optarg);
                        
                        vivsitedLong[i] = true;
                        break;
                    }
                }

                if(i >= SIZE_LONG) {
                    printf("Incorrect option: \'%s\'\n", argv[optind - 1] + 2);
                    return -1;
                }

                break;
            case '?':
                if(optind > 1 && strncmp(argv[optind - 1], "--", 2) == 0) {
                    printf("Incorrect option: \'%s\'\n", argv[optind - 1] + 2);
                    return -1;
                }
                
                break;
        };
    }

    if(shortInd > 0) {
        printf("Short options: ");
        for(int i = 0; i < shortInd; ++i) printf("\'%c\' ", shortOpt[i]);
    }

    if(longInd > 0) {
        printf("\nLong options: ");
        for(int i = 0; i < longInd; ++i) printf("\'%s\' ", longOpt[i]);
    }

    printf("\nNon options: ");
    for(int i = optind; i < argc; ++i) printf("\'%s\' ", argv[i]);
    
    printf("\n");

    return 0;
}