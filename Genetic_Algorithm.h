/*
 * Hill_climbing.h
 *
 *  Created on: Nov 14, 2014
 *      Author: n8
 */

#ifndef GENETIC_ALGORITHM_H_
#define GENETIC_ALGORITHM_H_
#include <vector>
#include <string>

class Board
{
public:
    int unknown_spots = 0;
    int number_conflicts_best_successor = 144; // 144 Max number of conflicts
    int number_conflicts = 0;
    bool solved = false;

    std::vector<int> state_vector;
	std::vector<std::string> initial_state;
	std::vector<int> sudoko_board;
    std::vector<int> successor;
    std::vector<int> best_successor;
    
    /* Boxes of current successor being evaluated */
    std::vector<int> box_top_left;
    std::vector<int> box_top_right;
    std::vector<int> box_bottom_left;
    std::vector<int> box_bottom_right;

    
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
    
    void setupSudokuBoxes()
    {
        box_top_left = {successor[0], successor[1], successor[4], successor[5]};
        
        box_top_right = {successor[2], successor[3], successor[6], successor[7]};
        
        box_bottom_left = {successor[8], successor[9], successor[12], successor[13]};
        
        box_bottom_right = {successor[10], successor[11], successor[14], successor[15]};
    }

    
    void initalizeSudokuBoard();
    void setupInitialPopulation();
    void printSudokuBoard();
    void printBoard(std::vector<int> &current_board);
    void printSuccessorBox(std::vector<int> box);
    void generateSuccessor();
    void evaluate();
    void checkHorizontalConflicts();
    void checkVerticalConflicts();
    void checkBoxConflicts();
    int duplicates(std::vector<int> box);
    void setBestSuccessor();
    void hillClimber();
};


class Population
{
public:
    std::string file_name;
    std::vector<Board*> pop;
    Population(std::string);

    ~Population(){};
    
    void setupPopulation();
    void geneticAlgorithm();
};





#endif /* GENETIC_ALGORITHM_H_ */
