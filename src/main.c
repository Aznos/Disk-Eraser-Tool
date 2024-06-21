#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "include/colors.h"
#include "include/main.h"

void getDisks(struct DISK_INFO disks[], int* numDisks) {
    int diskCount = 0;
    char command[50];
    char path[1035];
    FILE *fp;

    for(int i = 0; i < MAX_DISKS; i++) {
        snprintf(command, sizeof(command), "diskutil info /dev/disk%d | grep 'Disk Size'", i);
        fp = popen(command, "r");
        if(fp == NULL) {
            printf("%sFailed to run, make sure you're running on OSX\n", RED);
            exit(1);
        }

        if (fgets(path, sizeof(path) - 1, fp) != NULL) {
            if (strstr(path, "GB") != NULL) {
                float sizeGB;
                sscanf(path, "   Disk Size: %f GB", &sizeGB);
                disks[diskCount].size = (unsigned int)(sizeGB * 1024); // Convert GB to MB
            } else if (strstr(path, "TB") != NULL) {
                float sizeTB;
                sscanf(path, "   Disk Size: %f TB", &sizeTB);
                disks[diskCount].size = (unsigned int)(sizeTB * 1024 * 1024); // Convert TB to MB
            } else if (strstr(path, "MB") != NULL) {
                float sizeMB;
                sscanf(path, "   Disk Size: %f MB", &sizeMB);
                disks[diskCount].size = (unsigned int)sizeMB;
            } else {
                disks[diskCount].size = 0; // If size is not found or recognized
            }

            diskCount++;
        } else {
            break; // No more disks found
        }

        pclose(fp);
    }

    *numDisks = diskCount;
}

int main(int argc, char** argv) {
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
        printf("%s%s!!! WARNING !!!\n", RED, REDB);
        printf("%sAre you sure you want to proceed? This action is %sIRREVERSIBLE%s\nIf you are sure you want to proceed, type \"I am sure\" and press Enter\n\n%s> ", TRANSPARENTB, BHRED, RED, WHITE);

        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = 0;
        if(strcmp(input, "I am sure") == 0) {
            printf("%sWhich disk do you want to wipe (Or type \"all\" to wipe all disks):\n", YELLOW);
            for(int i = 0; i < numDisks; i++) {
                if(disks[i].size >= 1024 * 1024) {
                    printf("%sDisk Number: %s%d\n%sDisk Size: %s%u TB\n\n", CYAN, BCYAN, i + 1, CYAN, BCYAN, disks[i].size / (1024 * 1024));
                } else if(disks[i].size >= 1024) {
                    printf("%sDisk Number: %s%d\n%sDisk Size: %s%u GB\n\n", CYAN, BCYAN, i + 1, CYAN, BCYAN, disks[i].size / 1024);
                } else {
                    printf("%sDisk Number: %s%d\n%sDisk Size: %s%u MB\n\n", CYAN, BCYAN, i + 1, CYAN, BCYAN, disks[i].size);
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
                    printf("%sDeleting..\n", YELLOW);
                } else {
                    printf("%s\nQuitting program..\n", YELLOW);
                }
            } else if(atoi(input) > 0 && atoi(input) <= numDisks) {
                printf("%sAre you sure you want to wipe disk %s%d%s? This action is %sIRREVERSIBLE%s\nIf you are sure you want to proceed, type \"yes\" and press Enter\n\n%s> ", YELLOW, BCYAN, atoi(input), YELLOW, BHRED, RED, WHITE);
                fgets(input, sizeof(input), stdin);
                input[strcspn(input, "\n")] = 0;

                if(strcmp(input, "yes") == 0) {
                    printf("%sDeleting..\n", YELLOW);
                } else {
                    printf("%s\nQuitting program..\n", YELLOW);
                }
            } else {
                printf("%sInvalid disk number\nQuitting program..\n", YELLOW);
            }
        } else {
            printf("%s\nQuitting program..\n", YELLOW);
        }
    } else {
        printf("%s\nQuitting program..\n", YELLOW);
    }

    return 0;
}