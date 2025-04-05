#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <SFML/Graphics.hpp>

#include "../board/board.h"

#include <map>

class Graphics {
    sf::RenderWindow _window;
    Board *_board;
    sf::View view;

    sf::Vector2u size;
    unsigned int cellSize;
    int xPadding;
    sf::Vector2f startPoint;

    const int LINE_WIDTH = 2;
    const int BORDER = 30;
    const int RESERVE_BORDER = 30;

    std::map<int, std::string> figures = {
            {8,  "BOU.png"}, // Чёрный король
            {7,  "BHI.png"}, // Чёрная ладья
            {6,  "BKA.png"}, // Чёрный слон
            {5,  "BGI.png"}, // Чёрный золотой генерал
            {4,  "BKI.png"}, // Чёрный серебряный генерал
            {3,  "BKE.png"}, // Чёрный конь
            {2,  "BKY.png"}, // Чёрное копьё
            {1,  "BFU.png"}, // Чёрная пешка
            {-1, "WFU.png"},  // Белая пешка
            {-2, "WKY.png"},  // Белое копьё
            {-3, "WKE.png"},  // Белый конь
            {-4, "WKI.png"},  // Белый серебряный генерал
            {-5, "WGI.png"},  // Белый золотой генерал
            {-6, "WKA.png"},  // Белый слон
            {-7, "WHI.png"},  // Белая ладья
            {-8, "WOU.png"}   // Белый король
    };
    int _selectedX = -1;
    int _selectedY = -1;

public:
    Graphics(Board *board);

    const int EVENT_CLOSE = 0x1;

    void redraw();

    void closeWindow();

    int getEvents();

    void processMouse();

private:
    void drawLine(sf::Vector2f begin, sf::Vector2f end, int width, sf::Color color);

    void drawField();

    void drawFigures();

    void updateDrawingValues();
};

#endif // GRAPHICS_H
