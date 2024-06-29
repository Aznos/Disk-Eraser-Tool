#ifndef DISK_H
#define DISK_H

#define BUFFER_SIZE (1024 * 1024 * 32) // 32MB

#include "main.h"

void eraseDisk(struct DISK_INFO disk, int passes);

#endif