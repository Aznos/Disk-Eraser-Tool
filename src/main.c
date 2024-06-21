#include <stdio.h>
#include <string.h>
#include "include/colors.h"
#include "include/main.h"

void getDisks(struct DISK_INFO disks[]) {
    disks[0].size = 512;
    disks[1].size = 1024;
    disks[2].size = 20480000;
}

int main(int argc, char** argv) {
    DiskManager diskManager = {getDisks};
    
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
            struct DISK_INFO disks[DISKS];
            diskManager.getDisks(disks);
            for(int i = 0; i < DISKS; i++) {
                unsigned int diskNum = i + 1;
                if(disks[i].size >= 1024 * 1024) {
                    printf("%sDisk Number: %s%u\n%sDisk Size: %s%u TB\n\n", CYAN, BCYAN, diskNum, CYAN, BCYAN, disks[i].size / (1024 * 1024));
                } else if(disks[i].size >= 1024) {
                    printf("%sDisk Number: %s%u\n%sDisk Size: %s%u GB\n\n", CYAN, BCYAN, diskNum, CYAN, BCYAN, disks[i].size / 1024);
                } else {
                    printf("%sDisk Number: %s%u\n%sDisk Size: %s%u MB\n\n", CYAN, BCYAN, diskNum, CYAN, BCYAN, disks[i].size);
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