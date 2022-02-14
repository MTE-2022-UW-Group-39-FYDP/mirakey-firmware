/*
 * Maps ASCII codes for characters to a 72x40 bit frame buffer for the display (2880 bits)
 * Uses the following 8x8 font: https://www.wfonts.com/font/code-8x8
 * Upscales the 8x8 bitmaps to a 32x32 bitmap to be placed center in the 72x40 frame, see lib/glyphs/ideas/graph.svg
 */

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define FONT_ROW_TO_BYTE(s) ((s[0]=='*')<<7 | (s[1]=='*')<<6 | (s[2]=='*')<<5 | (s[3]=='*')<<4 | \
                (s[4]=='*')<<3 | (s[5]=='*')<<2 | (s[6]=='*')<<1 | (s[7]=='*')<<0)

static const unsigned char map[128][8] = {
        {},
        {},
        {},
        {},
        {},
        {},
        {},
        {},
        {},
        {},
        {},
        {},
        {},
        {},
        {},
        {},
        {},
        {},
        {},
        {},
        {},
        {},
        {},
        {},
        {},
        {},
        {},
        {},
        {},
        {},
        {},
        {},
        {},
        {},
        {},
        {},
        {},
        {},
        {},
        {},
        {},
        {},
        {},
        {},
        {},
        {},
        {},
        {},
        {},
        {},
        {},
        {},
        {},
        {},
        {},
        {},
        {},
        {},
        {},
        {},
        {},
        {},
        {},
        {},
        {},
        {
            FONT_ROW_TO_BYTE("   **   "),
            FONT_ROW_TO_BYTE("  *  *  "),
            FONT_ROW_TO_BYTE(" *    * "),
            FONT_ROW_TO_BYTE(" *    * "),
            FONT_ROW_TO_BYTE(" ****** "),
            FONT_ROW_TO_BYTE(" *    * "),
            FONT_ROW_TO_BYTE(" *    * "),
            FONT_ROW_TO_BYTE("***  ***")
        },
        {
            FONT_ROW_TO_BYTE("******  "),
            FONT_ROW_TO_BYTE(" *    * "),
            FONT_ROW_TO_BYTE(" *   *  "),
            FONT_ROW_TO_BYTE(" ****** "),
            FONT_ROW_TO_BYTE(" *     *"),
            FONT_ROW_TO_BYTE(" *     *"),
            FONT_ROW_TO_BYTE(" *     *"),
            FONT_ROW_TO_BYTE("******* ")
        },
        {
            FONT_ROW_TO_BYTE("  **** *"),
            FONT_ROW_TO_BYTE(" *    **"),
            FONT_ROW_TO_BYTE("*      *"),
            FONT_ROW_TO_BYTE("*       "),
            FONT_ROW_TO_BYTE("*       "),
            FONT_ROW_TO_BYTE("*      *"),
            FONT_ROW_TO_BYTE(" *    **"),
            FONT_ROW_TO_BYTE("  **** *")
        },
        {
            FONT_ROW_TO_BYTE("******  "),
            FONT_ROW_TO_BYTE(" *    * "),
            FONT_ROW_TO_BYTE(" *     *"),
            FONT_ROW_TO_BYTE(" *     *"),
            FONT_ROW_TO_BYTE(" *     *"),
            FONT_ROW_TO_BYTE(" *     *"),
            FONT_ROW_TO_BYTE(" *    * "),
            FONT_ROW_TO_BYTE("******  ")
        },
        {
            FONT_ROW_TO_BYTE("********"),
            FONT_ROW_TO_BYTE(" *     *"),
            FONT_ROW_TO_BYTE(" *      "),
            FONT_ROW_TO_BYTE(" ***    "),
            FONT_ROW_TO_BYTE(" *      "),
            FONT_ROW_TO_BYTE(" *      "),
            FONT_ROW_TO_BYTE(" *     *"),
            FONT_ROW_TO_BYTE("********")
        },
        {
            FONT_ROW_TO_BYTE("********"),
            FONT_ROW_TO_BYTE(" *     *"),
            FONT_ROW_TO_BYTE(" *     *"),
            FONT_ROW_TO_BYTE(" *  *   "),
            FONT_ROW_TO_BYTE(" ****   "),
            FONT_ROW_TO_BYTE(" *  *   "),
            FONT_ROW_TO_BYTE(" *      "),
            FONT_ROW_TO_BYTE("***     ")
        },
        {
            FONT_ROW_TO_BYTE("  ******"),
            FONT_ROW_TO_BYTE(" *     *"),
            FONT_ROW_TO_BYTE("*       "),
            FONT_ROW_TO_BYTE("*  *****"),
            FONT_ROW_TO_BYTE("*  *   *"),
            FONT_ROW_TO_BYTE("*      *"),
            FONT_ROW_TO_BYTE(" *     *"),
            FONT_ROW_TO_BYTE("  ***** ")
        },
        {
            FONT_ROW_TO_BYTE("***  ***"),
            FONT_ROW_TO_BYTE(" *    * "),
            FONT_ROW_TO_BYTE(" *    * "),
            FONT_ROW_TO_BYTE(" ****** "),
            FONT_ROW_TO_BYTE(" *    * "),
            FONT_ROW_TO_BYTE(" *    * "),
            FONT_ROW_TO_BYTE(" *    * "),
            FONT_ROW_TO_BYTE("***  ***")
        },
        {
            FONT_ROW_TO_BYTE("******* "),
            FONT_ROW_TO_BYTE("   *    "),
            FONT_ROW_TO_BYTE("   *    "),
            FONT_ROW_TO_BYTE("   *    "),
            FONT_ROW_TO_BYTE("   *    "),
            FONT_ROW_TO_BYTE("   *    "),
            FONT_ROW_TO_BYTE("   *    "),
            FONT_ROW_TO_BYTE("******* ")
        },
        {
            FONT_ROW_TO_BYTE("  ******"),
            FONT_ROW_TO_BYTE("     *  "),
            FONT_ROW_TO_BYTE("     *  "),
            FONT_ROW_TO_BYTE("     *  "),
            FONT_ROW_TO_BYTE("**   *  "),
            FONT_ROW_TO_BYTE("*    *  "),
            FONT_ROW_TO_BYTE("*    *  "),
            FONT_ROW_TO_BYTE(" ****   ")
        },
        {
            FONT_ROW_TO_BYTE("*** ****"),
            FONT_ROW_TO_BYTE(" *   *  "),
            FONT_ROW_TO_BYTE(" *  *   "),
            FONT_ROW_TO_BYTE(" ***    "),
            FONT_ROW_TO_BYTE(" *  *   "),
            FONT_ROW_TO_BYTE(" *   *  "),
            FONT_ROW_TO_BYTE(" *    * "),
            FONT_ROW_TO_BYTE("***  ***")
        },
        {
            FONT_ROW_TO_BYTE("***     "),
            FONT_ROW_TO_BYTE(" *      "),
            FONT_ROW_TO_BYTE(" *      "),
            FONT_ROW_TO_BYTE(" *      "),
            FONT_ROW_TO_BYTE(" *      "),
            FONT_ROW_TO_BYTE(" *     *"),
            FONT_ROW_TO_BYTE(" *     *"),
            FONT_ROW_TO_BYTE("********")
        },
        {
            FONT_ROW_TO_BYTE("**   ***"),
            FONT_ROW_TO_BYTE(" ** * * "),
            FONT_ROW_TO_BYTE(" * *  * "),
            FONT_ROW_TO_BYTE(" * *  * "),
            FONT_ROW_TO_BYTE(" *    * "),
            FONT_ROW_TO_BYTE(" *    * "),
            FONT_ROW_TO_BYTE(" *    * "),
            FONT_ROW_TO_BYTE("***  ***")
        },
        {
            FONT_ROW_TO_BYTE("**   ***"),
            FONT_ROW_TO_BYTE(" **   * "),
            FONT_ROW_TO_BYTE(" * *  * "),
            FONT_ROW_TO_BYTE(" *  * * "),
            FONT_ROW_TO_BYTE(" *   ** "),
            FONT_ROW_TO_BYTE(" *    * "),
            FONT_ROW_TO_BYTE(" *    * "),
            FONT_ROW_TO_BYTE("***  ***")
        },
        {
            FONT_ROW_TO_BYTE("  ****  "),
            FONT_ROW_TO_BYTE(" *    * "),
            FONT_ROW_TO_BYTE("*      *"),
            FONT_ROW_TO_BYTE("*      *"),
            FONT_ROW_TO_BYTE("*      *"),
            FONT_ROW_TO_BYTE("*      *"),
            FONT_ROW_TO_BYTE(" *    * "),
            FONT_ROW_TO_BYTE("  ****  ")
        },
        {
            FONT_ROW_TO_BYTE("******* "),
            FONT_ROW_TO_BYTE(" *     *"),
            FONT_ROW_TO_BYTE(" *     *"),
            FONT_ROW_TO_BYTE(" *     *"),
            FONT_ROW_TO_BYTE(" ****** "),
            FONT_ROW_TO_BYTE(" *      "),
            FONT_ROW_TO_BYTE(" *      "),
            FONT_ROW_TO_BYTE("***     ")
        },
        {
            FONT_ROW_TO_BYTE(" ****** "),
            FONT_ROW_TO_BYTE("*      *"),
            FONT_ROW_TO_BYTE("*      *"),
            FONT_ROW_TO_BYTE("*      *"),
            FONT_ROW_TO_BYTE("*      *"),
            FONT_ROW_TO_BYTE("*  *   *"),
            FONT_ROW_TO_BYTE(" ****** "),
            FONT_ROW_TO_BYTE("   *    ")
        },
        {
            FONT_ROW_TO_BYTE("******* "),
            FONT_ROW_TO_BYTE(" *     *"),
            FONT_ROW_TO_BYTE(" *     *"),
            FONT_ROW_TO_BYTE(" *     *"),
            FONT_ROW_TO_BYTE(" ****** "),
            FONT_ROW_TO_BYTE(" *   *  "),
            FONT_ROW_TO_BYTE(" *    * "),
            FONT_ROW_TO_BYTE("***  ***")
        },
        {
            FONT_ROW_TO_BYTE(" ***** *"),
            FONT_ROW_TO_BYTE("*     **"),
            FONT_ROW_TO_BYTE("*      *"),
            FONT_ROW_TO_BYTE(" *****  "),
            FONT_ROW_TO_BYTE("      * "),
            FONT_ROW_TO_BYTE("*      *"),
            FONT_ROW_TO_BYTE("**     *"),
            FONT_ROW_TO_BYTE("* ***** ")
        },
        {
            FONT_ROW_TO_BYTE("******* "),
            FONT_ROW_TO_BYTE("*  *  * "),
            FONT_ROW_TO_BYTE("   *    "),
            FONT_ROW_TO_BYTE("   *    "),
            FONT_ROW_TO_BYTE("   *    "),
            FONT_ROW_TO_BYTE("   *    "),
            FONT_ROW_TO_BYTE("   *    "),
            FONT_ROW_TO_BYTE("  ***   ")
        },
        {
            FONT_ROW_TO_BYTE("***  ***"),
            FONT_ROW_TO_BYTE(" *    * "),
            FONT_ROW_TO_BYTE(" *    * "),
            FONT_ROW_TO_BYTE(" *    * "),
            FONT_ROW_TO_BYTE(" *    * "),
            FONT_ROW_TO_BYTE(" *    * "),
            FONT_ROW_TO_BYTE(" *    * "),
            FONT_ROW_TO_BYTE("  ****  ")
        },
        {
            FONT_ROW_TO_BYTE("***  ***"),
            FONT_ROW_TO_BYTE(" *    * "),
            FONT_ROW_TO_BYTE(" *    * "),
            FONT_ROW_TO_BYTE(" *    * "),
            FONT_ROW_TO_BYTE("  *   * "),
            FONT_ROW_TO_BYTE("  *  *  "),
            FONT_ROW_TO_BYTE("   * *  "),
            FONT_ROW_TO_BYTE("    *   ")
        },
        {
            FONT_ROW_TO_BYTE("***  ***"),
            FONT_ROW_TO_BYTE(" *    * "),
            FONT_ROW_TO_BYTE(" *    * "),
            FONT_ROW_TO_BYTE(" *    * "),
            FONT_ROW_TO_BYTE(" * *  * "),
            FONT_ROW_TO_BYTE(" * *  * "),
            FONT_ROW_TO_BYTE(" * *  * "),
            FONT_ROW_TO_BYTE("  * **  ")
        },
        {
            FONT_ROW_TO_BYTE("***  ***"),
            FONT_ROW_TO_BYTE(" *    * "),
            FONT_ROW_TO_BYTE("  *  *  "),
            FONT_ROW_TO_BYTE("   **   "),
            FONT_ROW_TO_BYTE("  *  *  "),
            FONT_ROW_TO_BYTE("  *  *  "),
            FONT_ROW_TO_BYTE(" *    * "),
            FONT_ROW_TO_BYTE("***  ***")
        },
        {
            FONT_ROW_TO_BYTE("***  ***"),
            FONT_ROW_TO_BYTE(" *    * "),
            FONT_ROW_TO_BYTE("  *  *  "),
            FONT_ROW_TO_BYTE("   * *   "),
            FONT_ROW_TO_BYTE("    *   "),
            FONT_ROW_TO_BYTE("    *   "),
            FONT_ROW_TO_BYTE("    *   "),
            FONT_ROW_TO_BYTE("   ***  ")
        },
        {
            FONT_ROW_TO_BYTE("********"),
            FONT_ROW_TO_BYTE("*     * "),
            FONT_ROW_TO_BYTE("*    *  "),
            FONT_ROW_TO_BYTE("    *   "),
            FONT_ROW_TO_BYTE("   *    "),
            FONT_ROW_TO_BYTE("  *    *"),
            FONT_ROW_TO_BYTE(" *     *"),
            FONT_ROW_TO_BYTE("********")
        },
        {},
        {},
        {},
        {},
        {},
        {},
        {
            FONT_ROW_TO_BYTE("        "),
            FONT_ROW_TO_BYTE("        "),
            FONT_ROW_TO_BYTE(" *****  "),
            FONT_ROW_TO_BYTE("      * "),
            FONT_ROW_TO_BYTE(" ****** "),
            FONT_ROW_TO_BYTE("*     * "),
            FONT_ROW_TO_BYTE("*     * "),
            FONT_ROW_TO_BYTE(" *******")
        },{
            FONT_ROW_TO_BYTE("**      "),
            FONT_ROW_TO_BYTE(" *      "),
            FONT_ROW_TO_BYTE(" * **** "),
            FONT_ROW_TO_BYTE(" **    *"),
            FONT_ROW_TO_BYTE(" *     *"),
            FONT_ROW_TO_BYTE(" *     *"),
            FONT_ROW_TO_BYTE(" *     *"),
            FONT_ROW_TO_BYTE("******* ")
        },
        {
            FONT_ROW_TO_BYTE("        "),
            FONT_ROW_TO_BYTE("        "),
            FONT_ROW_TO_BYTE(" ****** "),
            FONT_ROW_TO_BYTE("*      *"),
            FONT_ROW_TO_BYTE("*       "),
            FONT_ROW_TO_BYTE("*      *"),
            FONT_ROW_TO_BYTE("*      *"),
            FONT_ROW_TO_BYTE(" ****** ")
        },
        {
            FONT_ROW_TO_BYTE("       *"),
            FONT_ROW_TO_BYTE("       *"),
            FONT_ROW_TO_BYTE(" ***** *"),
            FONT_ROW_TO_BYTE("*     **"),
            FONT_ROW_TO_BYTE("*      *"),
            FONT_ROW_TO_BYTE("*      *"),
            FONT_ROW_TO_BYTE("*     **"),
            FONT_ROW_TO_BYTE(" ***** *")
        },
        {
            FONT_ROW_TO_BYTE("        "),
            FONT_ROW_TO_BYTE("        "),
            FONT_ROW_TO_BYTE(" ****** "),
            FONT_ROW_TO_BYTE("*      *"),
            FONT_ROW_TO_BYTE("********"),
            FONT_ROW_TO_BYTE("*       "),
            FONT_ROW_TO_BYTE("*      *"),
            FONT_ROW_TO_BYTE(" ****** ")
        },
        {
            FONT_ROW_TO_BYTE("   **** "),
            FONT_ROW_TO_BYTE("  *    *"),
            FONT_ROW_TO_BYTE("  *    *"),
            FONT_ROW_TO_BYTE(" ****   "),
            FONT_ROW_TO_BYTE("  *     "),
            FONT_ROW_TO_BYTE("  *     "),
            FONT_ROW_TO_BYTE("  *     "),
            FONT_ROW_TO_BYTE("*****   ")
        },
        {
            FONT_ROW_TO_BYTE("      **"),
            FONT_ROW_TO_BYTE(" ****** "),
            FONT_ROW_TO_BYTE("*      *"),
            FONT_ROW_TO_BYTE("******* "),
            FONT_ROW_TO_BYTE(" *      "),
            FONT_ROW_TO_BYTE(" ****** "),
            FONT_ROW_TO_BYTE("*      *"),
            FONT_ROW_TO_BYTE(" ****** ")
        },
        {
            FONT_ROW_TO_BYTE("***     "),
            FONT_ROW_TO_BYTE(" *      "),
            FONT_ROW_TO_BYTE(" * ***  "),
            FONT_ROW_TO_BYTE(" **   * "),
            FONT_ROW_TO_BYTE(" *    * "),
            FONT_ROW_TO_BYTE(" *    * "),
            FONT_ROW_TO_BYTE(" *    * "),
            FONT_ROW_TO_BYTE("***  ***")
        },
        {
            FONT_ROW_TO_BYTE("   *    "),
            FONT_ROW_TO_BYTE("        "),
            FONT_ROW_TO_BYTE("  **    "),
            FONT_ROW_TO_BYTE("   *    "),
            FONT_ROW_TO_BYTE("   *    "),
            FONT_ROW_TO_BYTE("   *    "),
            FONT_ROW_TO_BYTE("   *    "),
            FONT_ROW_TO_BYTE(" *****  ")
        },
        {
            FONT_ROW_TO_BYTE("       *"),
            FONT_ROW_TO_BYTE("        "),
            FONT_ROW_TO_BYTE("     ***"),
            FONT_ROW_TO_BYTE("       *"),
            FONT_ROW_TO_BYTE("       *"),
            FONT_ROW_TO_BYTE("       *"),
            FONT_ROW_TO_BYTE("*      *"),
            FONT_ROW_TO_BYTE(" ****** ")
        },
        {
            FONT_ROW_TO_BYTE("***     "),
            FONT_ROW_TO_BYTE(" *  *** "),
            FONT_ROW_TO_BYTE(" *   *  "),
            FONT_ROW_TO_BYTE(" ****   "),
            FONT_ROW_TO_BYTE(" *  *   "),
            FONT_ROW_TO_BYTE(" *   *  "),
            FONT_ROW_TO_BYTE(" *    * "),
            FONT_ROW_TO_BYTE("***  ***")
        },
        {
            FONT_ROW_TO_BYTE("  **    "),
            FONT_ROW_TO_BYTE("   *    "),
            FONT_ROW_TO_BYTE("   *    "),
            FONT_ROW_TO_BYTE("   *    "),
            FONT_ROW_TO_BYTE("   *    "),
            FONT_ROW_TO_BYTE("   *    "),
            FONT_ROW_TO_BYTE("   *    "),
            FONT_ROW_TO_BYTE("******* ")
        },
        {
            FONT_ROW_TO_BYTE("        "),
            FONT_ROW_TO_BYTE("        "),
            FONT_ROW_TO_BYTE("* ** ** "),
            FONT_ROW_TO_BYTE(" *  *  *"),
            FONT_ROW_TO_BYTE(" *  *  *"),
            FONT_ROW_TO_BYTE(" *  *  *"),
            FONT_ROW_TO_BYTE(" *  *  *"),
            FONT_ROW_TO_BYTE("** ** **")
        },
        {
            FONT_ROW_TO_BYTE("        "),
            FONT_ROW_TO_BYTE("        "),
            FONT_ROW_TO_BYTE("** ***  "),
            FONT_ROW_TO_BYTE(" **   * "),
            FONT_ROW_TO_BYTE(" *    * "),
            FONT_ROW_TO_BYTE(" *    * "),
            FONT_ROW_TO_BYTE(" *    * "),
            FONT_ROW_TO_BYTE("***  ***")
        },
        {
            FONT_ROW_TO_BYTE("        "),
            FONT_ROW_TO_BYTE("        "),
            FONT_ROW_TO_BYTE(" ****** "),
            FONT_ROW_TO_BYTE("*      *"),
            FONT_ROW_TO_BYTE("*      *"),
            FONT_ROW_TO_BYTE("*      *"),
            FONT_ROW_TO_BYTE("*      *"),
            FONT_ROW_TO_BYTE(" ****** ")
        },
        {
            FONT_ROW_TO_BYTE("        "),
            FONT_ROW_TO_BYTE("******* "),
            FONT_ROW_TO_BYTE(" *     *"),
            FONT_ROW_TO_BYTE(" *     *"),
            FONT_ROW_TO_BYTE(" *     *"),
            FONT_ROW_TO_BYTE(" ****** "),
            FONT_ROW_TO_BYTE(" *      "),
            FONT_ROW_TO_BYTE("***     ")
        },
        {
            FONT_ROW_TO_BYTE("        "),
            FONT_ROW_TO_BYTE(" ****** "),
            FONT_ROW_TO_BYTE("*     * "),
            FONT_ROW_TO_BYTE("*     * "),
            FONT_ROW_TO_BYTE("*     * "),
            FONT_ROW_TO_BYTE(" ****** "),
            FONT_ROW_TO_BYTE("      * "),
            FONT_ROW_TO_BYTE("     ***")
        },
        {
            FONT_ROW_TO_BYTE("        "),
            FONT_ROW_TO_BYTE("        "),
            FONT_ROW_TO_BYTE("** ***  "),
            FONT_ROW_TO_BYTE(" **   * "),
            FONT_ROW_TO_BYTE(" *    * "),
            FONT_ROW_TO_BYTE(" *      "),
            FONT_ROW_TO_BYTE(" *      "),
            FONT_ROW_TO_BYTE("***     ")
        },
        {
            FONT_ROW_TO_BYTE("        "),
            FONT_ROW_TO_BYTE("        "),
            FONT_ROW_TO_BYTE(" ****** "),
            FONT_ROW_TO_BYTE("*       "),
            FONT_ROW_TO_BYTE(" ****** "),
            FONT_ROW_TO_BYTE("       *"),
            FONT_ROW_TO_BYTE("*      *"),
            FONT_ROW_TO_BYTE("******* ")
        },
        {
            FONT_ROW_TO_BYTE("  *     "),
            FONT_ROW_TO_BYTE("  *     "),
            FONT_ROW_TO_BYTE("*****   "),
            FONT_ROW_TO_BYTE("  *     "),
            FONT_ROW_TO_BYTE("  *     "),
            FONT_ROW_TO_BYTE("  *     "),
            FONT_ROW_TO_BYTE("  *   * "),
            FONT_ROW_TO_BYTE("   ***  ")
        },
        {
            FONT_ROW_TO_BYTE("        "),
            FONT_ROW_TO_BYTE("        "),
            FONT_ROW_TO_BYTE("***  ***"),
            FONT_ROW_TO_BYTE(" *    * "),
            FONT_ROW_TO_BYTE(" *    * "),
            FONT_ROW_TO_BYTE(" *    * "),
            FONT_ROW_TO_BYTE(" *    * "),
            FONT_ROW_TO_BYTE("  ****  ")
        },
        {
            FONT_ROW_TO_BYTE("        "),
            FONT_ROW_TO_BYTE("        "),
            FONT_ROW_TO_BYTE("***  ***"),
            FONT_ROW_TO_BYTE(" *    * "),
            FONT_ROW_TO_BYTE("  *  * "),
            FONT_ROW_TO_BYTE("  *  * "),
            FONT_ROW_TO_BYTE("   * * "),
            FONT_ROW_TO_BYTE("    *   ")
        },
        {
            FONT_ROW_TO_BYTE("        "),
            FONT_ROW_TO_BYTE("        "),
            FONT_ROW_TO_BYTE("***  ***"),
            FONT_ROW_TO_BYTE(" *    * "),
            FONT_ROW_TO_BYTE(" * *  * "),
            FONT_ROW_TO_BYTE(" * *  * "),
            FONT_ROW_TO_BYTE(" * *  * "),
            FONT_ROW_TO_BYTE("  * **  ")
        },
        {
            FONT_ROW_TO_BYTE("        "),
            FONT_ROW_TO_BYTE("        "),
            FONT_ROW_TO_BYTE("***  ***"),
            FONT_ROW_TO_BYTE("  *  *  "),
            FONT_ROW_TO_BYTE("   **   "),
            FONT_ROW_TO_BYTE("  *  *  "),
            FONT_ROW_TO_BYTE(" *    * "),
            FONT_ROW_TO_BYTE("***  ***")
        },
        {
            FONT_ROW_TO_BYTE("        "),
            FONT_ROW_TO_BYTE("        "),
            FONT_ROW_TO_BYTE("***  ***"),
            FONT_ROW_TO_BYTE(" *    * "),
            FONT_ROW_TO_BYTE(" *    * "),
            FONT_ROW_TO_BYTE("  ***** "),
            FONT_ROW_TO_BYTE("*     * "),
            FONT_ROW_TO_BYTE(" *****  ")
        },
        {
            FONT_ROW_TO_BYTE("        "),
            FONT_ROW_TO_BYTE("        "),
            FONT_ROW_TO_BYTE("********"),
            FONT_ROW_TO_BYTE("*      *"),
            FONT_ROW_TO_BYTE("    *** "),
            FONT_ROW_TO_BYTE(" ***    "),
            FONT_ROW_TO_BYTE("*      *"),
            FONT_ROW_TO_BYTE("********")
        },
        {},
        {},
        {},
        {},
        {}
};

void render8x8(const char *bitmap) {
    int set;
    for (int x = 0; x < 8; x++) {
        for (int y = 0; y < 8; y++) {
            set = bitmap[x] & 1 << (7 - y);
            printf("%c", set ? 'X' : ' ');
        }
        printf("\n");
    }
}

void render32x32(const uint32_t *bitmap) {
    int set;
    for (int x = 0; x < 32; x++) {
        for (int y = 0; y < 32; y++) {
            set = bitmap[x] & 1 << (31 - y);
            printf("%c", set ? 'X' : ' ');
        }
        printf("\n");
    }
}

uint32_t* upscale8to32(const char *bitmap) {
    static uint32_t result[32];
    memset(result, 0, sizeof(result));
    int set;
    int count_to_32 = 0;
    int curr_row = 0;
    for (int x = 0; x < 8; x++) {
        for (int y = 0; y < 8; y++) {
            set = bitmap[x] & 1 << (7 - y);
            for (int i = 0; i < 4; i++) {
                result[curr_row] |= set ? (1 << (31 - count_to_32)) : 0;
                count_to_32++;
            }
            if (count_to_32 >= 31) {
                count_to_32 = 0;
                // Duplicate this row 3 more times
                for (int j = 0; j < 3; j++) {
                    curr_row++;
                    result[curr_row] = result[curr_row - 1];
                }
                curr_row++;
            }
        }
    }
    return result;
}

int main() {

    int ascii_code = (int)'A';
    for (; ascii_code <= 122; ascii_code++) {
        char *bitmap8x8 = map[ascii_code];
        render8x8(bitmap8x8);
        printf("\n");
        uint32_t *bitmap32x32 = upscale8to32(bitmap8x8);
        render32x32(bitmap32x32);
        printf("\n");
    }

    return 0;
}
