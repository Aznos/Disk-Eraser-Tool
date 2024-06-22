#include "include/util.h"
#include "include/main.h"
#include "include/colors.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void getDisks(struct DISK_INFO disks[], int* numDisks) {
    int diskCount = 0;
    char command[100];
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
                disks[diskCount].size = (unsigned long long)(sizeGB * 1024 * 1024 * 1024); // Convert GB to bytes
            } else if (strstr(path, "TB") != NULL) {
                float sizeTB;
                sscanf(path, "   Disk Size: %f TB", &sizeTB);
                disks[diskCount].size = (unsigned long long)(sizeTB * 1024 * 1024 * 1024 * 1024); // Convert TB to bytes
            } else if (strstr(path, "MB") != NULL) {
                float sizeMB;
                sscanf(path, "   Disk Size: %f MB", &sizeMB);
                disks[diskCount].size = (unsigned long long)(sizeMB * 1024 * 1024); // Convert MB to bytes
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

void printProgress(unsigned long long totalWritten, unsigned long long totalSize, double elapsedTime) {
    double progress = (double)totalWritten / (double)totalSize;
    int barWidth = PROGRESS_BAR_WIDTH * progress;
    double writeSpeed = totalWritten / elapsedTime;
    double estimated = (totalSize - totalWritten) / writeSpeed;

    printf("\r[");
    for(int i = 0; i < PROGRESS_BAR_WIDTH; i++) {
        if(i < barWidth) printf("%s ", GREENB);
        else printf("%s ", REDB);
    }

    printf("%s] %d%% - %.2f MB/s - %llu/%llu bytes", TRANSPARENTB, (int)(progress * 100), writeSpeed / MB, totalWritten, totalSize);

    if (estimated >= 86400) {
        printf(" - Estimated time remaining: %.2f days", estimated / 86400);
    } else if (estimated >= 3600) {
        printf(" - Estimated time remaining: %.2f hours", estimated / 3600);
    } else if (estimated >= 60) {
        printf(" - Estimated time remaining: %.2f minutes", estimated / 60);
    } else {
        printf(" - Estimated time remaining: %.2f seconds", estimated);
    }

    fflush(stdout);
}