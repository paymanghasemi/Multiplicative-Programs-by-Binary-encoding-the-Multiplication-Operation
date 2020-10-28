/* Copyright 2018, Gurobi Optimization, LLC */

/* Want to cover three different sets but subject to a common budget of
 * elements allowed to be used. However, the sets have different priorities to
 * be covered; and we tackle this by using multi-objective optimization. */

#include "gurobi_c++.h"
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sys/time.h>
#include <math.h>
#include <iomanip>
using namespace std;

#define Output_Precision 10
#define Relative_Gap 1.0e-6
#define Num_threads 1 
#define denominator 1.0e-12
#define Time_Limit 3600
#define Negative_infinity -1000000
#define Positive_infinity 1000000
#define epsilon 1.0e-6
#define Abs_Optimal_Tol 1.0e-6
#define Rel_Optimal_Tol 1.0e-6



int N_Variables;
int N_Objectives;
int N_Constraints;

GRBEnv env(true);
GRBModel* Model;
GRBLinExpr *ObjF;
GRBQuadExpr Objective;
GRBLinExpr Math;

void Read_LP_File(char* IP_file_name) {
    Model = new GRBModel(env, IP_file_name);
    Model->set(GRB_IntParam_UpdateMode, 1);
    N_Variables = Model->get(GRB_IntAttr_NumVars)-N_Objectives;
    N_Constraints = Model->get(GRB_IntAttr_NumConstrs)-2*N_Objectives;
    ObjF = new GRBLinExpr [N_Objectives];
    Objective.clear();
    for (int i = 0; i < N_Objectives; i++) {
        ObjF[i].clear();
        ObjF[i] = Model->getRow(Model->getConstr(i));
        Model->remove(Model->getConstr(i));
    }
}

int main(int argc, char *argv[]) {
    try {

        N_Objectives = atoi(argv[2]);
        int Sense= atoi(argv[3]);
        env.start();
        env.set(GRB_IntParam_Threads, 1);
        env.set(GRB_IntParam_OutputFlag, 0);
        Read_LP_File(argv[1]);



        GRBVar* Var_z = Model->addVars(N_Objectives - 1);
        Model->addQConstr(Var_z[0], GRB_EQUAL, ObjF[0] * ObjF[1]);
       int Var_z_counter = 0;
        for (int i = 2; i < N_Objectives; i++) {
           Model->addQConstr(Var_z[Var_z_counter + 1], GRB_EQUAL, Var_z[Var_z_counter] * ObjF[i]);
           Var_z_counter++;
        }
        Objective = Var_z[Var_z_counter];

        if(Sense==0){
        Model->setObjective(Objective, GRB_MINIMIZE);
        }else{
            Model->setObjective(Objective, GRB_MAXIMIZE);
        }

        Model->set(GRB_IntParam_Threads, 1);
        Model->set(GRB_DoubleParam_MIPGap, 0.0);
        Model->set(GRB_DoubleParam_MIPGapAbs, 0.0);
        Model->set(GRB_IntParam_NonConvex, 2);
        //Model->set(GRB_IntParam_NumericFocus,1);
        Model->set(GRB_DoubleParam_TimeLimit,Time_Limit);
        double Start = clock();
        Model->optimize();
        Start = (clock() - Start) / CLOCKS_PER_SEC;
        
        if (Model->get(GRB_IntAttr_Status) == GRB_OPTIMAL || Model->get(GRB_IntAttr_Status) == GRB_TIME_LIMIT) {
            ofstream Output;
            Output.open("GRB_Result.txt", ios_base::app);
            Output<<argv[1] <<" Obj_value= " << setprecision(floor(log10(Model->get(GRB_DoubleAttr_ObjVal)))+7) << Model->get(GRB_DoubleAttr_ObjVal);
            Output<<" Gap= " << setprecision(3) << Model->get(GRB_DoubleAttr_MIPGap);
    Output<<  " Sol_Time= "<<setprecision(6)<< Start<<" N_var= "<<N_Variables<< " N_const= "<<N_Constraints<<endl;
    Output.close();
        } else {
            cout << "Could not Solve " << argv[1] << " : " << Model->get(GRB_IntAttr_Status) << endl;
            exit(0);
        }


    } catch (GRBException e) {
        cout << "Error code = " << e.getErrorCode() << endl;
        cout << e.getMessage() << endl;
    } catch (...) {
        cout << "Error during optimization" << endl;
    }
    return 0;
}
