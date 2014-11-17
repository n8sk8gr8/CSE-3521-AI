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
#include "Genetic_Algorithm.h"

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
    if (argc != 2)
    {
        cout << "Usage: " << argv[0] << " <Sudoku_board> " << endl;
        exit(2);
    }
    
	string filename(argv[1]);

    Population* population = new Population(filename);
    population->setupPopulation();
    population->geneticAlgorithm();
    
    cout << "Sudoku Puzzle Solved " << endl;
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            cout << population->solution_board[i * 4 + j] << " ";
        }
        cout << endl;
    }

	return 0;
}
