#ifndef MAIN_H
#define MAIN_H

#define MAX_DISKS 16

struct DISK_INFO {
    unsigned int size;
};

typedef struct {
    void (*getDisks)(struct DISK_INFO[], int*);
} DiskManager;

#endif