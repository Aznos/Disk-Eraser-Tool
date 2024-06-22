#ifndef MAIN_H
#define MAIN_H

#define MAX_DISKS 16
#define BUFFER_SIZE (1024ULL * 1024ULL * 1024ULL * 4ULL) //4GB
#define KB (1024ULL)
#define MB (1024 * KB)
#define GB (1024 * MB)
#define TB (1024 * GB)

struct DISK_INFO {
    unsigned long long size;
};

typedef struct {
    void (*getDisks)(struct DISK_INFO[], int*);
} DiskManager;

#endif