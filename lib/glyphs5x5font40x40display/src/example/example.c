#include <stdio.h>
#include "../glyphs.h"

int main() {
    unsigned char* bitmap = mapCharToBitmap('A');
    render40x40(bitmap);
    printf("\n");
    bitmap = mapCharToBitmap('B');
    render40x40(bitmap);
    printf("\n");
    bitmap = mapCharToBitmap('C');
    render40x40(bitmap);
}