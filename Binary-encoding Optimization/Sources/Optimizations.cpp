/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "External_variables.h"
#include "Functions.h"

long double* Upper_bound_finder(long double Trick, int Optimization_sense) {
    long double* Y_Upper_Bound;

    if (Trick == 0) {
        Y_Upper_Bound = new long double [N_Objectives];
    } else {
        Y_Upper_Bound = new long double [N_Objectives + 1];
    }

    for (int i = 0; i < N_Objectives; i++) {
        Model->setObjective(ObjF[i], GRB_MAXIMIZE);
        Model->set(GRB_DoubleParam_MIPGap, 0);
        Model->set(GRB_DoubleParam_MIPGapAbs, 0);
        Model->optimize();

        if (Model->get(GRB_IntAttr_Status) == GRB_OPTIMAL) {
            Y_Upper_Bound[i] = Model->get(GRB_DoubleAttr_ObjVal);

        } else {
            ofstream Output;
            Output.open("Result.txt", ios_base::app);
            Output << File_name <<" Objective_value= "<< "-2" << endl;
            Output.close();
            cout << "Error: Could not optimize objective " << i << " Error: " << Model->get(GRB_IntAttr_Status) << endl;
            exit(0);
        }
    }

    if (Trick == 1) {
        long double Objective_bound = 1;
        GRBVar temp_z = Model->addVar(0.0, GRB_INFINITY, 0.0, GRB_CONTINUOUS);
        GRBVar* temp_y = Model->addVars(N_Objectives, GRB_BINARY);
        GRBConstr* temp_const = new GRBConstr [N_Objectives + 1];
        Math.clear();
        for (int i = 0; i < N_Objectives; i++) {
            temp_const[i] = Model->addConstr(ObjF[i] - Y_Upper_Bound[i] * temp_y[i], GRB_LESS_EQUAL, temp_z);
            Math += temp_y[i];
        }
        temp_const[N_Objectives] = Model->addConstr(Math, GRB_EQUAL, N_Objectives - 1);
        Math = temp_z;
        Model->setObjective(Math, GRB_MINIMIZE);
        Model->set(GRB_IntParam_Threads, Num_threads);
        Model->set(GRB_DoubleParam_TimeLimit, Time_Limit);
        double Temp_time = clock();
        Model->optimize();
        Temp_time = (clock() - Temp_time) / CLOCKS_PER_SEC;
        Time_Limit = max(0.0, Time_Limit - Temp_time);
        Total_time += Temp_time;
        Total_IP_Solved++;
        if (Model->get(GRB_IntAttr_Status) == GRB_OPTIMAL) {
            for (int i = 0; i < N_Objectives; i++) {
                Objective_bound *= ObjF[i].getValue();
            }
            if (Time_Limit <= 0.0) {
                ofstream Output;
            Output.open("Result.txt", ios_base::app);
            Output << File_name <<" Objective_value= "<< "-11" << endl;
            Output.close();
                exit(0);
            }
        } else {
            ofstream Output;
            Output.open("Result.txt", ios_base::app);
            Output << File_name <<" Objective_value= "<< "-12" << endl;
            Output.close();
            exit(0);
        }
        Y_Upper_Bound[N_Objectives] = Objective_bound;
        Model->remove(temp_z);
        for (int i = 0; i < N_Objectives; i++) {
            Model->remove(temp_y[i]);
            Model->remove(temp_const[i]);
        }
        Model->remove(temp_const[N_Objectives]);
        delete [] temp_const;
        delete temp_y;
    } else if (Trick == 2) {
        long double Objective_bound[N_Objectives];
        double* Temp_var_value[N_Objectives];
        Model->update();
        double Temp_time = 0;
        for (int i = 0; i < N_Objectives; i++) {
            Temp_var_value[i]= new double [Model->get(GRB_IntAttr_NumVars)];
            Model->setObjective(ObjF[i], GRB_MINIMIZE);
            Model->set(GRB_IntParam_Threads, Num_threads);
            Model->set(GRB_DoubleParam_TimeLimit, Time_Limit);
            Temp_time = clock();
            Model->optimize();
            Temp_time = (clock() - Temp_time) / CLOCKS_PER_SEC;
            Time_Limit = max(0.0, Time_Limit - Temp_time);
            Total_time += Temp_time;
            Total_IP_Solved++;
            if (Model->get(GRB_IntAttr_Status) == GRB_OPTIMAL) {
                Objective_bound[i] = 1;
                for (int j = 0; j < N_Objectives; j++) {
                    Objective_bound[i] *= ObjF[j].getValue();
                }
                for(int j=0; j<Model->get(GRB_IntAttr_NumVars);j++){
                    Temp_var_value[i][j]=Model->getVar(j).get(GRB_DoubleAttr_X);
                }
                if (Time_Limit <= 0.0) {
                    ofstream Output;
                    Output.open("Result.txt", ios_base::app);
                    Output << File_name << " Objective_value= " << "-22" << endl;
                    Output.close();
                    exit(0);
                }
            } else {
                ofstream Output;
                Output.open("Result.txt", ios_base::app);
                Output << File_name << " Objective_value= " << "-23" << endl;
                Output.close();
                exit(0);
            }
        }
        Y_Upper_Bound[N_Objectives] = Objective_bound[0];
        int Optimal_address=0;
        for (int j = 1; j < N_Objectives; j++) {
            if (Y_Upper_Bound[N_Objectives] > Objective_bound[j]) {
                Y_Upper_Bound[N_Objectives] = Objective_bound[j];
                Optimal_address=j;
            }
        }
        for(int i=0;i<Model->get(GRB_IntAttr_NumVars);i++){
            Model->getVar(i).set(GRB_DoubleAttr_Start,Temp_var_value[Optimal_address][i]);
        }
    }
    return Y_Upper_Bound;
}

bool* Bitwise_optimization(int Optimization_sense, int Single_bit_Cut, long double Trick) {
    bool* U_Star;
    U_Star = new bool [N_bits_Objective];
    vector<bool*> Temp_U;

    for (int i = 0; i < N_bits_Objective; i++) {
        U_Star[i] = 1 - Optimization_sense;
    }

    if (Trick != 0) {
        Trick = floor(log2(Trick)) + 1;
        for (int i = Trick; i < N_bits_Objective; i++) {
            Model->addConstr(Expr_U[Address][i], GRB_EQUAL, Optimization_sense);
            U_Star[i] = Optimization_sense;
            Skipped_Trick++;
            Bit_solution_time[i] = -303;
        }
        Bit_location = Trick - 1;

    } else {
        Bit_location = N_bits_Objective - 1;
    }

    GRBConstr Temp_Constraint;
    int Distance_to_first_Preferred = 0;

    bool First_iteration = 1;
    while (Bit_location >= 0 && Time_Limit > 0) {

        if (Optimization_sense == 1) {
            Model->setObjective(Expr_U[Address][Bit_location], GRB_MAXIMIZE);
        } else {
            Model->setObjective(Expr_U[Address][Bit_location], GRB_MINIMIZE);
        }

        Model->set(GRB_IntParam_Threads, Num_threads);
        Model->set(GRB_DoubleParam_TimeLimit, Time_Limit);
        Bit_solution_time[Bit_location] = clock();
        Model->optimize();
        Bit_solution_time[Bit_location] = clock() - Bit_solution_time[Bit_location];
        Total_IP_Solved++;
        Bit_solution_time[Bit_location] /= CLOCKS_PER_SEC;
        Time_Limit = max(0.0, Time_Limit - Bit_solution_time[Bit_location]);
        Total_time += Bit_solution_time[Bit_location];
        if ((Model->get(GRB_IntAttr_Status) == GRB_INFEASIBLE || Model->get(GRB_IntAttr_Status) == GRB_INF_OR_UNBD) && Single_bit_Cut == 2) {

            for (int i = 0; i <= Bit_location; i++) {
                U_Star[i] = Temp_U.back()[i];
            }

            for (int i = 0; i < Bit_location; i++) {
                Bit_solution_time[i] = -202;
            }
            Skipped_Cut += Bit_location;
            break;
        } else if ((Model->get(GRB_IntAttr_Status) == GRB_INFEASIBLE || Model->get(GRB_IntAttr_Status) == GRB_INF_OR_UNBD) && Single_bit_Cut == 1) {
            for (int i = 0; i < Distance_to_first_Preferred; i++) {
                U_Star[Bit_location - i] = 1 - Optimization_sense;
                Model->addConstr(Expr_U[Address][Bit_location - i], GRB_EQUAL, 1 - Optimization_sense);
            }
            for (int i = 1; i < Distance_to_first_Preferred; i++) {
                Bit_solution_time[Bit_location - i] = -202;
            }
            Skipped_Cut += Distance_to_first_Preferred - 1;

            Bit_location -= Distance_to_first_Preferred - 1;
            if (Bit_location == 0) {
                break;
            }
            Temp_U = Temp_Vector_updater(Temp_U, U_Star[Bit_location], Optimization_sense, U_Star);
        } else if (Model->get(GRB_IntAttr_Status) == GRB_TIME_LIMIT) {
            Interrupted = 1;
            return U_Star;
        } else if (Model->get(GRB_IntAttr_Status) != GRB_OPTIMAL) {
            ofstream Output;
            Output.open("Result.txt", ios_base::app);
            Output << File_name <<" Objective_value= "<< "-1" << endl;
            Output.close();
            cout << "Error: Could not optimize Bit " << Bit_location << " Error: " << Model->get(GRB_IntAttr_Status) << endl;
            exit(0);
        } else if (Model->get(GRB_IntAttr_Status) == GRB_OPTIMAL) {
            /*************************************************/
            long double temp_objective_value = 1;
            for (int i = 0; i < N_Objectives; i++) {
                temp_objective_value *= ObjF[i].getValue() * precision;
            }
            if (Optimization_sense == 0 && temp_objective_value < Objective_value) {
                Objective_value = temp_objective_value;
                for (int i = 0; i < N_Objectives; i++) {
                    Y_star[i] = ObjF[i].getValue() * precision;
                }
            } else if (Optimization_sense == 1 && temp_objective_value > Objective_value) {
                Objective_value = temp_objective_value;
                for (int i = 0; i < N_Objectives; i++) {
                    Y_star[i] = ObjF[i].getValue() * precision;
                }
            }
            /******************************************/
            U_Star[Bit_location] = round(Model->get(GRB_DoubleAttr_ObjVal));
            if (Bit_location == 0) {
                break;
            }
            Temp_U.push_back(new bool [N_bits_Objective]);
            for (int i = Bit_location; i >= 0; i--) {
                Temp_U.back()[i] = round(Expr_U[Address][i].getValue());
                U_Star[i] = round(Expr_U[Address][i].getValue());
            }
            Model->addConstr(Expr_U[Address][Bit_location], GRB_EQUAL, U_Star[Bit_location]);
            Temp_U = Temp_Vector_updater(Temp_U, U_Star[Bit_location], Optimization_sense, U_Star);
        }

        if (!First_iteration) {
            Model->remove(Temp_Constraint);
        } else {
            First_iteration = 0;
        }
        if (Single_bit_Cut == 1 && Bit_location > 0) {
            Distance_to_first_Preferred = 0;
            Math.clear();
            for (int j = Bit_location; j >= 0; j--) {
                if (Temp_U.back()[j] != Optimization_sense) {
                    Distance_to_first_Preferred += 1;
                    Math += Expr_U[Address][j];
                } else {
                    break;
                }
            }

            if (Optimization_sense == 1) {
                Temp_Constraint = Model->addConstr(Math, GRB_GREATER_EQUAL, 1);
            } else {
                Temp_Constraint = Model->addConstr(Math, GRB_LESS_EQUAL, Distance_to_first_Preferred - 1);
            }
        } else if (Single_bit_Cut == 2 && Bit_location > 0) {
            Distance_to_first_Preferred = 0;
            Math.clear();

            for (int j = 0; j <= Bit_location; j++) {
                if (Temp_U.back()[j] != Optimization_sense) {
                    Math += Expr_U[Address][j];
                    Distance_to_first_Preferred += 1 - Optimization_sense;
                }
            }
            if (Optimization_sense == 1) {
                Temp_Constraint = Model->addConstr(Math, GRB_GREATER_EQUAL, 1);
            } else {
                Temp_Constraint = Model->addConstr(Math, GRB_LESS_EQUAL, Distance_to_first_Preferred - 1);
            }
        }
    }

    return U_Star;
}

bool* Bitwise_optimization(int Optimization_sense, long double Trick) {
    bool* U_Star;
    U_Star = new bool [N_bits_Objective];
    vector<bool*> Temp_U;
    for (int i = 0; i < N_bits_Objective; i++) {
        U_Star[i] = 1 - Optimization_sense;
    }
    if (Trick != 0) {
        Trick = floor(log2(Trick)) + 1;
        for (int i = Trick; i < N_bits_Objective; i++) {
            Model->addConstr(Expr_U[Address][i], GRB_EQUAL, Optimization_sense);
            U_Star[i] = Optimization_sense;
            Skipped_Trick++;
            Bit_solution_time[i] = -303;
        }
        Bit_location = Trick - 1;

    } else {
        Bit_location = N_bits_Objective - 1;
    }


    while (Bit_location >= 0 && Time_Limit > 0) {
        if (Optimization_sense == 1) {
            Model->setObjective(Expr_U[Address][Bit_location], GRB_MAXIMIZE);
        } else {
            Model->setObjective(Expr_U[Address][Bit_location], GRB_MINIMIZE);
        }


        Model->set(GRB_IntParam_Threads, Num_threads);
        Model->set(GRB_DoubleParam_TimeLimit, Time_Limit);
        Bit_solution_time[Bit_location] = clock();
        Model->optimize();
        Bit_solution_time[Bit_location] = clock() - Bit_solution_time[Bit_location];
        Total_IP_Solved++;
        Bit_solution_time[Bit_location] /= CLOCKS_PER_SEC;
        Time_Limit = max(0.0, Time_Limit - Bit_solution_time[Bit_location]);
        Total_time += Bit_solution_time[Bit_location];
        if (Model->get(GRB_IntAttr_Status) == GRB_TIME_LIMIT) {
            Interrupted = 1;
            return U_Star;
        } else if (Model->get(GRB_IntAttr_Status) != GRB_OPTIMAL) {
            ofstream Output;
            Output.open("Result.txt", ios_base::app);
            Output << File_name <<" Objective_value= "<< "-1" << endl;
            Output.close();
            cout << "Error: Could not optimize Bit " << Bit_location << " Error: " << Model->get(GRB_IntAttr_Status) << endl;
            exit(0);
        }
        long double temp_objective_value = 1;
        for (int i = 0; i < N_Objectives; i++) {
            temp_objective_value *= ObjF[i].getValue() * precision;
        }
        if (Optimization_sense == 0 && temp_objective_value < Objective_value) {
            Objective_value = temp_objective_value;
            for (int i = 0; i < N_Objectives; i++) {
                Y_star[i] = ObjF[i].getValue() * precision;
            }
        } else if (Optimization_sense == 1 && temp_objective_value > Objective_value) {
            Objective_value = temp_objective_value;
            for (int i = 0; i < N_Objectives; i++) {
                Y_star[i] = ObjF[i].getValue() * precision;
            }
        }

        U_Star[Bit_location] = round(Model->get(GRB_DoubleAttr_ObjVal));

        if (Bit_location == 0) {
            break;
        }


        Temp_U.push_back(new bool [N_bits_Objective]);
        for (int i = Bit_location; i >= 0; i--) {
            Temp_U.back()[i] = round(Expr_U[Address][i].getValue());
            U_Star[i] = round(Expr_U[Address][i].getValue());
        }
        Model->addConstr(Expr_U[Address][Bit_location], GRB_EQUAL, U_Star[Bit_location]);
        Temp_U = Temp_Vector_updater(Temp_U, U_Star[Bit_location], Optimization_sense, U_Star);

    }

    return U_Star;
}

bool* Binarized_optimization(int Optimization_sense, long double Trick) {
    bool* U_Star;
    U_Star = new bool [N_bits_Objective];
    Math.clear();
    for (int i = 0; i < N_bits_Objective; i++) {
        U_Star[i] = 1 - Optimization_sense;
    }

    if (Trick != 0) {
        Trick = floor(log2(Trick)) + 1;
        for (int i = Trick; i < N_bits_Objective; i++) {
            Model->addConstr(Expr_U[Address][i], GRB_LESS_EQUAL, Optimization_sense);
            U_Star[i] = Optimization_sense;
            Skipped_Trick++;
        }
        Math.clear();
        for (int i = 0; i < Trick; i++) {
            Math += pow(2, i) * Expr_U[Address][i];
        }
    } else {
        Math.clear();
        for (int i = 0; i < N_bits_Objective; i++) {
            Math += pow(2, i) * Expr_U[Address][i];
        }

    }


    if (Optimization_sense == 1) {
        Model->setObjective(Math, GRB_MAXIMIZE);
    } else {
        Model->setObjective(Math, GRB_MINIMIZE);
    }

    Model->set(GRB_IntParam_Threads, Num_threads);
    Model->set(GRB_DoubleParam_TimeLimit, Time_Limit);
    //Model->set(GRB_IntParam_NumericFocus,1);
    double Temp_time = clock();
    Model->optimize();
    Temp_time = (clock() - Temp_time)/CLOCKS_PER_SEC;
    Total_IP_Solved++;
    Total_time += Temp_time;
    if (Model->get(GRB_IntAttr_Status) == GRB_TIME_LIMIT) {
        Interrupted = 1;
        if (Model->get(GRB_IntAttr_SolCount) != 0) {
            for (int i = 0; i < N_Objectives; i++) {
                Y_star[i] = ObjF[i].getValue() * precision;
            }
            Gap_value = Model->get(GRB_DoubleAttr_ObjBound) * pow(precision, N_Objectives);
        } else {
            Gap_value = -1;
        }

        return U_Star;
    } else if (Model->get(GRB_IntAttr_Status) != GRB_OPTIMAL) {
        ofstream Output;
        Output.open("Result.txt", ios_base::app);
        Output << File_name << " Objective_value= " << "-1" << endl;
        Output.close();
        cout << Model->get(GRB_IntAttr_Status) << endl;
        cout << "Error: Could not optimize objective " << Bit_location << endl;
        exit(0);
    }
    for (int i = 0; i < N_Objectives; i++) {
        Y_star[i] = ObjF[i].getValue() * precision;
    }

    Objective_value = Model->get(GRB_DoubleAttr_ObjVal) * pow(precision, N_Objectives);
    Gap_value = 0.0;
    for (int i = 0; i < N_bits_Objective; i++) {
        U_Star[i] = round(Expr_U[Address][i].getValue());
    }



    return U_Star;
}

bool* Optimize(int Optimization_sense, int Solution_approach, int Single_bit_Cut, long double Trick) {

    Bit_solution_time = new double [N_bits_Objective];
    for (int i = 0; i < N_bits_Objective; i++) {
        Bit_solution_time[i] = 0;
    }

    if (Solution_approach == 1) {
        if (Single_bit_Cut != 0) {
            return Bitwise_optimization(Optimization_sense, Single_bit_Cut, Trick);
        } else {
            return Bitwise_optimization(Optimization_sense, Trick);
        }
    } else if (Solution_approach == 2) {
        return Binarized_optimization(Optimization_sense, Trick);
    }
}
