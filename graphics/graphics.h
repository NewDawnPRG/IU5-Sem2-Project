#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <SFML/Graphics.hpp>

#include "../board/board.h"

class Graphics
{
    sf::RenderWindow _window;
    Board *_board;
    sf::View view;

    const int LINE_WIDTH = 2;
    const int BORDER = 30;
    const int RESERVE_BORDER = 30;

public:
    Graphics(Board *board);

    const int EVENT_CLOSE = 0x1;

    void redraw();

    void closeWindow();

    int getEvents();

private:
    void drawLine(sf::Vector2f begin, sf::Vector2f end, int width, sf::Color color);
};

#endif // GRAPHICS_H
