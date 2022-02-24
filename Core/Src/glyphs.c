/*
 * Maps ASCII codes for characters to a 72x40 bit frame buffer for the display (2880 bits).
 * Uses the following 8x8 font: https://www.wfonts.com/font/code-8x8.
 * Upscales the 8x8 bitmaps to a 32x32 bitmap to be placed center in the 72x40 frame, see lib/glyphs/ideas/graph.svg.
 * The bit frames are ordered in rows, then columns.
 */

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "glyphs.h"

#define FONT_ROW_TO_BYTE(s) ((s[0]=='*')<<7 | (s[1]=='*')<<6 | (s[2]=='*')<<5 | (s[3]=='*')<<4 | \
                (s[4]=='*')<<3 | (s[5]=='*')<<2 | (s[6]=='*')<<1 | (s[7]=='*')<<0)

static const uint8_t map[128][8] = {
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

/*
 * Rendering functions to help debug/test and visualize
 */
void render8x8(const uint8_t *bitmap) {
    int set;
    for (int row = 0; row < 8; row++) {
        for (int column = 0; column < 8; column++) {
            set = bitmap[row] & 1 << (7 - column);
            printf("%c", set ? 'X' : ' ');
        }
        printf("\n");
    }
}

void render32x32(const uint32_t *bitmap) {
    int set;
    for (int row = 0; row < 32; row++) {
        for (int column = 0; column < 32; column++) {
            set = bitmap[row] & 1 << (31 - column);
            printf("%c", set ? 'X' : ' ');
        }
        printf("\n");
    }
}

void render72x40(const uint8_t *bitmap) {
    int set;
    printf("__________________________________________________________________________\n");
    for (int row = 0; row < 40; row++) {
        printf("|");
        for (int columnByte = 0; columnByte < 9; columnByte++) {
            for (int i = 0; i < 8; i++) {
                set = bitmap[row*9 + columnByte] & 1 << (7 - i);
                printf("%c", set ? 'X' : ' ');
            }
        }
        printf("|\n");
    }
    printf("__________________________________________________________________________\n");
}

uint32_t* upscale8to32(const uint8_t *bitmap) {
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

/*
 * Pad with 0s where needed to achieve 72x40 frame
 */
uint8_t* center32x32in72x40(const uint32_t *bitmap) {
    static uint8_t result[360];
    memset(result, 0, sizeof(result));

    // To be centered, glyph should start at halfway through byte 39
    int start = 38;
    for (int i = 0; i < 32; i++) {
        result[start] |= bitmap[i] >> 28;
        result[start+1] |= bitmap[i] >> 20;
        result[start+2] |= bitmap[i] >> 12;
        result[start+3] |= bitmap[i] >> 4;
        result[start+4] |= bitmap[i] << 4;
        start+=9;
    }

    return result;
}

uint8_t* mapCharToBitmap(uint8_t c) {
    int ascii_code = (int)c;
    uint8_t *bitmap8x8 = map[ascii_code];
    uint32_t *bitmap32x32 = upscale8to32(bitmap8x8);
    return center32x32in72x40(bitmap32x32);
}

//int main() {
//
//    int ascii_code = (int)'A';
//    for (; ascii_code <= 122; ascii_code++) {
//        uint8_t *bitmap8x8 = map[ascii_code];
//        render8x8(bitmap8x8);
//        printf("\n");
//        uint32_t *bitmap32x32 = upscale8to32(bitmap8x8);
//        render32x32(bitmap32x32);
//        printf("\n");
//        uint8_t *bitmap72x40 = center32x32in72x40(bitmap32x32);
//        render72x40(bitmap72x40);
//        printf("\n");
//    }
//
//    return 0;
//}
