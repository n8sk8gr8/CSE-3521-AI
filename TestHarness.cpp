/*
 * TestHarness.cpp
 *
 *  Created on: Nov 14, 2014
 *      Author: n8
 */
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <vector>
#include "Hill_climbing.h"

using namespace std;

/* Reads inital board and stores into a Board instance variable */
void Board::readFile(string filename)
{
    string line;
    ifstream file;
    bool first_line = true;
    file.open(filename);
    if( file.is_open())
    {
        while (getline(file, line))
        {
            if (line.compare("4") != 0)
            {
                for (int i = 0; i < line.length(); i++)
                {
                    initial_state.push_back(string(1, line[i]));
                }
                first_line = false;
            }
        }
        file.close();
    }
    else
    {
        cout << "Unable to open file." << endl;
        exit(1);
    }
}



int main(int argc, char ** argv)
{
	string filename("sudoku_data.txt");
    Board* board = new Board();
    board->readFile(filename);
    board->createStateVector();
    cout << "Number of unknown spots " << board->unknown_spots << endl;
    
    
    board->initalizeSudokuBoard();
    board->setupHillClimber();
    board->printSudokuBoard();
    cout << endl;
    board->generateSuccessor();
    
	return 0;
}
