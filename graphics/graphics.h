#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <SFML/Graphics.hpp>

#include "../board/board.h"

#include <map>

#include "../piece/piece.h"

class Graphics
{
    sf::RenderWindow _window;
    Board* _board;
    Piece* _piece;

    sf::Vector2u size;
    float cellSize;
    float xPadding;
    sf::Vector2f startPoint;
    sf::Vector2f endPoint;

    const float LINE_WIDTH = 2;
    const float BORDER = 30;
    const float RESERVE_BORDER = 30;

    const sf::Color clrBackground{130, 69, 19};
    const sf::Color clrSelection{48, 24, 7};

    std::map<int, std::string> figures = {
        {8, "BOU.png"}, // Чёрный король
        {3, "BHI.png"}, // Чёрная ладья
        {2, "BKA.png"}, // Чёрный слон
        {7, "BGI.png"}, // Чёрный золотой генерал
        {6, "BKI.png"}, // Чёрный серебряный генерал
        {5, "BKE.png"}, // Чёрный конь
        {4, "BKY.png"}, // Чёрное копьё
        {1, "BFU.png"}, // Чёрная пешка
        {-1, "WFU.png"}, // Белая пешка
        {-4, "WKY.png"}, // Белое копьё
        {-5, "WKE.png"}, // Белый конь
        {-6, "WKI.png"}, // Белый серебряный генерал
        {-7, "WGI.png"}, // Белый золотой генерал
        {-2, "WKA.png"}, // Белый слон
        {-3, "WHI.png"}, // Белая ладья
        {-8, "WOU.png"} // Белый король
    };
    int _selectedX = -1;
    int _selectedY = -1;

public:
    Graphics(Board* board);

    const int EVENT_CLOSE = 0x1;

    void redraw();

    void closeWindow();

    int getEvents();

    void onLeftClicked();

private:
    void drawLine(sf::Vector2f begin, sf::Vector2f end, float width, sf::Color color);

    void drawTriangle(sf::Vector2f point1, sf::Vector2f point2, sf::Vector2f point3, sf::Color color);

    void drawFigure(const sf::Vector2f& position, const int& figure);

    void drawSelection();

    void drawField();

    void drawFigures();

    void drawCurrentMove();

    void updateDrawingValues();

    void move(int newInd1, int newInd2);
};

#endif // GRAPHICS_H
