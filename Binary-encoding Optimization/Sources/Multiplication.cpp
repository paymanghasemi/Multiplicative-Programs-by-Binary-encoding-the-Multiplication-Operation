/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "External_variables.h"
#include <Multiplication.h>

int* add_one(int* Combination,int location,int N_bits[2]){
    if(Combination[location]+1<N_bits[location]){
        Combination[location]++;
        return Combination;
    }else{
        Combination[location]=0;
        location++;
        if(location<2){
            return add_one(Combination,location,N_bits);
        }else{
            Combination[0]=-101;
            return Combination;
        }
    }
}

Remaining_Multiplications Both_Expr(Remaining_Multiplications* Node, int counter) {
    Remaining_Multiplications New_Node;
    New_Node.Address = counter;
    New_Node.Is_Expr = 1;
    New_Node.Max_value = Node[0].Max_value * Node[1].Max_value;
    New_Node.N_bits = floor(log2(New_Node.Max_value)) + 1;

    long int N_Combinations = 1;
    int Bit = 0;
    int* Combination = new int [2];
    Combination[0] = 0;
    Combination[1] = 0;
    N_Combinations = Node[0].N_bits * Node[1].N_bits;
    Var_alpha[counter] = Model->addVars(N_Combinations, GRB_BINARY);
    Var_C[counter] = Model->addVars(New_Node.N_bits, GRB_INTEGER);
    Expr_V[counter] = new GRBLinExpr [New_Node.N_bits];
    Expr_U[counter] = new GRBLinExpr [New_Node.N_bits];
    int counter2 = 0;
    while (Combination[0] != -101) {
        if (Bit < New_Node.N_bits) {

            Model->addConstr(Var_alpha[counter][counter2], GRB_LESS_EQUAL, Expr_U[Node[0].Address][Combination[0]]);
            Model->addConstr(Var_alpha[counter][counter2], GRB_LESS_EQUAL, Expr_U[Node[1].Address][Combination[1]]);
            Model->addConstr(Var_alpha[counter][counter2] +1.0, GRB_GREATER_EQUAL, Expr_U[Node[0].Address][Combination[0]] +Expr_U[Node[1].Address][Combination[1]]);
            Math.clear();
            Expr_V[counter][Bit] += Var_alpha[counter][counter2];
        } else {
            Model->addConstr(1.0, GRB_GREATER_EQUAL, Expr_U[Node[0].Address][Combination[0]] +Expr_U[Node[1].Address][Combination[1]]);
            Math.clear();
            Model->remove(Var_alpha[counter][counter2]);
        }
        counter2++;
        int N_bits[2]={Node[0].N_bits,Node[1].N_bits};
        Combination = add_one(Combination, 0, N_bits);
        Bit = Combination[0] + Combination[1];
    }
    Expr_U[counter][0] = Expr_V[counter][0];
    Model->addConstr(Var_C[counter][0], GRB_EQUAL, 0.0);
    Model->addConstr(Var_C[counter][New_Node.N_bits - 1], GRB_EQUAL, 0.0);
    for (int j = 1; j < New_Node.N_bits; j++) {
        Model->addConstr(2 * Var_C[counter][j], GRB_LESS_EQUAL, Expr_V[counter][j] + Var_C[counter][j - 1]);
        Model->addConstr(Expr_V[counter][j] + Var_C[counter][j - 1], GRB_LESS_EQUAL, 1 + 2 * Var_C[counter][j]);
        Expr_U[counter][j] = Expr_V[counter][j] + Var_C[counter][j - 1] - 2 * Var_C[counter][j];
        Model->addConstr(Expr_U[counter][j], GRB_LESS_EQUAL, 1.0);
        Model->addConstr(Expr_U[counter][j], GRB_GREATER_EQUAL, 0.0);
    }
    delete [] Combination;
    return New_Node;
}


Remaining_Multiplications Both_Obj(Remaining_Multiplications* Node, int counter) {
    Remaining_Multiplications New_Node;
    New_Node.Address = counter;
    New_Node.Is_Expr = 1;
    New_Node.Max_value = Node[0].Max_value * Node[1].Max_value;
    New_Node.N_bits = floor(log2(New_Node.Max_value)) + 1;

    long int N_Combinations = 1;
    int Bit = 0;
    int* Combination = new int [2];
    Combination[0] = 0;
    Combination[1] = 0;
    N_Combinations = Node[0].N_bits * Node[1].N_bits;
    Var_alpha[counter] = Model->addVars(N_Combinations, GRB_BINARY);
    Var_C[counter] = Model->addVars(New_Node.N_bits, GRB_INTEGER);
    Expr_V[counter] = new GRBLinExpr [New_Node.N_bits];
    Expr_U[counter] = new GRBLinExpr [New_Node.N_bits];
    int counter2 = 0;
    while (Combination[0] != -101) {
        if (Bit < New_Node.N_bits) {

            Model->addConstr(Var_alpha[counter][counter2], GRB_LESS_EQUAL, Var_b[Node[0].Address][Combination[0]]);
            Model->addConstr(Var_alpha[counter][counter2], GRB_LESS_EQUAL, Var_b[Node[1].Address][Combination[1]]);
            Model->addConstr(Var_alpha[counter][counter2] +1.0, GRB_GREATER_EQUAL, Var_b[Node[0].Address][Combination[0]] +Var_b[Node[1].Address][Combination[1]]);
            Math.clear();
            Expr_V[counter][Bit] += Var_alpha[counter][counter2];
        } else {
            Model->addConstr(1.0, GRB_GREATER_EQUAL, Var_b[Node[0].Address][Combination[0]] +Var_b[Node[1].Address][Combination[1]]);
            Math.clear();
            Model->remove(Var_alpha[counter][counter2]);
        }
        counter2++;
        int N_bits[2]={Node[0].N_bits,Node[1].N_bits};
        Combination = add_one(Combination, 0, N_bits);
        Bit = Combination[0] + Combination[1];
    }
    Expr_U[counter][0] = Expr_V[counter][0];
    Model->addConstr(Var_C[counter][0], GRB_EQUAL, 0.0);
    Model->addConstr(Var_C[counter][New_Node.N_bits - 1], GRB_EQUAL, 0.0);
    for (int j = 1; j < New_Node.N_bits; j++) {
        Model->addConstr(2 * Var_C[counter][j], GRB_LESS_EQUAL, Expr_V[counter][j] + Var_C[counter][j - 1]);
        Model->addConstr(Expr_V[counter][j] + Var_C[counter][j - 1], GRB_LESS_EQUAL, 1 + 2 * Var_C[counter][j]);
        Expr_U[counter][j] = Expr_V[counter][j] + Var_C[counter][j - 1] - 2 * Var_C[counter][j];
        Model->addConstr(Expr_U[counter][j], GRB_LESS_EQUAL, 1.0);
        Model->addConstr(Expr_U[counter][j], GRB_GREATER_EQUAL, 0.0);
    }
    delete [] Combination;
    return New_Node;
}


Remaining_Multiplications Obj_Expr(Remaining_Multiplications Node_Obj, Remaining_Multiplications Node_Expr, int counter){
    Remaining_Multiplications New_Node;
    New_Node.Address = counter;
    New_Node.Is_Expr = 1;
    New_Node.Max_value = Node_Obj.Max_value * Node_Expr.Max_value;
    New_Node.N_bits = floor(log2(New_Node.Max_value)) + 1;

    long int N_Combinations = 1;
    int Bit = 0;
    int* Combination = new int [2];
    Combination[0] = 0;
    Combination[1] = 0;
    N_Combinations = Node_Obj.N_bits * Node_Expr.N_bits;
    Var_alpha[counter] = Model->addVars(N_Combinations, GRB_BINARY);
    Var_C[counter] = Model->addVars(New_Node.N_bits, GRB_INTEGER);
    Expr_V[counter] = new GRBLinExpr [New_Node.N_bits];
    Expr_U[counter] = new GRBLinExpr [New_Node.N_bits];
    int counter2 = 0;
    while (Combination[0] != -101) {
        if (Bit < New_Node.N_bits) {

            Model->addConstr(Var_alpha[counter][counter2], GRB_LESS_EQUAL, Var_b[Node_Obj.Address][Combination[0]]);
            Model->addConstr(Var_alpha[counter][counter2], GRB_LESS_EQUAL, Expr_U[Node_Expr.Address][Combination[1]]);
            Model->addConstr(Var_alpha[counter][counter2] +1.0, GRB_GREATER_EQUAL, Var_b[Node_Obj.Address][Combination[0]] +Expr_U[Node_Expr.Address][Combination[1]]);
            Math.clear();
            Expr_V[counter][Bit] += Var_alpha[counter][counter2];
        } else {
            Model->addConstr(1.0, GRB_GREATER_EQUAL, Var_b[Node_Obj.Address][Combination[0]] +Expr_U[Node_Expr.Address][Combination[1]]);
            Math.clear();
            Model->remove(Var_alpha[counter][counter2]);
        }
        counter2++;
        int N_bits[2]={Node_Obj.N_bits,Node_Expr.N_bits};
        Combination = add_one(Combination, 0, N_bits);
        Bit = Combination[0] + Combination[1];
    }
    Expr_U[counter][0] = Expr_V[counter][0];
    Model->addConstr(Var_C[counter][0], GRB_EQUAL, 0.0);
    Model->addConstr(Var_C[counter][New_Node.N_bits - 1], GRB_EQUAL, 0.0);
    for (int j = 1; j < New_Node.N_bits; j++) {
        Model->addConstr(2 * Var_C[counter][j], GRB_LESS_EQUAL, Expr_V[counter][j] + Var_C[counter][j - 1]);
        Model->addConstr(Expr_V[counter][j] + Var_C[counter][j - 1], GRB_LESS_EQUAL, 1 + 2 * Var_C[counter][j]);
        Expr_U[counter][j] = Expr_V[counter][j] + Var_C[counter][j - 1] - 2 * Var_C[counter][j];
        Model->addConstr(Expr_U[counter][j], GRB_LESS_EQUAL, 1.0);
        Model->addConstr(Expr_U[counter][j], GRB_GREATER_EQUAL, 0.0);
    }
    delete [] Combination;
    return New_Node;
}