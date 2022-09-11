#pragma once 
//#include <Piece.h>

enum class Color{
    White,
    Black,
    None
};

enum class Piece
{
    King,
    King_Moved, //Moved
    Queen,
    Rook,
    Rook_Moved,
    Bishop,
    Knight,
    Pawn, //First turn pawn
    Pawn_Moved,
    Pawn_En_Passant,
    Empty
};

struct Square{
    Color team;
    Piece type;
    Square(Color _team, Piece _type);
    Square();
};

class Board{
    private:
        Square grid[8][8];
    public:
        void set(int x, int y, Color _team, Piece _type);
        Square get(int x, int y);
        Board();


};