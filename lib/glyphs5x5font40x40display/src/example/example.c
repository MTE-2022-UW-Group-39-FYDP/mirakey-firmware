#include <stdio.h>
#include "../glyphs.h"

int main() {
    unsigned char* bitmap = mapCharToBitmap(146);
    render40x40(bitmap);
//    printf("\n");
//    bitmap = mapCharToBitmap('0');
//    render40x40(bitmap);
//    printf("\n");
//    bitmap = mapCharToBitmap(8);
//    render40x40(bitmap);
}