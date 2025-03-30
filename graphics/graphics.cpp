#include "graphics.h"

#include <iostream>
#include <cmath>

Graphics::Graphics(Board *board)
{
    _window = sf::RenderWindow(sf::VideoMode({600, 800}), "Сёги");
    _window.setMinimumSize(sf::Vector2u(480, 600));

    view = _window.getDefaultView();
    _window.setView(view);

    _board = board;
}

void Graphics::redraw()
{
    auto size = _window.getSize();
    auto fieldSize = sf::Vector2u(size.x - 2 * BORDER, size.y - BORDER - BOTTOM_BORDER);
    int rectSize = std::min(fieldSize.x, fieldSize.y);
    int xPadding = (fieldSize.x - rectSize) / 2;
    float cellSize = rectSize / 9.0;

    _window.clear(sf::Color(139, 69, 19));

    for (int i = 0; i <= 9; ++i)
    {
        drawLine({BORDER + xPadding + cellSize * i, BORDER},
                 {BORDER + xPadding + cellSize * i, BORDER + rectSize}, 3, sf::Color::White);
        drawLine({BORDER + xPadding, BORDER + cellSize * i},
                 {BORDER + xPadding + rectSize, BORDER + cellSize * i}, 3, sf::Color::White);
    }

    _window.display();
}

void Graphics::closeWindow()
{
    _window.close();
}

int Graphics::getEvents()
{
    int result = 0;
    while (const std::optional event = _window.pollEvent())
    {
        if (event->is<sf::Event::Closed>())
            result += EVENT_CLOSE;
        if (const auto *resized = event->getIf<sf::Event::Resized>())
        {
            sf::FloatRect view({0, 0}, {resized->size.x, resized->size.y});
            _window.setView(sf::View(view));
        }
    }

    return result;
}

void Graphics::drawLine(sf::Vector2f begin, sf::Vector2f end, int width, sf::Color color) {
    sf::Vector2f direction = end - begin;
    sf::Vector2f unitDirection = direction / std::sqrt(direction.x * direction.x + direction.y * direction.y);
    sf::Vector2f unitPerpendicular(-unitDirection.y, unitDirection.x);

    sf::Vector2f offset = (static_cast<float>(width) / 2.0f) * unitPerpendicular;

    sf::VertexArray line(sf::PrimitiveType::TriangleStrip, 4);
    
    line[0].position = begin - offset;
    line[1].position = begin + offset;
    line[2].position = end - offset;
    line[3].position = end + offset;

    line[0].color = color;
    line[1].color = color;
    line[2].color = color;
    line[3].color = color;

    _window.draw(line); 
}