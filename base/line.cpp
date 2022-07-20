#include "tgaimage.h"

void draw_line(int x0, int x1, int y0, int y1, TGAImage image, TGAColor color) {
    int dx = std::abs(x0 - x1);
    int dy = std::abs(y0 - y1); 
    if (dx < dy) {
        std::swap(x0, y0);
        std::swap(x1, y1);
    }
    if (x0 > x1) std::swap(x0, x1);
    int terror = 0, y = y0;
    for (int x = x0; x <= x1; x++) {
        image.set(x, y, color);
        terror += dy;
        if (terror > 2 * dx)
            y++;
    }
}