#include <stdio.h>

int main(int argc, char** argv) {
    printf("\e[1;1H\e[2J"); //Clear terminal

    printf("!!! WARNING !!!\n");
    printf("Using this tool will PERMANENTLY DELETE all the contents in a disk and make them UNRECOVERABLE\nIf you are sure you want to proceed, type \"I understand\" and press Enter\n");

    return 0;
}