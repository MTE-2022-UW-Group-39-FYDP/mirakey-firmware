#include <stdio.h>
#include <stdint.h>

// Need to create a 72x40 bit frame buffer for the display (2880 bits)
typedef uint8_t frame[360];
//frame buildFrame(char c);

int main() {
    printf("%lu\n", sizeof(frame));

    return 0;
}