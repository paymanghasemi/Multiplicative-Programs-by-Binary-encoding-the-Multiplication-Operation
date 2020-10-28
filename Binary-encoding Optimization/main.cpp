/* 
 * File:   main.cpp
 * Author: c3156840
 */

#include <iomanip>

#include "Global_variables.h"
#include "Functions.h"

/* Inputs are  file-name  N_Objectives  Optimization-Sense  Solution-approach  Bitwise-Cut  Enhancement  Precision
 *  Optimization-Sense -> 0 := minimize ; 1:= maximize
 *  Solution-approach -> 1 := Bitwise ; 2 := BRF
 *  Bitwise Cut -> 0 := No cut ; 1 := P-cut ; 2 := F-cut
 *  Enhancement -> 0:= No trick ; 1 := Min-Min ; 2 := Indirect Min-Min
 *  
 * 
 * 
 */



int main(int argc, char *argv[]) {
    Time=3600;
    File_name=argv[1];
    N_Objectives = atoi(argv[2]);
    int Optimization_sense = atoi(argv[3]);
    int Solution_approach = atoi(argv[4]);
    int Single_bit_Cut = 0;
    long double Trick = 0;
    if (Solution_approach == 1) {
        Single_bit_Cut = atoi(argv[5]);
    }
    if (Optimization_sense == 0) {
        Trick = atoi(argv[6]);
    }
    precision=atof(argv[7]);
    Reading_LP_File_and_Generating_Gurobi_Variables(argv[1]);
    

    long double* Y_Upper_Bound = Upper_bound_finder(Trick, Optimization_sense);
    Binary_reformulation(Y_Upper_Bound, Optimization_sense);
    if(Trick!=0){
        Trick= Y_Upper_Bound[N_Objectives];
    }
    
    bool* U_Star = Optimize(Optimization_sense, Solution_approach, Single_bit_Cut, Trick);
        
    
    Writing_Output(argv[1],U_Star,Solution_approach,Optimization_sense);
    return 0;
}

