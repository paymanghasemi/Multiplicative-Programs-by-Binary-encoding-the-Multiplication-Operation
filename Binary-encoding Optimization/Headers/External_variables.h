/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   External_variables.h
 * Author: payman
 *
 * Created on April 1, 2020, 6:41 PM
 */

#ifndef EXTERNAL_VARIABLES_H
#define EXTERNAL_VARIABLES_H

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

#define Num_threads 1 
#define Negative_infinity -1000000
#define Positive_infinity 1000000
#define epsilon 1.0e-6
#define Abs_Optimal_Tol 1.0e-6
#define Rel_Optimal_Tol 1.0e-6
#define denominator 1.0e-12

using namespace std;






extern double Time;
extern int N_Variables;
extern int N_Objectives;
extern int N_Constraints;
extern double Total_time;
extern double* Bit_solution_time;
extern int Bit_location;
extern int Total_IP_Solved;
extern double Code_time;
extern int Skipped_Trick;
extern int Skipped_Cut;
extern int Skipped_Inspect;
extern long int N_bits_Objective;
extern double Time_Limit;
extern char* File_name;
extern bool Continuous;
extern long double Objective_value;
extern long double Gap_value;
extern bool Interrupted;
extern long double* Y_star;
extern double precision;
extern int Address;
extern GRBEnv env;
extern GRBModel* Model;
extern GRBLinExpr *ObjF;
extern GRBLinExpr Objective;
extern GRBLinExpr Math;
extern GRBConstr* Constraints;
extern GRBVar** Var_b;
extern GRBVar** Var_alpha;
extern GRBVar** Var_C;
extern GRBLinExpr** Expr_V;
extern GRBLinExpr** Expr_U;

#endif /* EXTERNAL_VARIABLES_H */

