/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Global_variables.h
 * Author: payman
 *
 * Created on April 1, 2020, 6:41 PM
 */

#ifndef GLOBAL_VARIABLES_H
#define GLOBAL_VARIABLES_H

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <gurobi_c++.h>
#include <sys/time.h>
#include <ctime>
#include <math.h>   
#include <vector>
#include <complex>
#include <iomanip>



using namespace std;
double Time;
int N_Variables;
int N_Objectives;
int N_Constraints;

double Total_time(0);
double* Bit_solution_time;
int Total_IP_Solved=0;
int Skipped_Trick=0;
int Skipped_Cut=0;
int Skipped_Inspect=0;
double Code_time(0);
long int N_bits_Objective=0;
double Time_Limit;
int Bit_location;
char* File_name;
bool Interrupted=0;
bool Continuous=0;
long double Objective_value=-101;
long double Gap_value=-101;
long double* Y_star;
double precision=1;
int Address;
GRBEnv env(true);
GRBModel* Model;
GRBLinExpr *ObjF;
GRBLinExpr Objective;
GRBLinExpr Math;
GRBConstr* Constraints;
GRBVar** Var_b;
GRBVar** Var_alpha;
GRBVar** Var_C;
GRBLinExpr** Expr_V;
GRBLinExpr** Expr_U;



#endif /* GLOBAL_VARIABLES_H */

