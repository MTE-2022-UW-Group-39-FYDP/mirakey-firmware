#include "../glyphs.h"

int main() {
    unsigned char* bitmap = mapCharToBitmap('c');
    render72x40(bitmap);
}
