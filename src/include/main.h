#ifndef MAIN_H
#define MAIN_H

#define MAX_DISKS 9
#define KB (1024ULL)
#define MB (1024 * KB)
#define GB (1024 * MB)
#define TB (1024 * GB)

int numDisks;

struct DISK_INFO {
    unsigned long long size;
    char path[256];
};

typedef struct {
    void (*getDisks)(struct DISK_INFO[], int*);
} DiskManager;

#endif