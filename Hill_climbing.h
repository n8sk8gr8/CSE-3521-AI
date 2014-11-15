/*
 * Hill_climbing.h
 *
 *  Created on: Nov 14, 2014
 *      Author: n8
 */

#ifndef HILL_CLIMBING_H_
#define HILL_CLIMBING_H_
#include <vector>
#include <string>

class Board
{
public:
    int empty_spots = 0;
    //std::vector<std::string> state_vector;
    std::vector<int> state_vector;
	std::vector<std::string> initial_state;
	std::vector<int> sudoko_board;
    Board(){};
    void readFile(std::string filename);
    void createStateVector()
    {
        char character('a');
        for (int i = 0; i < initial_state.size(); i++)
        {
            if (initial_state[i].compare("*") == 0)
            {
                //initial_state[i] = std::string(1, char(character + empty_spots));
                initial_state[i] = std::string("0");
                //state_vector.push_back(std::string(1, char(character + empty_spots)));
                state_vector.push_back(i);
                empty_spots++;
            }
        }
    }
    
    void initalizeSudokuBoard();
    void setupHillClimber();
    void printSudokuBoard();

};



#endif /* HILL_CLIMBING_H_ */
