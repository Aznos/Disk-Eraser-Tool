#ifndef MAIN_H
#define MAIN_H

#define MAX_DISKS 16
#define KB (1024ULL)
#define MB (1024 * KB)
#define GB (1024 * MB)
#define TB (1024 * GB)
#define BUFFER_SIZE (1024 * 1024 * 32) // 32MB

struct DISK_INFO {
    unsigned long long size;
};

typedef struct {
    void (*getDisks)(struct DISK_INFO[], int*);
} DiskManager;

#endif