#include "piece.h"
#include <cmath>

Piece::Piece(Board* board) : _board(board)
{
}

Piece::~Piece()
{
}

bool Piece::checkMove(int sp1, int sp2, int fp1, int fp2, int figure)
{
    if (figure == 1) // Пешка
        return pawn(sp1, sp2, fp1, fp2, _board->getCurrentMove());
    if (figure == 4) // Копьё
        return spear(sp1, sp2, fp1, fp2, _board->getCurrentMove());
    if (figure == 5) // Конь
        return knight(sp1, sp2, fp1, fp2, _board->getCurrentMove());
    if (figure == 6) // Серебрянный конь
        return silver(sp1, sp2, fp1, fp2, _board->getCurrentMove());
    if (figure == 7) // Золотой конь
        return gold(sp1, sp2, fp1, fp2, _board->getCurrentMove());
    if (figure == 2) // Слон
        return bishop(sp1, sp2, fp1, fp2, _board->getCurrentMove());
    if (figure == 3) // Ладья
        return rook(sp1, sp2, fp1, fp2, _board->getCurrentMove());
    if (figure == 8) // Король
        return king(sp1, sp2, fp1, fp2, _board->getCurrentMove());
    return false;
}

bool Piece::pawn(int sp1, int sp2, int fp1, int fp2, bool m)
{
    if (sp2 != fp2) return false;
    if (m)
    {
        // Чёрные
        if (sp1 != fp1 + 1) return false;
        int cell = _board->GetCell(fp1, fp2);
        if (cell > 0 || cell == -8) return false; // Своя фигура
    }
    else
    {
        // Белые
        if (sp1 != fp1 - 1) return false;
        int cell = _board->GetCell(fp1, fp2);
        if (cell < 0 || cell == 8) return false; // Своя фигура
    }
    return true;
}

bool Piece::bishop(int sp1, int sp2, int fp1, int fp2, bool m)
{
    int dx = abs(sp1 - fp1), dy = abs(sp2 - fp2);
    if (dx != dy || dx == 0) return false;
    int stepX = (fp1 > sp1) ? 1 : -1;
    int stepY = (fp2 > sp2) ? 1 : -1;
    for (int i = 1; i < dx; ++i)
    {
        if (_board->GetCell(sp1 + i * stepX, sp2 + i * stepY) != 0) return false;
    }
    int target = _board->GetCell(fp1, fp2);
	if (m){
		if(target == -8) return false;
	}
	else{
		if(target == 8) return false;
	}
    return (m && target <= 0) || (!m && target >= 0);
}

bool Piece::rook(int sp1, int sp2, int fp1, int fp2, bool m)
{
    if (sp1 != fp1 && sp2 != fp2) return false;
    int step, start, end;
    if (sp1 == fp1)
    {
        step = (fp2 > sp2) ? 1 : -1;
        for (int y = sp2 + step; y != fp2; y += step)
        {
            if (_board->GetCell(sp1, y) != 0) return false;
        }
    }
    else
    {
        step = (fp1 > sp1) ? 1 : -1;
        for (int x = sp1 + step; x != fp1; x += step)
        {
            if (_board->GetCell(x, sp2) != 0) return false;
        }
    }
    int target = _board->GetCell(fp1, fp2);
	if (m){
		if(target == -8) return false;
	}
	else{
		if(target == 8) return false;
	}
    return (m && target <= 0) || (!m && target >= 0);
}

bool Piece::spear(int sp1, int sp2, int fp1, int fp2, bool m)
{
    if (sp2 != fp2) return false;
    int delta = fp1 - sp1;
    if ((m && delta >= 0) || (!m && delta <= 0)) return false;
    int step = (delta > 0) ? 1 : -1;
    for (int x = sp1 + step; x != fp1; x += step)
    {
        if (_board->GetCell(x, sp2) != 0) return false;
    }
    int target = _board->GetCell(fp1, fp2);
	if (m){
		if(target == -8) return false;
	}
	else{
		if(target == 8) return false;
	}
    return (m && target <= 0) || (!m && target >= 0);
}

bool Piece::knight(int sp1, int sp2, int fp1, int fp2, bool m)
{
    int dx = abs(sp1 - fp1), dy = abs(sp2 - fp2);
    if (!((dx == 2 && dy == 1) || (dx == 1 && dy == 2))) return false;
    int target = _board->GetCell(fp1, fp2);
	if (m){
		if(target == -8) return false;
	}
	else{
		if(target == 8) return false;
	}
    return (m && target <= 0) || (!m && target >= 0);
}

bool Piece::silver(int sp1, int sp2, int fp1, int fp2, bool m)
{
    int dx = fp1 - sp1, dy = fp2 - sp2;
    if (abs(dx) > 1 || abs(dy) > 1) return false;
    bool valid = (dx == 1 && dy == 0) || (abs(dx) == 1 && abs(dy) == 1);
    if (!m) valid = (dx == -1 && dy == 0) || (abs(dx) == 1 && abs(dy) == 1);
    if (!valid) return false;
    int target = _board->GetCell(fp1, fp2);
	if (m){
		if(target == -8) return false;
	}
	else{
		if(target == 8) return false;
	}
    return (m && target <= 0) || (!m && target >= 0);
}

bool Piece::gold(int sp1, int sp2, int fp1, int fp2, bool m)
{
    int dx = fp1 - sp1, dy = fp2 - sp2;
    if (abs(dx) > 1 || abs(dy) > 1) return false;
    if ((m && dx == -1 && abs(dy) == 1) || (!m && dx == 1 && abs(dy) == 1)) return false;
    int target = _board->GetCell(fp1, fp2);
	if (m){
		if(target == -8) return false;
	}
	else{
		if(target == 8) return false;
	}
    return (m && target <= 0) || (!m && target >= 0);
}

bool Piece::king(int sp1, int sp2, int fp1, int fp2, bool m)
{
    int dx = abs(sp1 - fp1), dy = abs(sp2 - fp2);
    if (dx > 1 || dy > 1) return false;
    int target = _board->GetCell(fp1, fp2);
	if (m){
		if(target == -8) return false;
	}
	else{
		if(target == 8) return false;
	}
    return (m && target <= 0) || (!m && target >= 0);
}
