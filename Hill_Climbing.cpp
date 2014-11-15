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
        successor.push_back(sudoko_board[i]);
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

void Board::printSuccessorBoard()
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            cout << successor[i * 4 + j] << " ";
        }
        cout << endl;
    }
}

/* Generate all possible successors */
void Board::generateSuccessor()
{
    for (int i = 0; i < state_vector.size(); i++)
    {
        for (int j = 0; j < 4; j++)
        {
            successor[state_vector[i]] = j + 1;
            this->evaluate();
        }
    }
}

void Board::checkHorizontalConflicts()
{
    for (int i = 0; i < 4; i++)
    {
        int row_conflicts = 0;
        for (int j = 0; j < 4; j++)
        {
            for (int k = 0; k < 4; k++)
            {
                
                row_conflicts += (successor[i * 4 + j] == successor[i * 4 + k] && (i * 4 + j) != (i * 4 + k)) ? 1 : 0;
            }
        }
        number_conflicts += row_conflicts;
    }
}


void Board::evaluate()
{
    this->checkHorizontalConflicts();
    cout << "Number of conflicts " << number_conflicts << endl;
    this->printSuccessorBoard();
    cout << endl;
    number_conflicts = 0;
}
