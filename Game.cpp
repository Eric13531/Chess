#include <iostream>
#include "Game.h"


Game::Game(){
    gameBoard = Board();
    turn = Color::White;
}

Board Game::getBoard(){
    return gameBoard;
}

void Game::whiteMate(){
    whiteKingMate = true;
}

void Game::blackMate(){
    blackKingMate = true;
}

bool Game::runGame(){
    while(!(whiteKingMate||blackKingMate)){
        GUI::printBoard(gameBoard);
        if(turn == Color::White){
            std::pair<int, int> pii = kingCheck(std::make_pair(whiteKingX, whiteKingY), turn);
            if(pii.first != -1){ 
                std::cout<<"White in check\n";
                if(mateCheck(std::make_pair(whiteKingX, whiteKingY), std::make_pair(pii.first, pii.second))){
                    std::cout<<"WHITE CHECKMATE\n";
                    whiteMate();
                    break;
                }
            }
        }
        else{
            std::pair<int, int> pii = kingCheck(std::make_pair(blackKingX, blackKingY), turn);
            if(pii.first != -1){
                std::cout<<"Black in check\n";
                if(mateCheck(std::make_pair(blackKingX, blackKingY), std::make_pair(pii.first, pii.second))){
                    std::cout<<"BLACK CHECKMATE\n";
                    blackMate();
                    break;
                }
            }
        }
        GUI::movePrompt(turn);
        bool moveComplete = false;
        bool exit;
        std::pair<int, int> piiPos;
        std::pair<int, int> piiDes;
        while(!moveComplete){
            exit = false;
            piiPos = GUI::posPrompt();
            while(!validPos(piiPos)){
                std::cout << "Try again\n";
                //std::cout << "a";
                piiPos = GUI::posPrompt();
            }
            piiDes = GUI::desPrompt();
            while(!validDes(piiPos, piiDes)){
                if(piiDes.first == -2 && piiDes.second == -2){
                    //std::cout<<"b";
                    exit = true;
                    break;
                }
                std::cout << "Try again\n";
                //std::cout << "c";
                piiDes = GUI::desPrompt();
            }
            //std::cout<<"d";
            if(!exit) moveComplete = true;
        }
        if(!executeMove(piiPos, piiDes)){
            GUI::kingInCheck();
            continue;
        }
        removeEnPassant();
        if(turn == Color::White) turn = Color::Black;
        else turn = Color::White;
    }

    return true;
}

bool Game::finalCheck(std::pair<int, int> pos, std::pair<int, int> des){
    Square s = gameBoard.get(pos.first, pos.second);
    Square sDes = gameBoard.get(des.first, des.second);
    gameBoard.set(des.first, des.second, s.team, s.type);
    gameBoard.set(pos.first, pos.second, Color::None, Piece::Empty);

    if(turn == Color::White){
        if(kingCheck(std::make_pair(whiteKingX, whiteKingY), turn).first != -1){
            gameBoard.set(des.first, des.second, sDes.team, sDes.type);
            gameBoard.set(pos.first, pos.second, s.team, s.type);
            return false;
        }
    }
    else{
        if(kingCheck(std::make_pair(blackKingX, blackKingY), turn).first != -1){
            gameBoard.set(des.first, des.second, sDes.team, sDes.type);
            gameBoard.set(pos.first, pos.second, s.team, s.type);
            return false;
        }
    }
    gameBoard.set(des.first, des.second, sDes.team, sDes.type);
    gameBoard.set(pos.first, pos.second, s.team, s.type);

    return true;
}

bool Game::desCheck(std::pair<int, int> pos, std::pair<int, int> des){
    return (validDes(pos, des) && finalCheck(pos, des));
}

bool Game::mateCheck(std::pair<int, int> kingPos, std::pair<int, int> atkPos){
    int x = kingPos.first;
    int y = kingPos.second;
    int n = atkPos.first;
    int m = atkPos.second;
    Piece p = gameBoard.get(n, m).type;
    
    if(desCheck(kingPos, std::make_pair(x+1, y+1))) return false;
    if(desCheck(kingPos, std::make_pair(x+1, y))) return false;
    if(desCheck(kingPos, std::make_pair(x+1, y-1))) return false;
    if(desCheck(kingPos, std::make_pair(x, y+1))) return false;
    if(desCheck(kingPos, std::make_pair(x, y-1))) return false;
    if(desCheck(kingPos, std::make_pair(x-1, y+1))) return false;
    if(desCheck(kingPos, std::make_pair(x-1, y))) return false;
    if(desCheck(kingPos, std::make_pair(x-1, y-1))) return false;

    if(p == Piece::King || p == Piece::King_Moved){
        return true;
    }

    else if(p == Piece::Pawn || p == Piece::Pawn_Moved || p == Piece::Knight){
        for(int i = 0; i<7; i++){
            for(int j = 0; j<7; j++){
                if(gameBoard.get(i, j).team == turn){
                    if(desCheck(std::make_pair(i, j), atkPos)) return false;
                }
            }
        }
    }

    else if(x == n){
        for(int i = 0; i<7; i++){
            for(int j = 0; j<7; j++){
                if(gameBoard.get(i, j).team == turn){
                    for(int k = m; k != y; k += (y-m)/std::abs(y-m)){
                        if(desCheck(std::make_pair(i, j), std::make_pair(n, k))) return false;
                    }
                }
            }
        }
    }

    else if(y == m){
        for(int i = 0; i<7; i++){
            for(int j = 0; j<7; j++){
                if(gameBoard.get(i, j).team == turn){
                    for(int k = n; k != x; k += (x-n)/std::abs(x-n)){
                        if(desCheck(std::make_pair(i, j), std::make_pair(k, m))) return false;
                    }
                }
            }
        }
    }

    else{
        for(int i = 0; i<7; i++){
            for(int j = 0; j<7; j++){
                if(gameBoard.get(i, j).team == turn){
                    int l = n;
                    for(int k = m; k != y; k += (y-m)/std::abs(y-m)){
                        if(desCheck(std::make_pair(i, j), std::make_pair(l, k))) return false;
                        l += (x-n) / std::abs(x-n);
                    }
                }
            }
        }
    }
    return true;
}

bool Game::validCoord(int x, int y){
    if(std::min(x, y)<0 || std::max(x, y) > 7) return false;
    return true;
}


bool Game::executeMove(std::pair<int, int> pos, std::pair<int, int> des){

    Square s = gameBoard.get(pos.first, pos.second);
    Square sDes = gameBoard.get(des.first, des.second);
    gameBoard.set(des.first, des.second, s.team, s.type);
    gameBoard.set(pos.first, pos.second, Color::None, Piece::Empty);
    //if king becomes in check, go back
    if(turn == Color::White){
        if(kingCheck(std::make_pair(whiteKingX, whiteKingY), turn).first != -1){
            gameBoard.set(des.first, des.second, sDes.team, sDes.type);
            gameBoard.set(pos.first, pos.second, s.team, s.type);
            return false;
        }
    }
    else{
        if(kingCheck(std::make_pair(blackKingX, blackKingY), turn).first != -1){
            gameBoard.set(des.first, des.second, sDes.team, sDes.type);
            gameBoard.set(pos.first, pos.second, s.team, s.type);
            return false;
        }
    }
    if(sDes.type == Piece::Pawn_En_Passant && sDes.team != s.team){
        if(s.type == Piece::Pawn || s.type == Piece::Pawn_Moved){
            gameBoard.set(des.first, des.second + 1, Color::None, Piece::Empty);
            gameBoard.set(des.first, des.second - 1, Color::None, Piece::Empty);
        }
    }
    //put kingcheck, if fail, return original and go back
    if(s.type == Piece::Pawn && std::abs(des.second - pos.second) == 2){

        gameBoard.set(des.first, des.second, s.team, Piece::Pawn_Moved);
        gameBoard.set(des.first, (pos.second + des.second) / 2, s.team, Piece::Pawn_En_Passant);
    }

    if(s.type == Piece::Rook){
        gameBoard.set(des.first, des.second, s.team, Piece::Rook_Moved);
    }

    if(s.type == Piece::King || s.type == Piece::King_Moved){
        if(turn == Color::White){
            whiteKingX = des.first;
            whiteKingY = des.second;
        }
        else{
            blackKingX = des.first;
            blackKingY = des.second;

        }
    }

    if(s.type == Piece::King){
        if(des.first - pos.first == 2){
            gameBoard.set(7, des.second, Color::None, Piece::Empty);
            gameBoard.set(pos.first + 1, des.second, s.team, Piece::Rook_Moved);
        }
        if(des.first - pos.first == -2){
            gameBoard.set(0, des.second, Color::None, Piece::Empty);
            gameBoard.set(pos.first - 1, des.second, s.team, Piece::Rook_Moved);
        }
        gameBoard.set(des.first, des.second, s.team, Piece::King_Moved);

    }
    return true;
}

bool Game::validPos(std::pair<int, int> pos){
    //return (gameBoard.get(pos.first - 1, 8 - pos.second).team == turn);
    return (gameBoard.get(pos.first, pos.second).team == turn);
}

bool Game::validDes(std::pair<int, int> pos, std::pair<int, int> des){
    if(gameBoard.get(pos.first, pos.second).type == Piece::Rook || gameBoard.get(pos.first, pos.second).type == Piece::Rook_Moved){
        if(des.first == pos.first && des.second == pos.second) return false;
        if(std::min(des.first, des.second) < 0 && std::max(des.first, des.second)>7) return false;
        if(std::min(std::abs(des.first - pos.first), std::abs(des.second-pos.second)) != 0) return false;
        if(des.second == pos.second){
            for(int i = pos.first + (des.first-pos.first)/(std::abs(des.first-pos.first)); i!=des.first; i += (des.first-pos.first)/(std::abs(des.first-pos.first))){
                if(gameBoard.get(i, pos.second).team != Color::None && gameBoard.get(i, pos.second).type != Piece::Pawn_En_Passant){
                    return false;
                }
            }
        }
        if(des.first == pos.first){
            for(int i = pos.second + (des.second-pos.second)/(std::abs(des.second-pos.second)); i!=des.second; i += (des.second-pos.second)/(std::abs(des.second-pos.second))){
                if(gameBoard.get(pos.first, i).team != Color::None && gameBoard.get(pos.first, i).type != Piece::Pawn_En_Passant){
                    return false;
                }
            }
        }
        if(gameBoard.get(des.first, des.second).team == turn) return false;
        return true;        
    }

    if(gameBoard.get(pos.first, pos.second).type == Piece::Bishop){
        if(des.first == pos.first && des.second == pos.second) return false;
        if(std::min(des.first, des.second) < 0 && std::max(des.first, des.second)>7) return false;
        if(std::abs(des.first - pos.first) != std::abs(des.second - pos.second)) return false;
        
        int j = pos.second + (des.second-pos.second)/(std::abs(des.second-pos.second));
        for(int i = pos.first + (des.first-pos.first)/(std::abs(des.first-pos.first)); i!=des.first; i += (des.first-pos.first)/(std::abs(des.first-pos.first))){
            if(gameBoard.get(i, j).team != Color::None && gameBoard.get(i, j).type != Piece::Pawn_En_Passant){
                return false;
            }
            j += (des.second-pos.second)/(std::abs(des.second-pos.second));
        }

        if(gameBoard.get(des.first, des.second).team == turn) return false;
        return true;        
    }

    if(gameBoard.get(pos.first, pos.second).type == Piece::Knight){
        if(des.first == pos.first && des.second == pos.second) return false;
        if(std::min(des.first, des.second) < 0 && std::max(des.first, des.second)>7) return false;
        if(!((std::abs(pos.first-des.first)==1 && std::abs(pos.second-des.second)==2) || (std::abs(pos.first-des.first)==2 && std::abs(pos.second-des.second)==1))) return false;
        

        if(gameBoard.get(des.first, des.second).team == turn) return false;
        return true;        
    }

    if(gameBoard.get(pos.first, pos.second).type == Piece::Queen){
        if(des.first == pos.first && des.second == pos.second) return false;
        if(std::min(des.first, des.second) < 0 && std::max(des.first, des.second)>7) return false;
        if(std::min(std::abs(des.first - pos.first), std::abs(des.second-pos.second)) != 0 && std::abs(des.first - pos.first) != std::abs(des.second - pos.second)) return false;
        
        
        if(des.second == pos.second){
            for(int i = pos.first + (des.first-pos.first)/(std::abs(des.first-pos.first)); i!=des.first; i += (des.first-pos.first)/(std::abs(des.first-pos.first))){
                if(gameBoard.get(i, pos.second).team != Color::None && gameBoard.get(i, pos.second).type != Piece::Pawn_En_Passant){
                    return false;
                }
            }
        }

        else if(des.first == pos.first){
            for(int i = pos.second + (des.second-pos.second)/(std::abs(des.second-pos.second)); i!=des.second; i += (des.second-pos.second)/(std::abs(des.second-pos.second))){
                if(gameBoard.get(pos.first, i).team != Color::None && gameBoard.get(pos.first, i).type != Piece::Pawn_En_Passant){
                    return false;
                }
            }
        }
        
        else{
            int j = pos.second + (des.second-pos.second)/(std::abs(des.second-pos.second));
            for(int i = pos.first + (des.first-pos.first)/(std::abs(des.first-pos.first)); i!=des.first; i += (des.first-pos.first)/(std::abs(des.first-pos.first))){
                if(gameBoard.get(i, j).team != Color::None && gameBoard.get(i, j).type != Piece::Pawn_En_Passant){
                    return false;
                }
                j += (des.second-pos.second)/(std::abs(des.second-pos.second));
            }
        }

        if(gameBoard.get(des.first, des.second).team == turn) return false;
        return true;        
    }

    if(gameBoard.get(pos.first, pos.second).type == Piece::Pawn_Moved || gameBoard.get(pos.first, pos.second).type == Piece::Pawn){
        //Promoting
        int yChange = 1;
        if(gameBoard.get(pos.first, pos.second).team == Color::Black){
            yChange = -1;
        }

        if(gameBoard.get(des.first, des.second).team == turn) return false;

        if(gameBoard.get(pos.first, pos.second).type == Piece::Pawn){
            if(des.second == pos.second + 2*yChange && des.first == pos.first){
                if(gameBoard.get(pos.first, pos.second + yChange).team == Color::None && gameBoard.get(pos.first, des.second).team == Color::None){
                    return true;
                }
                else return false;
            }
        }   

        if(des.second == pos.second + yChange && des.first == pos.first){
            if(gameBoard.get(pos.first, pos.second+yChange).team == Color::None) return true;
        }

        if(des.second == pos.second + yChange && std::abs(des.first - pos.first) == 1){
            if(gameBoard.get(des.first, des.second).team != Color::None && gameBoard.get(des.first, des.second).team != turn) return true;
        }


        return false;        
    }

    if(gameBoard.get(pos.first, pos.second).type == Piece::King){
        if(des.first - pos.first == 2 && gameBoard.get(pos.first + 1, pos.second).type == Piece::Empty && gameBoard.get(pos.first + 2, pos.second).type == Piece::Empty){
            if(gameBoard.get(7, pos.second).type == Piece::Rook){
                if(!(kingCheck(std::make_pair(pos.first, pos.second), gameBoard.get(pos.first, pos.second).team).first != -1)
                && !removeKingCheck(std::make_pair(pos.first + 1, pos.second), std::make_pair(pos.first, pos.second), gameBoard.get(pos.first, pos.second).team, Piece::King)
                && !removeKingCheck(std::make_pair(pos.first + 2, pos.second), std::make_pair(pos.first, pos.second), gameBoard.get(pos.first, pos.second).team, Piece::King)){
                    return true;
                }
            }
        }

        if(des.first - pos.first == -2 && gameBoard.get(pos.first - 1, pos.second).type == Piece::Empty && gameBoard.get(pos.first - 2, pos.second).type == Piece::Empty){
            if(gameBoard.get(0, pos.second).type == Piece::Rook){
                if(!(kingCheck(std::make_pair(pos.first, pos.second), gameBoard.get(pos.first, pos.second).team).first != -1)
                && !removeKingCheck(std::make_pair(pos.first - 1, pos.second), std::make_pair(pos.first, pos.second), gameBoard.get(pos.first, pos.second).team, Piece::King)
                && !removeKingCheck(std::make_pair(pos.first - 2, pos.second), std::make_pair(pos.first, pos.second), gameBoard.get(pos.first, pos.second).team, Piece::King)){
                    return true;
                }
            }
        }
    }

    if(gameBoard.get(pos.first, pos.second).type == Piece::King||gameBoard.get(pos.first, pos.second).type == Piece::King_Moved){
        if(des.first == pos.first && des.second == pos.second) return false;
        if(std::min(des.first, des.second) < 0 && std::max(des.first, des.second)>7) return false;
        if(gameBoard.get(des.first, des.second).team == turn) return false;
        if(std::abs(des.first - pos.first) <= 1 && std::abs(des.second - pos.second) <= 1)return true;
        return false;
    }


    
    //king check if 1-2 no knight, adjacent no pawn/king, grid no rook/queen, dia no bishop/queen
    //also king check opposing team after every move
    
    return false;
}

void Game::removeEnPassant(){
    if(turn == Color::White){
        for(int i = 0; i<8; i++){
            if(gameBoard.get(i, 5).type == Piece::Pawn_En_Passant) gameBoard.set(i, 5, Color::None, Piece::Empty);
        }
    }
    if(turn == Color::Black){
        for(int i = 0; i<8; i++){
            if(gameBoard.get(i, 2).type == Piece::Pawn_En_Passant) gameBoard.set(i, 2, Color::None, Piece::Empty);
        }
    }
}

std::pair<int, int> Game::kingCheck(std::pair<int, int> pos, Color team){
    int x = pos.first;
    int y = pos.second;

    if(validCoord(x+1, y+2) && gameBoard.get(x+1, y+2).type == Piece::Knight && gameBoard.get(x+1, y+2).team != team) return std::make_pair(x+1, y+2);
    if(validCoord(x-1, y+2) && gameBoard.get(x-1, y+2).type == Piece::Knight && gameBoard.get(x-1, y+2).team != team) return std::make_pair(x-1, y+2);
    if(validCoord(x+1, y-2) && gameBoard.get(x+1, y-2).type == Piece::Knight && gameBoard.get(x+1, y-2).team != team) return std::make_pair(x+1, y-2);
    if(validCoord(x-1, y-2) && gameBoard.get(x-1, y-2).type == Piece::Knight && gameBoard.get(x-1, y-2).team != team) return std::make_pair(x-1, y-2);
    if(validCoord(x+2, y+1) && gameBoard.get(x+2, y+1).type == Piece::Knight && gameBoard.get(x+2, y+1).team != team) return std::make_pair(x+2, y+1);
    if(validCoord(x-2, y+1) && gameBoard.get(x-2, y+1).type == Piece::Knight && gameBoard.get(x-2, y+1).team != team) return std::make_pair(x-2, y+1);
    if(validCoord(x+2, y-1) && gameBoard.get(x+2, y-1).type == Piece::Knight && gameBoard.get(x+2, y-1).team != team) return std::make_pair(x+2, y-1);
    if(validCoord(x-2, y-1) && gameBoard.get(x-2, y-1).type == Piece::Knight && gameBoard.get(x-2, y-1).team != team) return std::make_pair(x-2, y-1);

    int yChange = 1;
    if(team == Color::Black){
        yChange = -1;
    }
    if(validCoord(x+1, y+yChange) && gameBoard.get(x+1, y+yChange).type == Piece::Pawn && gameBoard.get(x+1, y+yChange).team != team) return std::make_pair(x+1, y+yChange);
    if(validCoord(x+1, y+yChange) && gameBoard.get(x+1, y+yChange).type == Piece::Pawn_Moved && gameBoard.get(x+1, y+yChange).team != team) return std::make_pair(x+1, y+yChange);
    if(validCoord(x-1, y+yChange) && gameBoard.get(x-1, y+yChange).type == Piece::Pawn && gameBoard.get(x-1, y+yChange).team != team) return std::make_pair(x-1, y+yChange);
    if(validCoord(x-1, y+yChange) && gameBoard.get(x-1, y+yChange).type == Piece::Pawn_Moved && gameBoard.get(x-1, y+yChange).team != team) return std::make_pair(x-1, y+yChange);

    if(validCoord(x+1, y+1) && (gameBoard.get(x+1, y+1).type == Piece::King || gameBoard.get(x+1, y+1).type == Piece::King_Moved)) return std::make_pair(x+1, y+1);
    if(validCoord(x+1, y) && (gameBoard.get(x+1, y).type == Piece::King || gameBoard.get(x+1, y).type == Piece::King_Moved)) return std::make_pair(x+1, y);
    if(validCoord(x+1, y-1) && (gameBoard.get(x+1, y-1).type == Piece::King || gameBoard.get(x+1, y-1).type == Piece::King_Moved)) return std::make_pair(x+1, y-1);
    if(validCoord(x, y+1) && (gameBoard.get(x, y+1).type == Piece::King || gameBoard.get(x, y+1).type == Piece::King_Moved)) return std::make_pair(x, y+1);
    if(validCoord(x, y-1) && (gameBoard.get(x, y-1).type == Piece::King || gameBoard.get(x, y-1).type == Piece::King_Moved)) return std::make_pair(x, y-1);
    if(validCoord(x-1, y+1) && (gameBoard.get(x-1, y+1).type == Piece::King || gameBoard.get(x-1, y+1).type == Piece::King_Moved)) return std::make_pair(x-1, y+1);
    if(validCoord(x-1, y) && (gameBoard.get(x-1, y).type == Piece::King || gameBoard.get(x-1, y).type == Piece::King_Moved)) return std::make_pair(x-1, y);
    if(validCoord(x-1, y-1) && (gameBoard.get(x-1, y-1).type == Piece::King || gameBoard.get(x-1, y-1).type == Piece::King_Moved)) return std::make_pair(x-1, y-1);


    while(validCoord(++x,y)){
        if(gameBoard.get(x,y).type == Piece::Pawn_En_Passant) continue;
        if(gameBoard.get(x,y).team == team){
            break;
        }
        if(gameBoard.get(x,y).team != team && gameBoard.get(x,y).team != Color::None){
            if(gameBoard.get(x,y).type == Piece::Rook||gameBoard.get(x,y).type == Piece::Rook_Moved||gameBoard.get(x,y).type == Piece::Queen) return std::make_pair(x, y);
            break;
        }
    }
    x = pos.first;
    while(validCoord(--x,y)){
        if(gameBoard.get(x,y).type == Piece::Pawn_En_Passant) continue;
        if(gameBoard.get(x,y).team == team){
            break;
        }
        if(gameBoard.get(x,y).team != team && gameBoard.get(x,y).team != Color::None){
            if(gameBoard.get(x,y).type == Piece::Rook||gameBoard.get(x,y).type == Piece::Rook_Moved||gameBoard.get(x,y).type == Piece::Queen) return std::make_pair(x, y);
            break;
        }
    }
    x = pos.first;
    while(validCoord(x,++y)){
        if(gameBoard.get(x,y).type == Piece::Pawn_En_Passant) continue;
        if(gameBoard.get(x,y).team == team){
            break;
        }
        if(gameBoard.get(x,y).team != team && gameBoard.get(x,y).team != Color::None){
            if(gameBoard.get(x,y).type == Piece::Rook||gameBoard.get(x,y).type == Piece::Rook_Moved||gameBoard.get(x,y).type == Piece::Queen) return std::make_pair(x, y);
            break;
        }
    }
    y = pos.second;
    while(validCoord(x,--y)){
        if(gameBoard.get(x,y).type == Piece::Pawn_En_Passant) continue;
        if(gameBoard.get(x,y).team == team){
            break;
        }
        if(gameBoard.get(x,y).team != team && gameBoard.get(x,y).team != Color::None){
            if(gameBoard.get(x,y).type == Piece::Rook||gameBoard.get(x,y).type == Piece::Rook_Moved||gameBoard.get(x,y).type == Piece::Queen) return std::make_pair(x, y);
            break;
        }
    }
    y = pos.second;
    while(validCoord(++x,++y)){
        if(gameBoard.get(x,y).type == Piece::Pawn_En_Passant) continue;
        if(gameBoard.get(x,y).team == team){
            break;
        }
        if(gameBoard.get(x,y).team != team && gameBoard.get(x,y).team != Color::None){
            if(gameBoard.get(x,y).type == Piece::Bishop||gameBoard.get(x,y).type == Piece::Queen) return std::make_pair(x, y);
            break;
        }
    }
    x = pos.first;
    y = pos.second;
    while(validCoord(++x,--y)){
        if(gameBoard.get(x,y).type == Piece::Pawn_En_Passant) continue;
        if(gameBoard.get(x,y).team == team){
            break;
        }
        if(gameBoard.get(x,y).team != team && gameBoard.get(x,y).team != Color::None){
            if(gameBoard.get(x,y).type == Piece::Bishop||gameBoard.get(x,y).type == Piece::Queen) return std::make_pair(x, y);
            break;
        }
    }
    x = pos.first;
    y = pos.second;
    while(validCoord(--x,--y)){
        if(gameBoard.get(x,y).type == Piece::Pawn_En_Passant) continue;
        if(gameBoard.get(x,y).team == team){
            break;
        }
        if(gameBoard.get(x,y).team != team && gameBoard.get(x,y).team != Color::None){
            if(gameBoard.get(x,y).type == Piece::Bishop||gameBoard.get(x,y).type == Piece::Queen) return std::make_pair(x, y);
            break;
        }
    }
    x = pos.first;
    y = pos.second;
    while(validCoord(--x,++y)){
        if(gameBoard.get(x,y).type == Piece::Pawn_En_Passant) continue;
        if(gameBoard.get(x,y).team == team){
            break;
        }
        if(gameBoard.get(x,y).team != team && gameBoard.get(x,y).team != Color::None){
            if(gameBoard.get(x,y).type == Piece::Bishop||gameBoard.get(x,y).type == Piece::Queen) return std::make_pair(x, y);
            break;
        }
    }

    return std::make_pair(-1, -1);
}


bool Game::removeKingCheck(std::pair<int, int> pos, std::pair<int, int> kingPos, Color team, Piece p){
    bool temp;
    gameBoard.set(kingPos.first, kingPos.second, Color::None, Piece::Empty);
    temp = kingCheck(pos, team).first != -1;
    gameBoard.set(kingPos.first, kingPos.second, team, p);
    return temp;
}