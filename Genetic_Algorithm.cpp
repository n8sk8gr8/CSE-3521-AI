//============================================================================
// Name        : Hill_Climbing.cpp
// Author      : Nate Rabold
// Description : 
//============================================================================

#include <iostream>
#include <fstream>
#include <time.h>
#include <unistd.h> // sleep
#include <cstdlib> // rand
#include "Genetic_Algorithm.h"
#include <math.h> // round

using namespace std;

double Board::population_fitness = 0.0;

Population::Population(std::string filename)
{
    pop.resize(50);
    cout << pop.size() << endl;
    file_name = filename;
}

void Population::setupPopulation()
{
    for (int i = 0; i < pop.size(); i++)
    {
        pop[i] = new Board();
        pop[i]->readFile(file_name);
        pop[i]->createStateVector();
    }
}

void Board::initalizeSudokuBoard()
{
    //sudoko_board.clear();
    for (int i = 0; i < initial_state.size(); i++)
    {
        sudoko_board.push_back(stoi(initial_state[i]));
    }
}

int randomInt()
{
    return ((rand() % 4) + 1);
}


void Board::setupInitialPopulation()
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
    evaluate();

    /* Check if the Sudoku puzzle is solved */
    if (fitness_value == 144)
    {
        cout << "SUDOKU PUZZLE SOLVED!!!!!!!" << endl;
        solved = true;
    }
}
void Population::crossoverProbability()
{
    for (int i = 0; i < pop.size(); i++)
    {
        double crossover_factor = 1000;
        pop[i]->crossover_prob = pop[i]->fitness_value / Board::population_fitness;
        
        cout << "Individual " << i << "  " << pop[i]->crossover_prob * crossover_factor << endl;
        
        for (int j = 0; j < round(pop[i]->crossover_prob * crossover_factor); j++)
        {
            crossover_population.push_back(i);
        }
    }
}


void Population::selectCrossoverIndividuals()
{
    for (int i = 0; i < pop.size(); i++)
    {
        int individual = rand() % crossover_population.size();
        pop[i] = pop[crossover_population[individual]];
        
        cout << "Individual " << i << " is  now " << crossover_population[individual] << endl;
        pop[i]->printSudokuBoard();
    }
}


void Population::crossover()
{
    for (int i = 0; i < pop.size() / 2; i++)
    {
        Board* individual1 = pop[i];
        Board* individual2 = pop[i + 1];
        
        int head_or_tails = rand() % 2;
        int split = rand() % individual1->sudoko_board.size();
        
        /* Cross head of individual1 and tail of individual2 */
        if (head_or_tails == 0)
        {
            for (int j = 0; j < split; j++)
            {
                pop[i]->successor[j] = individual1->sudoko_board[j];
                pop[i + 1]->successor[j] = individual2->sudoko_board[j];
            }
            
            for (int j = split; j < individual1->sudoko_board.size(); j++)
            {
                pop[i]->successor[j] = individual2->sudoko_board[j];
                pop[i + 1]->successor[j] = individual1->sudoko_board[j];
            }
        }
        /* Cross head of individual2 and tail of individual1 */
        else
        {
            for (int j = 0; j < split; j++)
            {
                pop[i]->successor[j] = individual2->sudoko_board[j];
                pop[i + 1]->successor[j] = individual1->sudoko_board[j];
            }
            
            for (int j = split; j < individual1->sudoko_board.size(); j++)
            {
                pop[i]->successor[j] = individual1->sudoko_board[j];
                pop[i + 1]->successor[j] = individual2->sudoko_board[j];
            }
        }
    }
}


void Population::mutation()
{
    for (int i = 0; i < pop.size(); i++)
    {
        int mutate = rand() % 10;
        int mutate_position = rand() % pop[i]->successor.size();
        
        if (mutate == 0)
        {
            pop[i]->successor[mutate_position] = randomInt();
        }
    }
}


void Population::setupNewPopulationToOldPopulation()
{
    for (int i = 0; i < pop.size(); i++)
    {
        for (int j = 0; j < pop[i]->sudoko_board.size(); j++)
        {
            pop[i]->sudoko_board[j] = pop[i]->successor[j];
        }
        
    }

}

void Population::geneticAlgorithm()
{
    for (int i = 0; i < pop.size(); i++)
    {
        
        //number_conflicts_best_successor = 144;
        pop[i]->initalizeSudokuBoard();
        pop[i]->setupInitialPopulation();
        cout << "Initalized individual " << i << endl;
    }
    
    for (int i = 0; i < 30; i++)
    {

        cout << "POP SIZE " << pop.size() << endl;
        for (int j = 0; j < pop.size(); j++)
        {
            pop[j]->printSudokuBoard();
            pop[j]->hillClimber();
        }
        crossoverProbability();
        cout << "Population fitness " << Board::population_fitness << endl;
        selectCrossoverIndividuals();
        cout << "\nCrossover occurs " << endl;
        crossover();
        mutation();
        setupNewPopulationToOldPopulation();
        Board::population_fitness = 0.0;
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
    
    fitness_value = MAXCONFLICTS - number_conflicts;
    population_fitness += fitness_value;
    cout << "Fitness Value = " << fitness_value << endl;
    
    cout << endl;
    number_conflicts = 0;
}
