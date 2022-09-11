#include <iostream>
#include "Graph.h"

namespace GUI{
    void printBoard(Board& board){
        std::cout << "  12345678\n";
        for(int i = 7; i>=0; i--){
            std::cout << (7-i+1) << " ";
            for(int j = 0; j<8; j++){
                char c = sprites.at(board.get(j, i).type);
                if(board.get(j,i).team == Color::Black){
                    c = tolower(c);
                }
                std::cout << c;
            }
            std::cout << "\n";
        }
    }  

    void movePrompt(Color c){
        std::cout<< ((c==Color::White) ? "White" : "Black") << " make your move:\n";
    }

    std::pair<int, int> posPrompt(){
        std::cout << "Choose a coordinate to move:\n";
        int x, y;
        int c, i=0;
        int count = 0;
        char str[100];
        do{
            c = getchar();
            str[i] = c;
            i++;
            if (c != ' ' && c != '\n') count++;
        } while(c!='\n');

        if(count != 2) return std::make_pair(-1,-1);
        if(str[0] < '0' || str[0] > '9' || str[2] < '0' || str[2] > '9') return std::make_pair(-1, -1);

        x = str[0] - '0';
        y = str[2] - '0';

        return std::make_pair(x - 1, 8 - y);
    }

    std::pair<int, int> desPrompt(){
        std::cout << "Choose a coordinate to move to:\n";
        int x, y;
        int c, i=0;
        int count = 0;
        char str[100];
        do{
            c = getchar();
            str[i] = c;
            i++;
            if (c != ' ' && c != '\n') count++;
        } while(c!='\n');

        if(count == 1 && str[0] == 'b') return std::make_pair(-2, -2);
        if(count != 2) return std::make_pair(-1,-1);
        if(str[0] < '0' || str[0] > '9' || str[2] < '0' || str[2] > '9') return std::make_pair(-1, -1);

        x = str[0] - '0';
        y = str[2] - '0';
        return std::make_pair(x - 1, 8 - y);
    }

    void kingInCheck(){
        std::cout<<"King is in check!\n";
    }
    
}