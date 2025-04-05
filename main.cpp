#include <iostream>

#include "graphics/graphics.h"
#include "board/board.h"

int main(){
    // Sample code
    Board *board = new Board();
    auto g = Graphics(board);

    int events = g.getEvents();
    while ((events & g.EVENT_CLOSE) != 1) {
        events = g.getEvents();
        g.processMouse();
        g.redraw();
    }
    g.closeWindow();

    return 0;
}
