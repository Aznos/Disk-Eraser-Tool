#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "include/colors.h"
#include "include/main.h"

void getDisks(struct DISK_INFO disks[], int* numDisks) {
    FILE *fp;
    char path[1035];
    int diskCount = 0;

    fp = popen("diskutil list", "r");
    if(fp == NULL) {
        printf("%sFailed to run, make sure you're running on OSX", RED);
        exit(1);
    }

    while(fgets(path, sizeof(path) - 1, fp) != NULL) {
        if(strstr(path, "dev/disk") != NULL && diskCount < MAX_DISKS) {
            bool sizeFound = false;
            while(fgets(path, sizeof(path) - 1, fp) != NULL && !sizeFound) {
                if(strstr(path, "GB") != NULL) {
                    float sizeGB;
                    sscanf(path, " * %f GB", &sizeGB);
                    disks[diskCount].size = (unsigned int)(sizeGB * 1024); //Convert GB to MB
                    sizeFound = true;
                } else if(strstr(path, "TB") != NULL) {
                    float sizeTB;
                    sscanf(path, " * %f TB", &sizeTB);
                    disks[diskCount].size = (unsigned int)(sizeTB * 1024 * 1024); //Convert TB to MB
                } else if(strstr(path, "MB") != NULL) {
                    float sizeMB;
                    sscanf(path, " * %f MB", &sizeMB);
                    disks[diskCount].size = (unsigned int)sizeMB;
                    sizeFound = true;
                }
            }

            diskCount++;
        }
    }

    pclose(fp);
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
        } else {
            printf("%s\nQuitting program..\n", YELLOW);
        }
    } else {
        printf("%s\nQuitting program..\n", YELLOW);
    }

    return 0;
}