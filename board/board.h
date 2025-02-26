#ifndef BOARD_H
#define BOARD_H

class Board {
public:
    Board(); // Конструктор доски
    ~Board();// Деструктор доски

    void SetMove(int oldInd1, int oldInd2, int newInd1, int newInd2);// Выполнить ход
    void capturedAdd(int figure);// добавить фигуру в список
    void capturedRemove(int ind1, int ind2, int num);// убрать фигуру из списка
    int GetCell(int ind1, int ind2) const;// Получить информацию о клетке поля
    void CheckCheck();// проверка наличия шаха
    void FindKing(int &ind1, int &ind2); // поиск короля

private:
    int boardMas_[9][9]; // Массив самой доски
    int capturedBlack_[19];// Массив захваченных черных фигур
    int capturedWhite_[19];// Массив захваченных белых фигур
    bool currentMove_; // Чей текущий ход?
    bool checkBlack_; // Находится ли король черных под шахом?
    bool checkWhite_;// Находится ли король белых под шахом?
};

#endif //BOARD_H
