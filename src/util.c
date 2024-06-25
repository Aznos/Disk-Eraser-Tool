#include "include/util.h"
#include "include/main.h"
#include "include/colors.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void getDisks(struct DISK_INFO disks[], int* numDisks) {
    int diskCount = 0;
    char command[] = "diskutil list | grep '^/dev/disk[0-9]' | awk '{print $1}'";
    char path[1035];
    FILE *fp;

    fp = popen(command, "r");
    if (fp == NULL) {
        printf("%sFailed to run command\n", RED);
        exit(1);
    }

    while (fgets(path, sizeof(path) - 1, fp) != NULL) {
        path[strcspn(path, "\n")] = 0;
        char sizeCommand[100];
        snprintf(sizeCommand, sizeof(sizeCommand), "diskutil info %s | grep 'Disk Size'", path);

        FILE *sizeFp = popen(sizeCommand, "r");
        if (sizeFp == NULL) {
            printf("%sFailed to run command for size\n", RED);
            exit(1);
        }

        char sizePath[1035];
        if (fgets(sizePath, sizeof(sizePath) - 1, sizeFp) != NULL) {

            if (strstr(sizePath, "GB") != NULL) {
                float sizeGB;
                sscanf(sizePath, "   Disk Size: %f GB", &sizeGB);
                disks[diskCount].size = (unsigned long long)(sizeGB * 1024 * 1024 * 1024); // Convert GB to bytes
            } else if (strstr(sizePath, "TB") != NULL) {
                float sizeTB;
                sscanf(sizePath, "   Disk Size: %f TB", &sizeTB);
                disks[diskCount].size = (unsigned long long)(sizeTB * 1024 * 1024 * 1024 * 1024); // Convert TB to bytes
            } else if (strstr(sizePath, "MB") != NULL) {
                float sizeMB;
                sscanf(sizePath, "   Disk Size: %f MB", &sizeMB);
                disks[diskCount].size = (unsigned long long)(sizeMB * 1024 * 1024); // Convert MB to bytes
            } else {
                disks[diskCount].size = 0; // If size is not found or recognized
            }

            diskCount++;
        }

        pclose(sizeFp);
    }

    pclose(fp);
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

    int days = estimated / 86400;
    int hours = (estimated - (days * 86400)) / 3600;
    int minutes = (estimated - (days * 86400) - (hours * 3600)) / 60;
    int seconds = estimated - (days * 86400) - (hours * 3600) - (minutes * 60);

    if (days > 0) {
        printf(" - Estimated time remaining: %d days, %d hours, %d minutes, %d seconds", days, hours, minutes, seconds);
    } else if (hours > 0) {
        printf(" - Estimated time remaining: %d hours, %d minutes, %d seconds", hours, minutes, seconds);
    } else if (minutes > 0) {
        printf(" - Estimated time remaining: %d minutes, %d seconds", minutes, seconds);
    } else {
        printf(" - Estimated time remaining: %d seconds", seconds);
    }

    fflush(stdout);
}