//============================================================================
// Name        : Hill_Climbing.cpp
// Author      : Nate Rabold
// Description : 
//============================================================================

#include <iostream>
#include <fstream>
#include <time.h>
#include <unistd.h> // sleep
#include <cstdlib> // for srand
#include "Hill_climbing.h"

using namespace std;

void Board::initalizeSudokuBoard()
{
    for (int i = 0; i < initial_state.size(); i++)
    {
        sudoko_board.push_back(stoi(initial_state[i]));
    }
}

int randomInt()
{
    srand(time(NULL));
    sleep(1);
    return ((rand() % 4) + 1);
}

void Board::setupHillClimber()
{
    for (int i = 0; i < initial_state.size(); i++)
    {
        if (sudoko_board[i] == 0)
        {
            sudoko_board[i] = randomInt();
        }
    }
}

void Board::printSudokuBoard()
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            cout << sudoko_board[i * 4 + j] << " ";
        }
        cout << endl;
    }
}
