#include "graphics.h"

#include <string>
#include <cmath>
#include <iostream>

Graphics::Graphics(Board *board) {
    std::string title = "Сёги";
    _window = sf::RenderWindow(sf::VideoMode({600, 800}),
                               sf::String::fromUtf8(title.begin(), title.end()));
    _window.setMinimumSize(sf::Vector2u(480, 600));
    _window.setFramerateLimit(30);

    _board = board;

    updateDrawingValues();
}

void Graphics::redraw() {
    _window.clear(clrBackground);
    drawField();
    drawSelection();
    drawFigures();
    drawCurrentMove();
    _window.display();
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
            sf::FloatRect r({0, 0},
                            {(float) resized->size.x, (float) resized->size.y});
            _window.setView(sf::View(r));
            updateDrawingValues();
        }
    }

    return result;
}

void Graphics::processMouse() {
    if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
        sf::Vector2i pos = sf::Mouse::getPosition(_window);
        if (startPoint.x <= pos.x && pos.x <= endPoint.x &&
            startPoint.y <= pos.y && pos.y <= endPoint.y) {
            _selectedX = (pos.x - startPoint.x) / cellSize;
            _selectedY = (pos.y - startPoint.y) / cellSize;
        }
    }
}

void Graphics::drawLine(sf::Vector2f begin, sf::Vector2f end, float width, sf::Color color) {
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

void Graphics::drawTriangle(sf::Vector2f point1, sf::Vector2f point2, sf::Vector2f point3, sf::Color color) {
    sf::ConvexShape triangle;
    triangle.setPointCount(3);

    triangle.setPoint(0, point1);
    triangle.setPoint(1, point2);
    triangle.setPoint(2, point3);

    triangle.setFillColor(color);
    _window.draw(triangle);
}

void Graphics::drawSelection() {
    if (_selectedX != -1 && _selectedY != -1 && _board->GetCell(_selectedY, _selectedX) != 0) {
        sf::RectangleShape rect;
        rect.setSize({cellSize, cellSize});
        rect.setPosition({startPoint.x + cellSize * (float) _selectedX,
                          startPoint.y + cellSize * (float) _selectedY});
        rect.setFillColor(clrSelection);
        _window.draw(rect);
    }
}

void Graphics::drawField() {
    sf::RectangleShape rect;
    rect.setFillColor(sf::Color::Transparent);
    rect.setOutlineColor(sf::Color::Blue);
    rect.setOutlineThickness(LINE_WIDTH);

    // up reserve
    rect.setSize({cellSize * 9, cellSize * 2});
    rect.setPosition({BORDER + xPadding, BORDER});
    _window.draw(rect);

    // field
    for (int i = 0; i <= 9; ++i) {
        drawLine({startPoint.x + cellSize * (float) i, startPoint.y},
                 {startPoint.x + cellSize * (float) i, startPoint.y + 9 * cellSize},
                 LINE_WIDTH, sf::Color::White); // vertical line
        drawLine({startPoint.x, startPoint.y + cellSize * (float) i},
                 {startPoint.x + 9 * cellSize, startPoint.y + cellSize * (float) i},
                 LINE_WIDTH, sf::Color::White); // horizontal line
    }

    // down reserve
    rect.setPosition({BORDER + xPadding, (float) size.y - BORDER - 2 * cellSize});
    _window.draw(rect);
}

void Graphics::drawFigures() {
    for (int x = 0; x < 9; ++x) {
        for (int y = 0; y < 9; ++y) {
            int figure = _board->GetCell(y, x);
            if (std::abs(figure) > 0 && std::abs(figure) < 9) {
                sf::Texture texture("img/figures/" + figures[figure], false);
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

void Graphics::drawCurrentMove() {
    if (_board->getCurrentMove()) {
        drawTriangle({endPoint.x + cellSize / 2, endPoint.y - cellSize / 2},
                     {endPoint.x + cellSize / 6, endPoint.y - cellSize / 6},
                     {endPoint.x + cellSize * 5 / 6, endPoint.y - cellSize / 6},
                     sf::Color::Black);
    } else {
        drawTriangle({endPoint.x + cellSize / 6, startPoint.y + cellSize / 6},
                     {endPoint.x + cellSize * 5 / 6, startPoint.y + cellSize / 6},
                     {endPoint.x + cellSize / 2, startPoint.y + cellSize / 2},
                     sf::Color::White);
    }
}

void Graphics::updateDrawingValues() {
    size = _window.getSize();
    cellSize = std::min((size.x - 2 * BORDER - 2 * BORDER) / 9,
                        (size.y - 2 * BORDER - 2 * BORDER - 2 * RESERVE_BORDER) / 13);
    xPadding = (size.x - 2 * BORDER - cellSize * 9) / 2;
    startPoint = {BORDER + xPadding, static_cast<float>((size.y / 2) - (4.5 * cellSize))};
    endPoint = {startPoint.x + 9 * cellSize, startPoint.y + 9 * cellSize};
}