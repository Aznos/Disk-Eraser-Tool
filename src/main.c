#include "include/colors.h"
#include "include/main.h"
#include "include/disk.h"
#include "include/util.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char** argv) {
    srand(time(NULL));

    DiskManager diskManager = {getDisks};
    struct DISK_INFO disks[MAX_DISKS];
    int numDisks;

    diskManager.getDisks(disks, &numDisks);
    
    printf(CLEAR_TERMINAL);
    printf("%s%s!!! WARNING !!!\n", BBLACK, REDB);
    printf("%s%sUsing this tool will %sPERMANENTLY DELETE %sall the contents in a disk and make them %sUNRECOVERABLE%s\nIf you are sure you want to proceed, type \"I understand\" and press Enter\n\n%s> ", TRANSPARENTB, RED, BHRED, RED, BHRED, RED, WHITE);

    char input[50];
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = 0;

    if(strcmp(input, "I understand") == 0) {
        printf("%s%s!!! WARNING !!!\n", BBLACK, REDB);
        printf("%s%sAre you sure you want to proceed? This action is %sIRREVERSIBLE%s\nIf you are sure you want to proceed, type \"I am sure\" and press Enter\n\n%s> ", TRANSPARENTB, RED, BHRED, RED, WHITE);

        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = 0;
        if(strcmp(input, "I am sure") == 0) {
            printf("%sWhich disk do you want to wipe (Or type \"all\" to wipe all disks):\n", YELLOW);
            for(int i = 0; i < numDisks; i++) {
                if(disks[i].size >= TB) {
                    printf("%sDisk Number: %s%d\n%sDisk Size: %s%llu TB\n\n", CYAN, BCYAN, i + 1, CYAN, BCYAN, disks[i].size / TB);
                } else if(disks[i].size >= GB) {
                    printf("%sDisk Number: %s%d\n%sDisk Size: %s%llu GB\n\n", CYAN, BCYAN, i + 1, CYAN, BCYAN, disks[i].size / GB);
                } else if(disks[i].size >= MB) {
                    printf("%sDisk Number: %s%d\n%sDisk Size: %s%llu MB\n\n", CYAN, BCYAN, i + 1, CYAN, BCYAN, disks[i].size / MB);
                } else {
                    printf("%sDisk Number: %s%d\n%sDisk Size: %s%llu KB\n\n", CYAN, BCYAN, i + 1, CYAN, BCYAN, disks[i].size / KB);
                }
            }

            printf("%s> ", WHITE);
            fgets(input, sizeof(input), stdin);
            input[strcspn(input, "\n")] = 0;

            if(strcmp(input, "all") == 0) {
                printf("%sAre you sure you want to wipe all disks? This action is %sIRREVERSIBLE%s\nIf you are sure you want to proceed, type \"yes\" and press Enter\n\n%s> ", YELLOW, BHRED, RED, WHITE);
                fgets(input, sizeof(input), stdin);
                input[strcspn(input, "\n")] = 0;

                if(strcmp(input, "yes") == 0) {
                    printf("%sHow many passes do you want to make? (Default is 5, minimum is 2)\n%s> ", YELLOW, WHITE);
                    fgets(input, sizeof(input), stdin);
                    input[strcspn(input, "\n")] = 0;

                    int passes = atoi(input);
                    if(passes <= 0) {
                        passes = 5;
                    }

                    for(int i = 0; i < numDisks; i++) {
                        eraseDisk(disks[i], i, passes);
                    }
                } else {
                    printf("%s\nQuitting program..\n", YELLOW);
                }
            } else {
                int diskNum = atoi(input);
                if(diskNum > 0 && diskNum <= numDisks) {
                    printf("%sAre you sure you want to wipe disk %s%d%s? This action is %sIRREVERSIBLE%s\nIf you are sure you want to proceed, type \"yes\" and press Enter\n\n%s> ", YELLOW, BCYAN, diskNum, YELLOW, BHRED, RED, WHITE);
                    fgets(input, sizeof(input), stdin);
                    input[strcspn(input, "\n")] = 0;

                    if(strcmp(input, "yes") == 0) {
                        printf("%sHow many passes do you want to make? (Default is 5, minimum is 2)\n%s> ", YELLOW, WHITE);
                        fgets(input, sizeof(input), stdin);
                        input[strcspn(input, "\n")] = 0;

                        int passes = atoi(input);
                        if(passes <= 0) {
                            passes = 5;
                        }

                        eraseDisk(disks[diskNum - 1], diskNum - 1, passes);
                    } else {
                        printf("%s\nQuitting program..\n", YELLOW);
                    }
                } else {
                    printf("%sInvalid disk number\nQuitting program..\n", YELLOW);
                }
            }
        } else {
            printf("%s\nQuitting program..\n", YELLOW);
        }
    } else {
        printf("%s\nQuitting program..\n", YELLOW);
    }

    return 0;
}