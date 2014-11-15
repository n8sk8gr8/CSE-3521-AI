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
    int unknown_spots = 0;
    int number_conflicts = 0;

    std::vector<int> state_vector;
	std::vector<std::string> initial_state;
	std::vector<int> sudoko_board;
    std::vector<int> successor;
    
    Board(){};
    ~Board();
    
    void readFile(std::string filename);
    void createStateVector()
    {
        char character('a');
        for (int i = 0; i < initial_state.size(); i++)
        {
            if (initial_state[i].compare("*") == 0)
            {
                initial_state[i] = std::string("0");
                state_vector.push_back(i);
                unknown_spots++;
            }
        }
    }
    
    void initalizeSudokuBoard();
    void setupHillClimber();
    void printSudokuBoard();
    void printSuccessorBoard();
    void generateSuccessor();
    void evaluate();
    void checkHorizontalConflicts();


};



#endif /* HILL_CLIMBING_H_ */
