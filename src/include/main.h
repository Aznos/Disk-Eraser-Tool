#ifndef MAIN_H
#define MAIN_H

#define DISKS 3

struct DISK_INFO {
    unsigned int size;
};

typedef struct {
    void (*getDisks)(struct DISK_INFO[]);
} DiskManager;

#endif