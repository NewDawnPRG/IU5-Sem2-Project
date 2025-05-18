#ifndef PIECE_H
#define PIECE_H

#include <iostream>
#include "../board/board.h"

class Piece {
public:
	piece();
	~piece();

    bool pawn(int sp1, int sp2, int fp1, int fp2, bool move);
	bool bishop(int sp1, int sp2, int fp1, int fp2, bool move);
	bool rook(int sp1, int sp2, int fp1, int fp2, bool move);
	bool spear(int sp1, int sp2, int fp1, int fp2, bool move);
	bool knight(int sp1, int sp2, int fp1, int fp2, bool move);
	bool silver(int sp1, int sp2, int fp1, int fp2, bool move);
	bool gold(int sp1, int sp2, int fp1, int fp2, bool move);
	bool king(int sp1, int sp2, int fp1, int fp2, bool move);
private:
    // Code
};

#endif //PIECE_H
