#include "graphics.h"

#include <string>
#include <cmath>
#include <iostream>

Graphics::Graphics(Board* board)
{
    std::string title = "Сёги", icon_path = "img/icon.png";
    _window = sf::RenderWindow(sf::VideoMode({960, 1280}),
                               sf::String::fromUtf8(title.begin(), title.end()));
    _window.setMinimumSize(sf::Vector2u(720, 900));
    _window.setFramerateLimit(60);

    sf::Image icon;
    if (icon.loadFromFile(icon_path))
        _window.setIcon(icon);

    _board = board;
    _piece = new Piece(board);

    updateDrawingValues();
}

void Graphics::redraw()
{
    _window.clear(clrBackground);
    drawField();
    drawSelection();
    drawFigures();
    drawCurrentMove();
    drawChecks();
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
        if (const auto* resized = event->getIf<sf::Event::Resized>())
        {
            sf::FloatRect r({0, 0},
                            {static_cast<float>(resized->size.x), static_cast<float>(resized->size.y)});
            _window.setView(sf::View(r));
            updateDrawingValues();
        }
        if (const auto* released = event->getIf<sf::Event::MouseButtonReleased>())
        {
            if (released->button == sf::Mouse::Button::Left)
            {
                onLeftClicked();
            }
        }
    }

    return result;
}

void Graphics::onLeftClicked()
{
    sf::Vector2i pos = sf::Mouse::getPosition(_window);
    if (startPoint.x <= pos.x && pos.x <= endPoint.x &&
        startPoint.y <= pos.y && pos.y <= endPoint.y)
    {
        // clicked in field
        int x = (pos.x - startPoint.x) / cellSize, y = (pos.y - startPoint.y) / cellSize;
        int cell = _board->GetCell(y, x);
        if (_selectedX == x && _selectedY == y)
        {
            _selectedX = -1, _selectedY = -1, _selected = -1;
        }
        else if (cell > 0 != _board->getCurrentMove() || cell == 0)
        {
            // empty cell
            if (_selectedX != -1 && _selectedY != -1)
                move(y, x);
            else if (_selected != -1)
            {
                _board->capturedRemove(y, x, _selected);
                _selectedX = -1, _selectedY = -1, _selected = -1;
            }
        }
        else if (cell > 0 == _board->getCurrentMove())
        {
            // selected color == move color
            _selectedX = x, _selectedY = y, _selected = -1;
        }
        else if (cell > 0 != _board->getCurrentMove())
        {
            // selected color != move color
            move(y, x);
        }
    }
    else if (BORDER + xPadding <= pos.x && pos.x <= BORDER + xPadding + cellSize * 9 &&
        BORDER <= pos.y && pos.y <= BORDER + cellSize * 2)
    {
        // clicked on up reserve
        if (_board->getCurrentMove())
            return;
        int index = (pos.x - BORDER - xPadding) / cellSize;
        if (pos.y - BORDER > cellSize)
            index += 9;

        if (_board->getCapturedBlack()[index] != 0)
            _selectedX = -1, _selectedY = -1, _selected = index;
        else
            _selectedX = -1, _selectedY = -1, _selected = -1;
    }
    else if (BORDER + xPadding <= pos.x && pos.x <= BORDER + xPadding + cellSize * 9 &&
        static_cast<float>(size.y) - BORDER - 2 * cellSize <= pos.y &
        pos.y <= static_cast<float>(size.y) - BORDER - 2 * cellSize + 9 * cellSize)
    {
        // clicked on down reserve
        if (!_board->getCurrentMove())
            return;
        int index = (pos.x - BORDER - xPadding) / cellSize;
        if (pos.y > static_cast<float>(size.y) - BORDER - cellSize)
            index += 9;

        if (_board->getCapturedWhite()[index] != 0)
            _selectedX = -1, _selectedY = -1, _selected = index;
        else
            _selectedX = -1, _selectedY = -1, _selected = -1;
    }
}

void Graphics::drawFigure(const sf::Vector2f& position, const int& figure)
{
    const sf::Texture texture("img/figures/" + figures[figure], false);
    float scale = cellSize / std::max(texture.getSize().x, texture.getSize().y);
    sf::Sprite sprite(texture);
    sprite.setPosition({
        position.x + (cellSize - texture.getSize().x * scale) / 2,
        position.y + (cellSize - texture.getSize().y * scale) / 2
    });
    sprite.setScale({scale, scale});
    _window.draw(sprite);
}

void Graphics::drawLine(sf::Vector2f begin, sf::Vector2f end, float width, sf::Color color)
{
    sf::Vector2f direction = end - begin;
    sf::Vector2f unitDirection = direction / std::sqrt(direction.x * direction.x + direction.y * direction.y);
    sf::Vector2f unitPerpendicular(-unitDirection.y, unitDirection.x);

    sf::Vector2f offset = width / 2 * unitPerpendicular;

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

void Graphics::drawTriangle(sf::Vector2f point1, sf::Vector2f point2, sf::Vector2f point3, sf::Color color)
{
    sf::ConvexShape triangle;
    triangle.setPointCount(3);

    triangle.setPoint(0, point1);
    triangle.setPoint(1, point2);
    triangle.setPoint(2, point3);

    triangle.setFillColor(color);
    _window.draw(triangle);
}

void Graphics::drawSelection()
{
    if (_selectedX != -1 && _selectedY != -1)
    {
        sf::RectangleShape rect;
        rect.setSize({cellSize, cellSize});
        rect.setPosition({
            startPoint.x + cellSize * static_cast<float>(_selectedX),
            startPoint.y + cellSize * static_cast<float>(_selectedY)
        });
        rect.setFillColor(clrSelection);
        _window.draw(rect);
    }
    else if (_selected != -1)
    {
        sf::RectangleShape rect;
        rect.setSize({cellSize, cellSize});
        if (!_board->getCurrentMove())
            rect.setPosition({
                BORDER + xPadding + cellSize * (_selected % 9),
                BORDER + cellSize * (_selected / 9)
            });
        else
            rect.setPosition({
                BORDER + xPadding + cellSize * (_selected % 9),
                static_cast<float>(size.y) - BORDER - cellSize * 2 + cellSize * (_selected / 9)
            });
        rect.setFillColor(clrSelection);
        _window.draw(rect);
    }
}

void Graphics::drawField()
{
    sf::RectangleShape rect;
    rect.setFillColor(sf::Color::Transparent);
    rect.setOutlineColor(sf::Color::Blue);
    rect.setOutlineThickness(LINE_WIDTH);

    // up reserve
    rect.setSize({cellSize * 9, cellSize * 2});
    rect.setPosition({BORDER + xPadding, BORDER});
    _window.draw(rect);

    // field
    for (int i = 0; i <= 9; ++i)
    {
        drawLine({startPoint.x + cellSize * (float)i, startPoint.y},
                 {startPoint.x + cellSize * (float)i, startPoint.y + 9 * cellSize},
                 LINE_WIDTH, sf::Color::White); // vertical line
        drawLine({startPoint.x, startPoint.y + cellSize * (float)i},
                 {startPoint.x + 9 * cellSize, startPoint.y + cellSize * (float)i},
                 LINE_WIDTH, sf::Color::White); // horizontal line
    }

    // down reserve
    rect.setPosition({BORDER + xPadding, static_cast<float>(size.y) - BORDER - 2 * cellSize});
    _window.draw(rect);
}

void Graphics::drawFigures()
{
    // drawing figures on field
    for (int x = 0; x < 9; ++x)
    {
        for (int y = 0; y < 9; ++y)
        {
            int figure = _board->GetCell(y, x);
            if (std::abs(figure) > 0 && std::abs(figure) < 9)
            {
                drawFigure({startPoint.x + cellSize * x, startPoint.y + cellSize * y},
                           figure);
            }
        }
    }

    // drawing figures on up reverse
    int num = 0;
    for (int i = 0; i < 19; ++i)
    {
        if (_board->getCapturedBlack()[i] != 0)
        {
            drawFigure({BORDER + xPadding + cellSize * (num % 9), BORDER + cellSize * (num / 9)},
                       _board->getCapturedBlack()[i]);
            ++num;
        }
    }

    // drawing figures on down reverse
    num = 0;
    for (int i = 0; i < 19; ++i)
    {
        if (_board->getCapturedWhite()[i] != 0)
        {
            drawFigure({
                           BORDER + xPadding + cellSize * (num % 9),
                           static_cast<float>(size.y) - BORDER - 2 * cellSize + cellSize * (num / 9)
                       },
                       _board->getCapturedWhite()[i]);
            ++num;
        }
    }
}

void Graphics::drawCurrentMove()
{
    if (_board->getCurrentMove())
    {
        drawTriangle({endPoint.x + cellSize / 2, endPoint.y - cellSize / 2},
                     {endPoint.x + cellSize / 6, endPoint.y - cellSize / 6},
                     {endPoint.x + cellSize * 5 / 6, endPoint.y - cellSize / 6},
                     sf::Color::Black);
    }
    else
    {
        drawTriangle({endPoint.x + cellSize / 6, startPoint.y + cellSize / 6},
                     {endPoint.x + cellSize * 5 / 6, startPoint.y + cellSize / 6},
                     {endPoint.x + cellSize / 2, startPoint.y + cellSize / 2},
                     sf::Color::White);
    }
}

void Graphics::drawCheck(float x, float y, bool good)
{
    std::string path = "img/check/";
    if (good)
        path += "good.png";
    else
        path += "warning.png";

    const sf::Texture texture(path, false);
    float scale = cellSize / static_cast<float>(std::max(texture.getSize().x, texture.getSize().y));
    sf::Sprite sprite(texture);
    sprite.setPosition({x, y});
    sprite.setScale({scale, scale});
    _window.draw(sprite);
}

void Graphics::drawChecks()
{
    drawCheck(startPoint.x - cellSize, endPoint.y - cellSize, !_board->checkBlack_);
    drawCheck(startPoint.x - cellSize, startPoint.y, !_board->checkWhite_);
}

void Graphics::updateDrawingValues()
{
    size = _window.getSize();
    cellSize = std::min((size.x - 2 * BORDER - 2 * BORDER) / 9,
                        (size.y - 2 * BORDER - 2 * BORDER - 2 * RESERVE_BORDER) / 13);
    xPadding = (size.x - 2 * BORDER - cellSize * 9) / 2;
    startPoint = {BORDER + xPadding, static_cast<float>((size.y / 2) - (4.5 * cellSize))};
    endPoint = {startPoint.x + 9 * cellSize, startPoint.y + 9 * cellSize};
}

void Graphics::move(int y, int x)
{
    if (_piece->checkMove(_selectedY, _selectedX, y, x,
                          abs(_board->GetCell(_selectedY, _selectedX))))
    {
        _board->SetMove(_selectedY, _selectedX, y, x);
        _selectedX = -1, _selectedY = -1, _selected = -1;
    }
}
