/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "External_variables.h"
#include "Functions.h"

void Reading_LP_File_and_Generating_Gurobi_Variables(char* Input_file_name) {
    env.start();
    env.set(GRB_IntParam_OutputFlag, 0);
    Time_Limit=Time;
    Model = new GRBModel(env, Input_file_name);
    Model->set(GRB_IntParam_UpdateMode, 1);
    N_Variables = Model->get(GRB_IntAttr_NumVars);
    if(Model->get(GRB_IntAttr_NumIntVars)+Model->get(GRB_IntAttr_NumBinVars)<N_Variables){
        Continuous=1;
    }
    N_Constraints = Model->get(GRB_IntAttr_NumConstrs);
    ObjF = new GRBLinExpr [N_Objectives];
    Objective.clear();
    for (int i = 0; i < N_Objectives; i++) {
        Model->chgCoeff(Model->getConstr(i+N_Objectives),Model->getVar(i),precision);
    }
    for (int i = 0; i < N_Objectives; i++) {
        ObjF[i].clear();
        ObjF[i] = Model->getRow(Model->getConstr(i));
        Model->remove(Model->getConstr(i));
    }
    Y_star=new long double[N_Objectives];
    for(int i=0;i<N_Objectives;i++){
        Y_star[i]=-1;
    }
}