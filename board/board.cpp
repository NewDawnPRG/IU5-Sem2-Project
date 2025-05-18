#include "board.h"

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
        capturedWhite_[i] = 0;
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
    int ind1;
    int ind2;
    FindKing(ind1, ind2);
    if (currentMove_) {
        // если ход черных
        // Проверка соседних клеток
        // Вправо (j+1)
        if (ind2 + 1 < 9) {
            int cell = boardMas_[ind1][ind2 + 1];
            if (cell == 1 || cell == 3 || cell == 4 || cell == 6 || cell == 7) {
                checkWhite_ = true;
                return;
            }
        }
        // Влево (j-1)
        if (ind2 - 1 >= 0) {
            int cell = boardMas_[ind1][ind2 - 1];
            if (cell == 3 || cell == 4 || cell == 6 || cell == 7) {
                checkWhite_ = true;
                return;
            }
        }
        // Вверх (i-1, j)
        if (ind1 - 1 >= 0) {
            int cell = boardMas_[ind1 - 1][ind2];
            if (cell == 1 || cell == 3 || cell == 4 || cell == 6 || cell == 7) {
                checkWhite_ = true;
                return;
            }
        }
        // Вниз (i+1, j)
        if (ind1 + 1 < 9) {
            int cell = boardMas_[ind1 + 1][ind2];
            if (cell == 3 || cell == 4 || cell == 7) {
                checkWhite_ = true;
                return;
            }
        }
        // Верх-лево (i-1, j-1)
        if (ind1 - 1 >= 0 && ind2 - 1 >= 0) {
            int cell = boardMas_[ind1 - 1][ind2 - 1];
            if (cell == 2 || cell == 6 || cell == 7) {
                checkWhite_ = true;
                return;
            }
        }
        // Верх-право (i-1, j+1)
        if (ind1 - 1 >= 0 && ind2 + 1 < 9) {
            int cell = boardMas_[ind1 - 1][ind2 + 1];
            if (cell == 2 || cell == 6 || cell == 7) {
                checkWhite_ = true;
                return;
            }
        }
        // Низ-лево (i+1, j-1)
        if (ind1 + 1 < 9 && ind2 - 1 >= 0) {
            int cell = boardMas_[ind1 + 1][ind2 - 1];
            if (cell == 2 || cell == 6) {
                checkWhite_ = true;
                return;
            }
        }
        // Низ-право (i+1, j+1)
        if (ind1 + 1 < 9 && ind2 + 1 < 9) {
            int cell = boardMas_[ind1 + 1][ind2 + 1];
            if (cell == 2 || cell == 6) {
                checkWhite_ = true;
                return;
            }
        }
        // Проверка коня
        if (ind1 - 2 >= 0) {
            if (ind2 - 1 >= 0 && boardMas_[ind1 - 2][ind2 - 1] == 5) {
                checkWhite_ = true;
                return;
            }
            if (ind2 + 1 < 9 && boardMas_[ind1 - 2][ind2 + 1] == 5) {
                checkWhite_ = true;
                return;
            }
        }
        // Проверка ладьи и колесницы по горизонтали и вертикали
        // Влево
        for (int k = ind2 - 1; k >= 0; --k) {
            int cell = boardMas_[ind1][k];
            if (cell == 3 || cell == 4) {
                checkWhite_ = true;
                break;
            }
            if (cell != 0) break;
        }
        if (checkWhite_) return;
        // Вправо
        for (int k = ind2 + 1; k < 9; ++k) {
            int cell = boardMas_[ind1][k];
            if (cell == 3 || cell == 4) {
                checkWhite_ = true;
                break;
            }
            if (cell != 0) break;
        }
        if (checkWhite_) return;
        // Вверх
        for (int k = ind1 - 1; k >= 0; --k) {
            int cell = boardMas_[k][ind2];
            if (cell == 3 || cell == 4) {
                checkWhite_ = true;
                break;
            }
            if (cell != 0) break;
        }
        if (checkWhite_) return;
        // Вниз
        for (int k = ind1 + 1; k < 9; ++k) {
            int cell = boardMas_[k][ind2];
            if (cell == 3 || cell == 4) {
                checkWhite_ = true;
                break;
            }
            if (cell != 0) break;
        }
        if (checkWhite_) return;
        // Проверка слона по диагоналям
        // Вверх-лево
        for (int i = ind1 - 1, j = ind2 - 1; i >= 0 && j >= 0; --i, --j) {
            int cell = boardMas_[i][j];
            if (cell == 2) {
                checkWhite_ = true;
                break;
            }
            if (cell != 0) break;
        }
        if (checkWhite_) return;
        // Вверх-право
        for (int i = ind1 - 1, j = ind2 + 1; i >= 0 && j < 9; --i, ++j) {
            int cell = boardMas_[i][j];
            if (cell == 2) {
                checkWhite_ = true;
                break;
            }
            if (cell != 0) break;
        }
        if (checkWhite_) return;
        // Вниз-лево
        for (int i = ind1 + 1, j = ind2 - 1; i < 9 && j >= 0; ++i, --j) {
            int cell = boardMas_[i][j];
            if (cell == 2) {
                checkWhite_ = true;
                break;
            }
            if (cell != 0) break;
        }
        if (checkWhite_) return;
        // Вниз-право
        for (int i = ind1 + 1, j = ind2 + 1; i < 9 && j < 9; ++i, ++j) {
            int cell = boardMas_[i][j];
            if (cell == 2) {
                checkWhite_ = true;
                break;
            }
            if (cell != 0) break;
        }
    } else {
        // Проверка соседних клеток
        // Влево (j-1)
        if (ind2 - 1 >= 0) {
            int cell = boardMas_[ind1][ind2 - 1];
            if (cell == -1 || cell == -3 || cell == -4 || cell == -6 || cell == -7) {
                checkBlack_ = true;
                return;
            }
        }
        // Вправо (j+1)
        if (ind2 + 1 < 9) {
            int cell = boardMas_[ind1][ind2 + 1];
            if (cell == -3 || cell == -4 || cell == -6 || cell == -7) {
                checkBlack_ = true;
                return;
            }
        }
        // Вниз (i+1, j)
        if (ind1 + 1 < 9) {
            int cell = boardMas_[ind1 + 1][ind2];
            if (cell == -1 || cell == -3 || cell == -4 || cell == -6 || cell == -7) {
                checkBlack_ = true;
                return;
            }
        }
        // Вверх (i-1, j)
        if (ind1 - 1 >= 0) {
            int cell = boardMas_[ind1 - 1][ind2];
            if (cell == -3 || cell == -4 || cell == -7) {
                checkBlack_ = true;
                return;
            }
        }
        // Низ-право (i+1, j+1)
        if (ind1 + 1 < 9 && ind2 + 1 < 9) {
            int cell = boardMas_[ind1 + 1][ind2 + 1];
            if (cell == -2 || cell == -6 || cell == -7) {
                checkBlack_ = true;
                return;
            }
        }
        // Низ-лево (i+1, j-1)
        if (ind1 + 1 < 9 && ind2 - 1 >= 0) {
            int cell = boardMas_[ind1 + 1][ind2 - 1];
            if (cell == -2 || cell == -6 || cell == -7) {
                checkBlack_ = true;
                return;
            }
        }
        // Верх-право (i-1, j+1)
        if (ind1 - 1 >= 0 && ind2 + 1 < 9) {
            int cell = boardMas_[ind1 - 1][ind2 + 1];
            if (cell == -2 || cell == -6) {
                checkBlack_ = true;
                return;
            }
        }
        // Верх-лево (i-1, j-1)
        if (ind1 - 1 >= 0 && ind2 - 1 >= 0) {
            int cell = boardMas_[ind1 - 1][ind2 - 1];
            if (cell == -2 || cell == -6) {
                checkBlack_ = true;
                return;
            }
        }

        // Проверка коня (для белых)
        if (ind1 + 2 < 9) {
            if (ind2 - 1 >= 0 && boardMas_[ind1 + 2][ind2 - 1] == -5) {
                checkBlack_ = true;
                return;
            }
            if (ind2 + 1 < 9 && boardMas_[ind1 + 2][ind2 + 1] == -5) {
                checkBlack_ = true;
                return;
            }
        }

        // Проверка ладьи и колесницы
        // Влево
        for (int k = ind2 - 1; k >= 0; --k) {
            int cell = boardMas_[ind1][k];
            if (cell == -3 || cell == -4) {
                checkBlack_ = true;
                break;
            }
            if (cell != 0) break;
        }
        if (checkBlack_) return;
        // Вправо
        for (int k = ind2 + 1; k < 9; ++k) {
            int cell = boardMas_[ind1][k];
            if (cell == -3 || cell == -4) {
                checkBlack_ = true;
                break;
            }
            if (cell != 0) break;
        }
        if (checkBlack_) return;
        // Вниз
        for (int k = ind1 + 1; k < 9; ++k) {
            int cell = boardMas_[k][ind2];
            if (cell == -3 || cell == -4) {
                checkBlack_ = true;
                break;
            }
            if (cell != 0) break;
        }
        if (checkBlack_) return;
        // Вверх
        for (int k = ind1 - 1; k >= 0; --k) {
            int cell = boardMas_[k][ind2];
            if (cell == -3 || cell == -4) {
                checkBlack_ = true;
                break;
            }
            if (cell != 0) break;
        }
        if (checkBlack_) return;

        // Проверка слона
        // Вниз-право
        for (int i = ind1 + 1, j = ind2 + 1; i < 9 && j < 9; ++i, ++j) {
            int cell = boardMas_[i][j];
            if (cell == -2) {
                checkBlack_ = true;
                break;
            }
            if (cell != 0) break;
        }
        if (checkBlack_) return;
        // Вниз-лево
        for (int i = ind1 + 1, j = ind2 - 1; i < 9 && j >= 0; ++i, --j) {
            int cell = boardMas_[i][j];
            if (cell == -2) {
                checkBlack_ = true;
                break;
            }
            if (cell != 0) break;
        }
        if (checkBlack_) return;
        // Вверх-право
        for (int i = ind1 - 1, j = ind2 + 1; i >= 0 && j < 9; --i, ++j) {
            int cell = boardMas_[i][j];
            if (cell == -2) {
                checkBlack_ = true;
                break;
            }
            if (cell != 0) break;
        }
        if (checkBlack_) return;
        // Вверх-лево
        for (int i = ind1 - 1, j = ind2 - 1; i >= 0 && j >= 0; --i, --j) {
            int cell = boardMas_[i][j];
            if (cell == -2) {
                checkBlack_ = true;
                break;
            }
            if (cell != 0) break;
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
