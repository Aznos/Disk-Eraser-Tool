#ifndef UTIL_H
#define UTIL_H

#define PROGRESS_BAR_WIDTH 50

#include "main.h"

void getDisks(struct DISK_INFO disks[], int* numDisks);
void printProgress(unsigned long long totalWritten, unsigned long long totalSize, double elapsedTime);

#endif