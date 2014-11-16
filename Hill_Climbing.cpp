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
    sudoko_board.clear();
    for (int i = 0; i < initial_state.size(); i++)
    {
        sudoko_board.push_back(stoi(initial_state[i]));
    }
}

int randomInt()
{
    srand(time(NULL));
    //sleep(1);
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

void Board::printBoard(vector<int> &current_board)
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            cout << current_board[i * 4 + j] << " ";
        }
        cout << endl;
    }
}

void Board::printSuccessorBox(vector<int> box)
{
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            cout << box[i * 2 + j] << " ";
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
            int value = successor[state_vector[i]];
            successor[state_vector[i]] = j + 1;
            this->evaluate();
            successor[state_vector[i]] = value;
        }
    }
}

/* Counts all of the horizontal conflicts in the current successor board */
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


/* Counts all of the vertical conflicts in the current successor board */
void Board::checkVerticalConflicts()
{
    for (int i = 0; i < 4; i++)
    {
        int col_conflicts = 0;
        for (int j = 0; j < 4; j++)
        {
            for (int k = 0; k < 4; k++)
            {
                col_conflicts += (successor[j * 4 + i] == successor[k * 4 + i] && (j * 4 + i) != (k * 4 + i)) ? 1 : 0;
            }
        }
        number_conflicts += col_conflicts;
    }
}

/* Returns the number of duplicates */
int Board::duplicates(vector<int> box)
{
    int num_duplicates = 0;
    for (int i = 0; i < box.size(); i++)
    {
        for (int j = 0; j < box.size(); j++)
        {
            num_duplicates += (box[i] == box[j] && i != j) ? 1 : 0;
        }
    }
    return num_duplicates;
}

/* Counts all of the conflicts in the boxes for a given successor */
void Board::checkBoxConflicts()
{
    int box_conflicts = 0;

    box_conflicts += this->duplicates(box_top_left);
    box_conflicts += this->duplicates(box_top_right);
    box_conflicts += this->duplicates(box_bottom_right);
    box_conflicts += this->duplicates(box_bottom_left);
    
    number_conflicts += box_conflicts;
    
    box_bottom_left.clear();
    box_bottom_right.clear();
    box_top_left.clear();
    box_top_right.clear();
}


void Board::hillClimber()
{
    int number_conflicts_best_previous_successor = 145;
    while (number_conflicts_best_successor != number_conflicts_best_previous_successor && !solved)
    {
        number_conflicts_best_previous_successor = number_conflicts_best_successor;
        
        /* Find next successor */
        generateSuccessor();
        
        cout << "number_conflicts_best_successor = " << number_conflicts_best_successor << endl;
        
        cout << "number_conflicts_best_previous_successor = " << number_conflicts_best_previous_successor << endl;

        /* Check if the Sudoku puzzle is solved */
        if (number_conflicts_best_successor == 0)
        {
            cout << "SUDOKU PUZZLE SOLVED!!!!!!!" << endl;
            solved = true;
        }
        
        /* Set Best successor found to successor */
        for (int i = 0; i < best_successor.size(); i++)
        {
            successor[i] = (best_successor[i]);
        }
    }
}

void Board::randomRestart()
{
    int i = 0;
    while (!solved)
    {
        number_conflicts_best_successor = 144;
        initalizeSudokuBoard();
        setupHillClimber();
        printSudokuBoard();
        hillClimber();
        cout << "TIMEOUT COUNT " << i << endl;
        i++;
        successor.clear();
    }
}

void Board::evaluate()
{
    this->setupSudokuBoxes();
    
    /* Calculate all of the conflicts of the current successor */
    this->checkHorizontalConflicts();
    this->checkVerticalConflicts();
    this->checkBoxConflicts();
    
    cout << "Number of conflicts " << number_conflicts << endl;
    this->printBoard(successor);
    
    if (number_conflicts < number_conflicts_best_successor)
    {
        cout << "Setting new Best successor " << endl;
        this->setBestSuccessor();
    }
    
    if (number_conflicts == number_conflicts_best_successor)
    {
        if (randomInt() > 2)
        {
  
            cout << "Setting new Best successor " << endl;
            this->setBestSuccessor();
        }
    }
    
    
    
    cout << endl;
    number_conflicts = 0;
}


void Board::setBestSuccessor()
{
    best_successor.clear();
    number_conflicts_best_successor = number_conflicts;
    cout << successor.size() << endl;
    
    for (int i = 0; i < successor.size(); i++)
    {
        best_successor.push_back(successor[i]);
    }
}
