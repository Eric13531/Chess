#pragma once 
#include "Board.h"
#include "Graph.h"

class Game{
    private:
        Board gameBoard;
        Color turn;
        bool whiteKingCheck, blackKingCheck;
        bool whiteKingMate, blackKingMate;


        int whiteKingX = 4, whiteKingY = 0;
        int blackKingX = 4, blackKingY = 7;

        bool validPos(std::pair<int, int> pos);
        bool validDes(std::pair<int, int> pos, std::pair<int, int> des);
        std::pair<int, int> kingCheck(std::pair<int, int> pos, Color team);
        bool removeKingCheck(std::pair<int, int> pos, std::pair<int, int> kingPos, Color team, Piece p); //kingPos, coord of king     p for moved or unmoved
        bool validCoord(int x, int y);

        bool executeMove(std::pair<int, int> pos, std::pair<int, int> des);

        bool mateCheck(std::pair<int, int> kingPos, std::pair<int, int> atkPos);
        bool finalCheck(std::pair<int, int> pos, std::pair<int, int> des);
        bool desCheck(std::pair<int, int> pos, std::pair<int, int> des);

        //remove enpassant squares of opposite team after turn
        void removeEnPassant();

        void whiteMate();
        void blackMate();
    public:
        Game();
        Board getBoard();
        //gameCondition runGame();
        bool runGame();
};