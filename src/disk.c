#include "include/disk.h"
#include "include/colors.h"
#include "include/main.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>

void overwriteDisk(const char* diskPath, bool random, unsigned long long size) {
    int disk = open(diskPath, O_WRONLY);
    if(disk < 0) {
        printf("%sFailed to open disk %s\n", RED, diskPath);
        return;
    }

    unsigned char buffer[BUFFER_SIZE];
    unsigned long long totalWritten = 0;
    time_t startTime = time(NULL);
    time_t lastPrinttime = startTime;

    while(size > 0) {
        if(random) {
            for(unsigned int i = 0; i < sizeof(buffer); i++) {
                buffer[i] = "0123456789ABCDEF"[rand() % 16];
            }
        } else {
            memset(buffer, '0', sizeof(buffer));
        }

        ssize_t written = write(disk, buffer, sizeof(buffer));
        if(written < 0) {
            printf("%sFailed to write to disk %s\n", RED, diskPath);
            break;
        }

        totalWritten += written;
        size -= written;

        time_t currentTime = time(NULL);
        if(difftime(currentTime, lastPrinttime) >= 1.0) {
            double elapsedTime = difftime(currentTime, startTime);
            double writeSpeed = totalWritten / elapsedTime; //BPS
            unsigned long long bytesLeft = totalWritten + size;
            double estimated = bytesLeft / writeSpeed;

            if(estimated >= 86400) {
                estimated /= 86400;
                printf("%s%llu/%llu bytes written (%.2f%%)\nEstimated time remaining: %.2f days\n", YELLOW, totalWritten, totalWritten + size, (double)totalWritten / (double)(totalWritten + size) * 100, estimated);
            } else if(estimated >= 3600) {
                estimated /= 3600;
                printf("%s%llu/%llu bytes written (%.2f%%)\nEstimated time remaining: %.2f hours\n", YELLOW, totalWritten, totalWritten + size, (double)totalWritten / (double)(totalWritten + size) * 100, estimated);
            } else if(estimated >= 60) {
                estimated /= 60;
                printf("%s%llu/%llu bytes written (%.2f%%)\nEstimated time remaining: %.2f minutes\n", YELLOW, totalWritten, totalWritten + size, (double)totalWritten / (double)(totalWritten + size) * 100, estimated);
            } else {
                printf("%s%llu/%llu bytes written (%.2f%%)\nEstimated time remaining: %.2f seconds\n", YELLOW, totalWritten, totalWritten + size, (double)totalWritten / (double)(totalWritten + size) * 100, estimated);
            }

            lastPrinttime = currentTime;
        }
    }
    
    close(disk);
}

void eraseDisk(struct DISK_INFO disk, int num, int passes) {
    printf("%sErasing disk %d\n", YELLOW, num + 1);
    char diskPath[50];
    snprintf(diskPath, sizeof(diskPath), "/dev/disk%d", num);

    for(int i = 0; i < passes; i++) {
        if(i < 1) {
            overwriteDisk(diskPath, false, disk.size);
        } else {
            overwriteDisk(diskPath, true, disk.size);
        }

        printf("%sPass %d complete\n", YELLOW, i + 1);
        sleep(1);
    }
}