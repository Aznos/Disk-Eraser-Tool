#include <stdio.h>
#include "include/colors.h"

int main(int argc, char** argv) {
    printf(CLEAR_TERMINAL);
    printf("%s%s!!! WARNING !!!\n", RED, REDB);
    printf("%sUsing this tool will %sPERMANENTLY DELETE %sall the contents in a disk and make them %sUNRECOVERABLE%s\nIf you are sure you want to proceed, type \"I understand\" and press Enter\n", TRANSPARENTB, BHRED, RED, BHRED, RED);

    return 0;
}