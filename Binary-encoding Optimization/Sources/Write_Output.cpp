/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "External_variables.h"
#include "Functions.h"

void Writing_Output(char* Input_file_name, bool* U_Star, int Solution_approach, int Optimization_sense) {
    ofstream Output;
    Output.open("Result.txt", ios_base::app);

    Objective_value = 1;
    for (int i = 0; i < N_Objectives; i++) {
        if (Y_star[i] != -1) {
            Objective_value *= Y_star[i];
        } else {
            Objective_value = -1;
        }
    }
    if (Solution_approach != 2) {
        if (Interrupted == 1) {
            for (int i = Bit_location; i >= 0; i--) {
                U_Star[i] = Optimization_sense;
            }
            Gap_value = 0;
            for (int i = 0; i < N_bits_Objective; i++) {
                Gap_value += pow(2, i) * U_Star[i];
            }
            if (Gap_value == 0) {
                Gap_value = 1;
            } else {
                Gap_value *= pow(precision, N_Objectives);
            }
            if (Optimization_sense == 0) {
                Gap_value = ((Objective_value - Gap_value) / Objective_value);
            } else {
                Gap_value = ((Gap_value - Objective_value) / Gap_value);
            }
        } else {
            Gap_value = 0.0;
        }
    } else if (Interrupted == 1) {
        if (Objective_value != -1 && Gap_value != -1) {
            if (Optimization_sense == 0) {
                Gap_value = ((Objective_value - Gap_value) / Objective_value);
            } else {
                Gap_value = ((Gap_value - Objective_value) / Gap_value);
            }
        }
    }


    if (Gap_value > 1e-6 || Objective_value<0) {
        Total_time = Time;
    }


    Output << Input_file_name << " Obj_val= " << setprecision(floor(log10(Objective_value)) + 7) << Objective_value;
    Output << " Gap= " << setprecision(floor(log10(Objective_value)) + 6) << Gap_value;
    Output << " Sol_Time= " << setprecision(6) << Total_time << " N_var= " << N_Variables - N_Objectives << " N_const= " << N_Constraints - 2 * N_Objectives;
    Output << " Bits= " << N_bits_Objective << " IP_Solved= " << Total_IP_Solved << " Total_Skipped= " << N_bits_Objective - Total_IP_Solved;
    Output << " Skipped_Inspect= " << Skipped_Inspect;
    Output << " Skipped_Cut= " << Skipped_Cut;
    Output << " Skipped_Trick= " << Skipped_Trick<< endl;
    Output.close();
    if (Solution_approach != 2) {
        Output.open("Bit_time.txt", ios_base::app);
        Output << Input_file_name << " Total_Bits= " << N_bits_Objective << " ; ";
        for (int i = N_bits_Objective - 1; i >= 0; i--) {
            Output << setprecision(6) << Bit_solution_time[i] << " ";
        }
        Output << endl;
        Output.close();
    } else if (Interrupted == 0) {
        Output.open("Objective_values.txt", ios_base::app);
        Output << Input_file_name << " Valuese= ";
        for (int i = 0; i < N_Objectives; i++) {
            Output << setprecision(floor(log10(Y_star[i])) + 6) << Y_star[i] << " ";
        }
        Output << endl;
        Output.close();
    } else {
        Output.open("Objective_values.txt", ios_base::app);
        Output << Input_file_name << " N/A";
        Output << endl;
        Output.close();
    }
}



