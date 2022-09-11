#include "Board.h"

Square::Square(Color _team, Piece _type){
    team = _team;
    type = _type;
}

Square::Square(){
    team = Color::None;
    type = Piece::Empty;
}

void Board::set(int x, int y, Color _team, Piece _type){
    grid[x][y] = Square(_team, _type);
}

Square Board::get(int x, int y){
    return grid[x][y];
}

Board::Board(){
    for(int i = 2; i<6; i++){
        for(int j = 0; j<8; j++){
            grid[i][j] = Square();
        }
    }

    grid[0][0] = Square(Color::White, Piece::Rook);
    grid[1][0] = Square(Color::White, Piece::Knight);
    grid[2][0] = Square(Color::White, Piece::Bishop);
    grid[3][0] = Square(Color::White, Piece::Queen);
    grid[4][0] = Square(Color::White, Piece::King);
    grid[5][0] = Square(Color::White, Piece::Bishop);
    grid[6][0] = Square(Color::White, Piece::Knight);
    grid[7][0] = Square(Color::White, Piece::Rook);

    grid[0][1] = Square(Color::White, Piece::Pawn);
    grid[1][1] = Square(Color::White, Piece::Pawn);
    grid[2][1] = Square(Color::White, Piece::Pawn);
    grid[3][1] = Square(Color::White, Piece::Pawn);
    grid[4][1] = Square(Color::White, Piece::Pawn);
    grid[5][1] = Square(Color::White, Piece::Pawn);
    grid[6][1] = Square(Color::White, Piece::Pawn);
    grid[7][1] = Square(Color::White, Piece::Pawn);

    grid[0][7] = Square(Color::Black, Piece::Rook);
    grid[1][7] = Square(Color::Black, Piece::Knight);
    grid[2][7] = Square(Color::Black, Piece::Bishop);
    grid[3][7] = Square(Color::Black, Piece::Queen);
    grid[4][7] = Square(Color::Black, Piece::King);
    grid[5][7] = Square(Color::Black, Piece::Bishop);
    grid[6][7] = Square(Color::Black, Piece::Knight);
    grid[7][7] = Square(Color::Black, Piece::Rook);
    
    grid[0][6] = Square(Color::Black, Piece::Pawn);
    grid[1][6] = Square(Color::Black, Piece::Pawn);
    grid[2][6] = Square(Color::Black, Piece::Pawn);
    grid[3][6] = Square(Color::Black, Piece::Pawn);
    grid[4][6] = Square(Color::Black, Piece::Pawn);
    grid[5][6] = Square(Color::Black, Piece::Pawn);
    grid[6][6] = Square(Color::Black, Piece::Pawn);
    grid[7][6] = Square(Color::Black, Piece::Pawn);
}