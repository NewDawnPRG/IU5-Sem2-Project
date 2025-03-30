#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <SFML/Graphics.hpp>

#include "../board/board.h"

class Graphics
{
    sf::RenderWindow _window;
    Board *_board;
    sf::View view;

    const int BORDER = 30;
    const int BOTTOM_BORDER = 100;

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
