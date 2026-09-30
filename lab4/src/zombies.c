#include <unistd.h>
#include <stdlib.h>

int main() {
        for (int i = 0; i < 16; i++) {
                if (fork() == 0) {
                        exit(0);
                }
        }
        while (1);
}

// to kill:
// ps aux | grep zombies
// kill -9 $parentPID