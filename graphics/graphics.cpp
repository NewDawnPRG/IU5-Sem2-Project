#include "graphics.h"

#include <string>
#include <cmath>
#include <iostream>

Graphics::Graphics(Board *board) {
    std::string title = "Сёги";
    _window = sf::RenderWindow(sf::VideoMode({600, 800}), sf::String::fromUtf8(title.begin(), title.end()));
    _window.setMinimumSize(sf::Vector2u(480, 600));

    view = _window.getDefaultView();
    _window.setView(view);

    _window.setFramerateLimit(30);

    _board = board;
}

void Graphics::redraw() {
    auto size = _window.getSize();

    auto fieldSize = sf::Vector2u(size.x - 2 * BORDER, size.y - 2 * BORDER);
    auto cellSize = std::min((fieldSize.x - 2 * BORDER) / 9,
                             (fieldSize.y - 2 * BORDER - 2 * RESERVE_BORDER) / 13);
    int xPadding = (fieldSize.x - cellSize * 9) / 2;

    _window.clear(sf::Color(139, 69, 19));
    drawField(size, cellSize, xPadding);
    drawFigures(size, cellSize, xPadding);
    _window.display();
}

void Graphics::drawField(sf::Vector2u &size, unsigned int &cellSize, int &xPadding) {
    sf::RectangleShape rect;
    rect.setFillColor(sf::Color::Transparent);
    rect.setOutlineColor(sf::Color::Blue);
    rect.setOutlineThickness(LINE_WIDTH);

    // up reserve
    rect.setSize({cellSize * 9, cellSize * 2});
    rect.setPosition({BORDER + xPadding, BORDER});
    _window.draw(rect);

    // field
    sf::Vector2f startPoint{BORDER + xPadding, (size.y / 2) - (4.5 * cellSize)};
    for (int i = 0; i <= 9; ++i) {
        drawLine({startPoint.x + cellSize * i, startPoint.y},
                 {startPoint.x + cellSize * i, startPoint.y + 9 * cellSize},
                 LINE_WIDTH, sf::Color::White); // vertical line
        drawLine({startPoint.x, startPoint.y + cellSize * i},
                 {startPoint.x + 9 * cellSize, startPoint.y + cellSize * i},
                 LINE_WIDTH, sf::Color::White); // horizontal line
    }

    // down reserve
    rect.setPosition({BORDER + xPadding, size.y - BORDER - 2 * cellSize});
    _window.draw(rect);
}

void Graphics::drawFigures(sf::Vector2u &size, unsigned int &cellSize, int &xPadding) {
    sf::Vector2f startPoint{BORDER + xPadding, (size.y / 2) - (4.5 * cellSize)};
    for (int x = 0; x < 9; ++x) {
        for (int y = 0; y < 9; ++y) {
            int figure = _board->GetCell(y, x);
            if (std::abs(figure) > 0 && std::abs(figure) < 9) {
                sf::Texture texture("img/" + figures[figure], false);
                float scale = static_cast<float>(cellSize) / std::max(texture.getSize().x, texture.getSize().y);
                sf::Sprite sprite(texture);
                sprite.setPosition({startPoint.x + cellSize * x + (cellSize - texture.getSize().x * scale) / 2,
                                    startPoint.y + cellSize * y + (cellSize - texture.getSize().y * scale) / 2});
                sprite.setScale({scale, scale});
                _window.draw(sprite);
            }
        }
    }
}

void Graphics::closeWindow() {
    _window.close();
}

int Graphics::getEvents() {
    int result = 0;
    while (const std::optional event = _window.pollEvent()) {
        if (event->is<sf::Event::Closed>())
            result += EVENT_CLOSE;
        if (const auto *resized = event->getIf<sf::Event::Resized>()) {
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