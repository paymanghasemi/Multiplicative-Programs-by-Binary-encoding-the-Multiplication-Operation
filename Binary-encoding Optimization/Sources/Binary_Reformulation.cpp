/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "External_variables.h"
#include "Functions.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <Multiplication.h>



void Insert_Node(vector<Remaining_Multiplications> &Vector_Remaining,Remaining_Multiplications Node){
    Vector_Remaining.insert(Vector_Remaining.begin(),Node);
}



void Binary_reformulation(long double* Y_Upper_Bound, int Optimization_sense) {

    Var_b = new GRBVar* [N_Objectives];
    Math.clear();
    int N_bits_Y[N_Objectives];
    Var_alpha = new GRBVar* [N_Objectives - 1];
    Var_C = new GRBVar* [N_Objectives - 1];
    Expr_V = new GRBLinExpr* [N_Objectives - 1];
    Expr_U = new GRBLinExpr* [N_Objectives - 1];
    vector<Remaining_Multiplications> Vector_Remaining;
    for (int i = 0; i < N_Objectives; i++) {
        N_bits_Y[i] = floor(log2(Y_Upper_Bound[i])) + 1;
        Var_b[i] = Model->addVars(N_bits_Y[i], GRB_BINARY);
        Math.clear();
        for (int j = 0; j < N_bits_Y[i]; j++) {
            Math += pow(2, j) * Var_b[i][j];
        }
        /**************************************************/

        if (!Continuous) {
            Model->addConstr(ObjF[i], GRB_EQUAL, Math);
        } else {
            if (Optimization_sense == 0) {
                Model->addConstr(ObjF[i], GRB_LESS_EQUAL, Math);
            } else {
                Model->addConstr(ObjF[i], GRB_GREATER_EQUAL, Math);
            }
        }
        /**************************************************/
        Math.clear();
        Remaining_Multiplications Node;
        Node.Address=i;
        Node.Is_Expr=0;
        Node.Max_value=Y_Upper_Bound[i];
        Node.N_bits=N_bits_Y[i];
        Insert_Node(Vector_Remaining,Node);
    }
    int counter=0;
    while(Vector_Remaining.size()>1){
        Remaining_Multiplications Node[2];
        Node[0]=Vector_Remaining.front();
        Vector_Remaining.erase(Vector_Remaining.begin());
        Node[1]=Vector_Remaining.front();
        Vector_Remaining.erase(Vector_Remaining.begin());
        Remaining_Multiplications New_Node;
        if(Node[0].Is_Expr && Node[1].Is_Expr){
            New_Node=Both_Expr(Node,counter);
        }else if(!Node[0].Is_Expr && Node[1].Is_Expr){
            New_Node=Obj_Expr(Node[0],Node[1],counter);
        }else if(Node[0].Is_Expr && !Node[1].Is_Expr){
            New_Node=Obj_Expr(Node[1],Node[0],counter);
        }else{
            New_Node=Both_Obj(Node,counter);
        }
        Insert_Node(Vector_Remaining,New_Node);
        counter++;
    }
    
    N_bits_Objective = Vector_Remaining.front().N_bits;
    if(Optimization_sense==1){
        Objective_value=0;
    }else{
        Objective_value=Vector_Remaining.front().Max_value;
    }
    Address=Vector_Remaining.front().Address;
    Vector_Remaining.erase(Vector_Remaining.begin());
}
