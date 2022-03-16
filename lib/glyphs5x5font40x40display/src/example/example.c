#include <stdio.h>
#include "../glyphs.h"

int main() {
    unsigned char* bitmap = mapCharToBitmap(' ');
    render40x40(bitmap);
    printf("\n");
    bitmap = mapCharToBitmap('0');
    render40x40(bitmap);
    printf("\n");
    bitmap = mapCharToBitmap('a');
    render40x40(bitmap);
}