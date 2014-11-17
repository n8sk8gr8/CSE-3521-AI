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
std::vector<int> Population::solution_board;


Population::Population(std::string filename)
{
    pop.resize(10);
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
        solved = true;
        Population::setSolutionBoard(this);
    }
}

bool Population::checkSolved()
{
    for (int i = 0; i < pop.size(); i++)
    {
        if(pop[i]->solved)
        {
            return true;
        }
        
    }
    return false;
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
        
        for (int j = 0; j < pop[i]->sudoko_board.size(); j++)
        {
            pop[i]->sudoko_board[j] = pop[crossover_population[individual]]->successor[j];
        }
            
    }
    crossover_population.clear();
}


void Population::crossover()
{
    for (int i = 0; i < pop.size() / 2; i+=2)
    {
        /* Randomly choose mates after weighted average calculated */
        int mate1 = rand() % pop.size();
        int mate2 = rand() % pop.size();
        
        while( mate1 == mate2)
        {
            mate2 = rand() % pop.size();
        }
        
        Board* individual1 = pop[mate1];
        Board* individual2 = pop[mate2];
        
        /* Crossover the first row and set the new population */
        for (int j = 0; j < 4; j++)
        {
            if (individual1->unknown_spot(j) && individual2->unknown_spot(j))
            {
                pop[i]->successor[j] = individual1->sudoko_board[j];
                pop[i + 1]->successor[j] = individual2->sudoko_board[j];
            }
        }
        
        /* Crossover the second row and set the new population */
        for (int j = 4; j < 8; j++)
        {
            if (individual1->unknown_spot(j) && individual2->unknown_spot(j))
            {
                pop[i]->successor[j] = individual2->sudoko_board[j];
                pop[i + 1]->successor[j] = individual1->sudoko_board[j];
            }
        }
        
        /* Crossover the third row and set the new population */
        for (int j = 8; j < 12; j++)
        {
            if (individual1->unknown_spot(j) && individual2->unknown_spot(j))
            {
                pop[i]->successor[j] = individual1->sudoko_board[j];
                pop[i + 1]->successor[j] = individual2->sudoko_board[j];
            }
        }
        
        /* Crossover the fourth row and set the new population */
        for (int j = 12; j < individual1->sudoko_board.size(); j++)
        {
            if (individual1->unknown_spot(j) && individual2->unknown_spot(j))
            {
                pop[i]->successor[j] = individual2->sudoko_board[j];
                pop[i + 1]->successor[j] = individual1->sudoko_board[j];
            }
        }
    }
}


bool Board::unknown_spot(int spot)
{
    return (initial_state[spot].compare("0") == 0);
}


void Population::mutation()
{
    for (int i = 0; i < pop.size(); i++)
    {
        int mutate = rand() % 1;
        int mutate_position = rand() % pop[i]->successor.size();
        
        while(!pop[i]->unknown_spot(mutate_position))
        {
            mutate_position = rand() % pop[i]->successor.size();
        }
        
        int spot;
        if (mutate == 0 && pop[i]->unknown_spot(mutate_position))
        {
            //spot = pop[i]->successor[mutate_position];
            int ranInt = randomInt();
            while (pop[i]->successor[mutate_position] == ranInt)
            {
                ranInt = randomInt();
            }
            pop[i]->successor[mutate_position] = ranInt;//pop[i + 1]->successor[mutate_position];
            //pop[i + 1]->successor[mutate_position] = spot;

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
        pop[i]->initalizeSudokuBoard();
        pop[i]->setupInitialPopulation();
        cout << "Initalized individual " << i << endl;
    }
    int generation = 0;
    while(!checkSolved())
    {
        cout << "THIS IS the " << generation << " GENERATION" << endl;
        for (int j = 0; j < pop.size(); j++)
        {
            pop[j]->hillClimber();
        }

        crossoverProbability();
        cout << "Population fitness " << Board::population_fitness << endl;
        selectCrossoverIndividuals();
        cout << "\nCrossover occurs " << endl;
        crossover();
        
        mutation();
        
        Board::population_fitness = 0.0;
        generation++;
    }

    /* "SUDOKU PUZZLE SOLVED!! */
    return;
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

void Population::setSolutionBoard(Board* correct_sudoku)
{
    for (int i = 0; i < correct_sudoku->successor.size(); i++)
    {
        solution_board.push_back(correct_sudoku->successor[i]);
    }
}
