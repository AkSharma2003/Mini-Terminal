#include "commands.h"
#include <stdio.h>
#include <stdlib.h>

void listFiles() {
    #ifdef _WIN32
        system("dir");
    #else
        system("ls");
    #endif
}
