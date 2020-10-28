/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Multiplication.h
 * Author: payman
 *
 * Created on May 15, 2020, 10:13 PM
 */

#ifndef MULTIPLICATION_H
#define MULTIPLICATION_H
struct Remaining_Multiplications {
    int N_bits;
    long double Max_value;
    bool Is_Expr;
    int Address;
};

Remaining_Multiplications Both_Expr(Remaining_Multiplications* Node, int counter);
Remaining_Multiplications Obj_Expr(Remaining_Multiplications Node0, Remaining_Multiplications Node1, int counter);
Remaining_Multiplications Both_Obj(Remaining_Multiplications* Node, int counter);


#endif /* MULTIPLICATION_H */

