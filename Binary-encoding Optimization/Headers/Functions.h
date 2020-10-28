/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Functions.h
 * Author: payman
 *
 * Created on April 1, 2020, 6:50 PM
 */

#ifndef FUNCTIONS_H
#define FUNCTIONS_H

void Reading_LP_File_and_Generating_Gurobi_Variables(char* Input_file_name);
long double* Upper_bound_finder(long double Trick, int Optimization_sense);
void Binary_reformulation(long double* Y_Upper_Bound, int Optimization_sense);
vector<bool*> Temp_Vector_updater(vector<bool*> Temp_U, bool Current_value, int Preferred_value, bool* &U_Star);
bool* Optimize(int Optimization_sense, int Solution_approach, int Single_bit_Cut, long double Trick);
void Writing_Output(char* Input_file_name, bool* U_Star,int Solution_approach, int Optimization_sense);
#endif /* FUNCTIONS_H */

