#include "board.h"
#include <cmath>

Board::Board() {
    const int initData[9][9] = {
        {-4, -5, -6, -7, -8, -7, -6, -5, -4},
        {0, -3, 0, 0, 0, 0, 0, -2, 0},
        {-1, -1, -1, -1, -1, -1, -1, -1, -1},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {1, 1, 1, 1, 1, 1, 1, 1, 1},
        {0, 2, 0, 0, 0, 0, 0, 3, 0},
        {4, 5, 6, 7, 8, 7, 6, 5, 4}
    }; // создаем массив с текущими фигурами на доске

    for (int i = 0; i < 9; ++i) {
        for (int j = 0; j < 9; ++j) {
            boardMas_[i][j] = initData[i][j]; // перенос фигур в нужное поле
        }
    }

    // Инициализация массивов захваченных фигур
    for (int i = 0; i < 19; ++i) {
        capturedBlack_[i] = 0;
        capturedWhite_[i] = 1;
    }
    currentMove_ = true; // начальный ход за черными (true), белые - false.
    checkBlack_ = false;
    checkWhite_ = false; // установки шахов
};

void Board::SetMove(int oldInd1, int oldInd2, int newInd1, int newInd2) {
    if (boardMas_[newInd1][newInd2] !=
        0) {
        // если в новой позиции была фигура
        capturedAdd(boardMas_[newInd1][newInd2]); // отправить фигуру в список
    }
    boardMas_[newInd1][newInd2] = boardMas_[oldInd1][oldInd2]; // установка фигуры на позицию
    boardMas_[oldInd1][oldInd2] = 0; // обнуление предыдущей позиции
    CheckCheck(); // проверить, не появился ли шах на доске
    currentMove_ = !currentMove_; // сменить ход
};

void Board::capturedAdd(int figure) {
    if (currentMove_) {
        // если ход черных
        int i = 0;
        while (capturedWhite_[i] != 0) {
            i++;
        }
        capturedWhite_[i] = figure * -1; // добавим в список захваченных белых фигур еще одну
    } else {
        // иначе
        int i = 0;
        while (capturedBlack_[i] != 0) {
            i++;
        }
        capturedBlack_[i] = figure * -1; // добавим в список захваченных черных фигур еще одну
    }
};

int Board::GetCell(int ind1, int ind2) const {
    return boardMas_[ind1][ind2];
};

void Board::capturedRemove(int ind1, int ind2, int num) {
    if (boardMas_[ind1][ind2] == 0) {
        // если в точке нет фигур
        if (currentMove_) {
            // если ход черных
            boardMas_[ind1][ind2] = capturedWhite_[num]; // выбрасываем фигуру
            capturedWhite_[num] = 0; // ставим на это место 0
        } else {
            // иначе
            boardMas_[ind1][ind2] = capturedBlack_[num]; // выбрасываем фигуру
            capturedBlack_[num] = 0; // ставим на это место 0
        }
    }
    CheckCheck(); // проверить, не появился ли шах на доске
    currentMove_ = !currentMove_; // сменить ход
}

void Board::CheckCheck() {
    checkWhite_ = false;
    checkBlack_ = false;

    // Находим позиции королей
    int whiteKingRow = -1, whiteKingCol = -1;
    int blackKingRow = -1, blackKingCol = -1;
    for (int i = 0; i < 9; ++i) {
        for (int j = 0; j < 9; ++j) {
            int cell = GetCell(i, j);
            if (cell == -8) {
                whiteKingRow = i;
                whiteKingCol = j;
            } else if (cell == 8) {
                blackKingRow = i;
                blackKingCol = j;
            }
        }
    }

    if (whiteKingRow == -1 || blackKingRow == -1) return;

    // Проверка шаха для белого короля (атакуют черные фигуры)
    for (int i = 0; i < 9; ++i) {
        for (int j = 0; j < 9; ++j) {
            int piece = GetCell(i, j);
            if (piece <= 0) continue;

            bool isAttacking = false;
            switch (piece) {
                case 1: { // Пешка (черная)
                    if (whiteKingRow == i - 1 && whiteKingCol == j)
                        isAttacking = true;
                    break;
                }
                case 2: { // Слон (черный)
                    if (abs(whiteKingRow - i) != abs(whiteKingCol - j)) break;
                    int dr = (whiteKingRow > i) ? 1 : -1;
                    int dc = (whiteKingCol > j) ? 1 : -1;
                    int r = i + dr, c = j + dc;
                    bool clear = true;
                    while (r != whiteKingRow || c != whiteKingCol) {
                        if (r < 0 || r >= 9 || c < 0 || c >= 9 || GetCell(r, c) != 0) {
                            clear = false;
                            break;
                        }
                        r += dr;
                        c += dc;
                    }
                    isAttacking = clear;
                    break;
                }
                case 3: { // Ладья (черная)
                    if (i != whiteKingRow && j != whiteKingCol) break;
                    int dr = (i == whiteKingRow) ? 0 : (whiteKingRow > i) ? 1 : -1;
                    int dc = (j == whiteKingCol) ? 0 : (whiteKingCol > j) ? 1 : -1;
                    int r = i + dr, c = j + dc;
                    bool clear = true;
                    while (r != whiteKingRow || c != whiteKingCol) {
                        if (r < 0 || r >= 9 || c < 0 || c >= 9 || GetCell(r, c) != 0) {
                            clear = false;
                            break;
                        }
                        r += dr;
                        c += dc;
                    }
                    isAttacking = clear;
                    break;
                }
                case 4: { // Копье (черное)
                    if (j != whiteKingCol || whiteKingRow > i) break;
                    bool clear = true;
                    for (int r = i - 1; r > whiteKingRow; --r) {
                        if (GetCell(r, j) != 0) {
                            clear = false;
                            break;
                        }
                    }
                    isAttacking = clear && (whiteKingRow < i);
                    break;
                }
                case 5: { // Конь (черный)
                    if (whiteKingRow == i - 2 && (whiteKingCol == j - 1 || whiteKingCol == j + 1) &&
                        i - 2 >= 0 && whiteKingCol >= 0 && whiteKingCol < 9)
                        isAttacking = true;
                    break;
                }
                case 6: { // Серебро (черное)
                    int dr = whiteKingRow - i;
                    int dc = whiteKingCol - j;
                    if ((dr == -1 && (dc == 0 || abs(dc) == 1)) || (dr == 1 && abs(dc) == 1))
                        isAttacking = true;
                    break;
                }
                case 7: { // Золото (черное)
                    int dr = whiteKingRow - i;
                    int dc = whiteKingCol - j;
                    if ((dr == -1 && (dc == 0 || abs(dc) == 1)) || (dr == 0 && abs(dc) == 1) || (dr == 1 && dc == 0))
                        isAttacking = true;
                    break;
                }
            }
            if (isAttacking) checkWhite_ = true;
        }
    }

    // Проверка шаха для черного короля (атакуют белые фигуры)
    for (int i = 0; i < 9; ++i) {
        for (int j = 0; j < 9; ++j) {
            int piece = GetCell(i, j);
            if (piece >= 0) continue;
            int absPiece = -piece;

            bool isAttacking = false;
            switch (absPiece) {
                case 1: { // Пешка (белая)
                    if (blackKingRow == i + 1 && blackKingCol == j)
                        isAttacking = true;
                    break;
                }
                case 2: { // Слон (белый)
                    if (abs(blackKingRow - i) != abs(blackKingCol - j)) break;
                    int dr = (blackKingRow > i) ? 1 : -1;
                    int dc = (blackKingCol > j) ? 1 : -1;
                    int r = i + dr, c = j + dc;
                    bool clear = true;
                    while (r != blackKingRow || c != blackKingCol) {
                        if (r < 0 || r >= 9 || c < 0 || c >= 9 || GetCell(r, c) != 0) {
                            clear = false;
                            break;
                        }
                        r += dr;
                        c += dc;
                    }
                    isAttacking = clear;
                    break;
                }
                case 3: { // Ладья (белая)
                    if (i != blackKingRow && j != blackKingCol) break;
                    int dr = (i == blackKingRow) ? 0 : (blackKingRow > i) ? 1 : -1;
                    int dc = (j == blackKingCol) ? 0 : (blackKingCol > j) ? 1 : -1;
                    int r = i + dr, c = j + dc;
                    bool clear = true;
                    while (r != blackKingRow || c != blackKingCol) {
                        if (r < 0 || r >= 9 || c < 0 || c >= 9 || GetCell(r, c) != 0) {
                            clear = false;
                            break;
                        }
                        r += dr;
                        c += dc;
                    }
                    isAttacking = clear;
                    break;
                }
                case 4: { // Копье (белое)
                    if (j != blackKingCol || blackKingRow < i) break;
                    bool clear = true;
                    for (int r = i + 1; r < blackKingRow; ++r) {
                        if (GetCell(r, j) != 0) {
                            clear = false;
                            break;
                        }
                    }
                    isAttacking = clear && (blackKingRow > i);
                    break;
                }
                case 5: { // Конь (белый)
                    if (blackKingRow == i + 2 && (blackKingCol == j - 1 || blackKingCol == j + 1) &&
                        i + 2 < 9 && blackKingCol >= 0 && blackKingCol < 9)
                        isAttacking = true;
                    break;
                }
                case 6: { // Серебро (белое)
                    int dr = blackKingRow - i;
                    int dc = blackKingCol - j;
                    if ((dr == 1 && (dc == 0 || abs(dc) == 1)) || (dr == -1 && abs(dc) == 1))
                        isAttacking = true;
                    break;
                }
                case 7: { // Золото (белое)
                    int dr = blackKingRow - i;
                    int dc = blackKingCol - j;
                    if ((dr == 1 && (dc == 0 || abs(dc) == 1)) || (dr == 0 && abs(dc) == 1) || (dr == -1 && dc == 0))
                        isAttacking = true;
                    break;
                }
            }
            if (isAttacking) checkBlack_ = true;
        }
    }
}

void Board::FindKing(int &ind1, int &ind2) {
    if (currentMove_) {
        // если ход черных
        for (int i = 0; i < 9; ++i) {
            for (int j = 0; j < 9; ++j) {
                if (boardMas_[i][j] == -8) {
                    // если король найден
                    ind1 = i;
                    ind2 = j;
                    break;
                }
            }
        }
    } else {
        for (int i = 0; i < 9; ++i) {
            for (int j = 0; j < 9; ++j) {
                if (boardMas_[i][j] == 8) {
                    // если король найден
                    ind1 = i;
                    ind2 = j;
                    break;
                }
            }
        }
    }
}

bool Board::getCurrentMove() {
    return currentMove_;
}

int *Board::getCapturedBlack() {
    return capturedBlack_;
}

int *Board::getCapturedWhite() {
    return capturedWhite_;
}
