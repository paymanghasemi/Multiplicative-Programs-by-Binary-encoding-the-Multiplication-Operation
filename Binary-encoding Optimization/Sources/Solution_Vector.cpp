/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "External_variables.h"
#include "Functions.h"

vector<bool*> Temp_Vector_updater(vector<bool*> Temp_U, bool Current_value, int Preferred_value, bool* &U_Star) {

    int counter = 0;
    while (counter < Temp_U.size()) {
        if (Temp_U.at(counter)[Bit_location] != Current_value) {
            delete [] Temp_U.at(counter);
            Temp_U.erase(Temp_U.begin() + counter);
        } else {
            counter++;
        }
    }

    Bit_location--;
    counter = Temp_U.size() - 1;
    while (counter >= 0) {
        if (Temp_U.at(counter)[Bit_location] == Preferred_value) {
            U_Star[Bit_location] = Preferred_value;
            Bit_solution_time[Bit_location]=-101;
            Skipped_Inspect++;
            if (Bit_location != 0) {
                Model->addConstr(Expr_U[Address][Bit_location],GRB_EQUAL, Preferred_value);
                Temp_U = Temp_Vector_updater(Temp_U, Preferred_value, Preferred_value, U_Star);
            }else{
                Bit_location--;
            }
            break;
        } else {
            counter--;
        }
    }

    return Temp_U;
}
