#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h>
#include "include/colors.h"
#include "include/main.h"

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

void overwriteDisk(const char* diskPath, bool random, unsigned long long size) {
    int disk = open(diskPath, O_WRONLY);
    if(disk < 0) {
        printf("%sFailed to open disk %s\n", RED, diskPath);
        return;
    }

    unsigned char buffer[1024];
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
        if(difftime(currentTime, lastPrinttime) >= 5.0) {
            printf("%sProgress: %llu/%llu bytes (%.2f%%)\n", YELLOW, totalWritten, totalWritten + size, (double)totalWritten / (double)(totalWritten + size) * 100);
            lastPrinttime = currentTime;
        }
    }
    
    close(disk);
}

void eraseDisk(struct DISK_INFO disk, int num) {
    printf("%sErasing disk %d\n", YELLOW, num + 1);
    char diskPath[50];
    snprintf(diskPath, sizeof(diskPath), "/dev/disk%d", num);

    for(int i = 0; i < 10; i++) {
        if(i < 5) {
            overwriteDisk(diskPath, false, disk.size);
        } else {
            overwriteDisk(diskPath, true, disk.size);
        }

        printf("%sPass %d complete\n", YELLOW, i + 1);
        sleep(1);
    }
}

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
                    for(int i = 0; i < numDisks; i++) {
                        eraseDisk(disks[i], i);
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
                        eraseDisk(disks[diskNum - 1], diskNum - 1);
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