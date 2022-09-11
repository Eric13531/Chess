#pragma once 
#include <map>
#include "Board.h"
#include <utility>

namespace GUI{
    void printBoard(Board& board);
    void movePrompt(Color c);
    std::pair<int, int> posPrompt();
    std::pair<int, int> desPrompt();

    void kingInCheck();

    const std::map<Piece, char> sprites = {
        {Piece::King, 'K'},
        {Piece::King_Moved, 'K'}, //Already castled
        {Piece::Queen, 'Q'},
        {Piece::Rook, 'R'},
        {Piece::Rook_Moved, 'R'},
        {Piece::Bishop, 'B'},
        {Piece::Knight, 'N'},
        {Piece::Pawn, 'P'}, //First turn pawn
        {Piece::Pawn_Moved, 'P'},
        {Piece::Pawn_En_Passant, '+'},
        {Piece::Empty, '#'}
    };
}