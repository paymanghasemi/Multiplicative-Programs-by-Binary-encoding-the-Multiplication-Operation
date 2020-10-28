/* 
 * File:   main.cpp
 * Author: c3156840
 *
 * Created on 23 July 2013, 11:28 AM
 */
#include <gurobi_c++.h>
#include <cstdlib>
#include <stdio.h>
#include <cmath>
#include <random>
#include <fstream>
#include <string>


using namespace std;

#define A_min -30
#define A_max 30
#define z_min -10
#define z_max 10
#define N_Infinity -100000000
#define Sparsity_Matrix 0.5

#define div 4


/*------------------------------------------------------------------------------
 
 -----------------------------Public Variables----------------------------------
 
 -----------------------------------------------------------------------------*/
int N_Objectives;
int Row_N;
int Column_N;
int PFL;
int PINT;
int Sense;
double Sparsity_ObjFs;
int b_min;
int b_max;

/*------------------------------------------------------------------------------
 
 ------------------------Declaring CPLEX information----------------------------
 
 -----------------------------------------------------------------------------*/

void Parameter_setting() {
    Sparsity_ObjFs = 0.5;
    b_min = 1 * Sparsity_Matrix * Column_N;
    b_max = 10 * Sparsity_Matrix * Column_N;
}
bool Instance_Generator();

int main(int argc, char *argv[]) {
    N_Objectives = atoi(argv[1]);
    Sense = atoi(argv[2]);
    Column_N = atoi(argv[3]);
    Row_N = atoi(argv[4]);
    PFL = atoi(argv[5]);
    PINT = atoi(argv[6]);
    Parameter_setting();
    bool Completed = 0;
    while (Completed == 0) {
        Completed = Instance_Generator();
    }
    return 0;
}

bool Instance_Generator() {
    int** ObjF;
    int** Matrix;
    int* RHS;
    int Number_of_NonZeros;
    int* Number_of_NonZeros_In_Each_Row;
    int Counter;
    int save;
    bool done = 0;
    int FLOAT;
    int INT;
    int BOOL;
    int remain;
    int setter;
    bool assigned;

    GRBEnv env(true);
    env.start();
    env.set(GRB_IntParam_OutputFlag, 0);
    GRBModel Model(env);
    GRBVar* Variable;
    GRBVar* Obj_Variables;
    GRBLinExpr Math;
    GRBLinExpr Objective;


    random_device rd;
    mt19937_64 gen(rd());

    uniform_int_distribution<int> A_matrix_random(A_min, A_max);
    uniform_int_distribution<int> b_matrix_random(b_min, b_max);
    uniform_int_distribution<int> z_matrix_random(z_min, z_max);








    //------------------------------------------------------------------------------
    ObjF = new int* [N_Objectives];
    for (int i = 0; i < N_Objectives; i++) {
        ObjF[i] = new int [Column_N];
    }
    for (int i = 0; i < N_Objectives; i++) {
        for (int j = 0; j < Column_N; j++) {
            ObjF[i][j] = 0;
        }
    }



    Matrix = new int* [Row_N];
    for (int i = 0; i < Row_N; i++) {
        Matrix[i] = new int [Column_N];
    }
    for (int i = 0; i < Row_N; i++) {
        for (int j = 0; j < Column_N; j++) {
            Matrix[i][j] = 0;
        }
    }
    RHS = new int [Row_N];
    /*------------------------------------------------------------------------------
 
     ------------------------------Generating Matrix--------------------------------
 
     -----------------------------------------------------------------------------*/
    Number_of_NonZeros = Row_N * Column_N*Sparsity_Matrix;
    Number_of_NonZeros_In_Each_Row = new int [Row_N];
    for (int i = 0; i < Row_N; i++) {
        Number_of_NonZeros_In_Each_Row[i] = 1;
    }
    Number_of_NonZeros -= Row_N;
    while (Number_of_NonZeros > 0) {
        for (int i = 0; i < Row_N; i++) {
            if (Number_of_NonZeros == 0) {
                break;
            }
            if (Column_N - Number_of_NonZeros_In_Each_Row[i] > 0) {
                Counter = (fmin(Number_of_NonZeros, Column_N - Number_of_NonZeros_In_Each_Row[i]) / div) + 1;
                uniform_int_distribution<int> dis(1, Counter);
                save = dis(gen);
                Number_of_NonZeros_In_Each_Row[i] += save;
                Number_of_NonZeros -= save;
            }
        }
    }
    //------------Ensuring That Each Row and Each Variable are useful----------- 
    Counter = 0;
    for (int i = 0; i < Row_N; i++) {
        Matrix[i][Counter] = A_matrix_random(gen);
        Counter++;
        if (Counter == Column_N) {
            Counter = 0;
        }
        Number_of_NonZeros_In_Each_Row[i]--;
    }
    if (Row_N < Column_N) {
        while (Counter < Column_N) {
            for (int i = 0; i < Row_N; i++) {
                if (Counter >= Column_N) {
                    break;
                }
                if (Number_of_NonZeros_In_Each_Row[i] > 0) {
                    done = 0;
                    while (!done) {
                        save = abs(A_matrix_random(gen));
                        if (save != 0) {
                            Matrix[i][Counter] = save;
                            done = 1;
                            Counter++;
                            Number_of_NonZeros_In_Each_Row[i]--;
                        }
                    }
                }

            }
        }
    }
    //------------------Assigning Values to All Other Elements------------------ 
    for (int i = 0; i < Row_N; i++) {
        while (Number_of_NonZeros_In_Each_Row[i] > 0) {
            uniform_int_distribution<int> dis(0, Column_N - 1);

            done = 0;
            while (done == 0) {
                save = dis(gen);
                if (Matrix[i][save] == 0) {
                    Matrix[i][save] = A_matrix_random(gen);
                    done = 1;
                }
            }
            Number_of_NonZeros_In_Each_Row[i]--;
        }
    }

    /*------------------------------------------------------------------------------
 
     --------------------------Generating Objectives--------------------------------
 
     -----------------------------------------------------------------------------*/
    int* Obj_nonzeros;
    Obj_nonzeros = new int [N_Objectives];
    Number_of_NonZeros = N_Objectives * floor(fmax(1, Column_N * Sparsity_ObjFs));


    int* Objective_diversity;
    Objective_diversity = new int [Column_N];
    for (int i = 0; i < Column_N; i++) {
        Objective_diversity[i] = 0;
    }
    for (int i = 0; i < N_Objectives; i++) {
        Obj_nonzeros[i] = fmax(1, Column_N * Sparsity_ObjFs);
    }
    uniform_int_distribution<int> dis(0, Column_N - 1);
    int temp_location;
    while (Number_of_NonZeros > 0) {
        for (int i = 0; i < N_Objectives; i++) {
            uniform_int_distribution<int> Z_negative(z_min -10*i, -1);
            uniform_int_distribution<int> Z_positive(1, z_max + 10*i);
            if (Obj_nonzeros[i] > 0) {
                done = 0;
                while (!done) {
                    temp_location = dis(gen);
                    if (ObjF[i][temp_location] == 0) {
                        if (Objective_diversity[temp_location] > 0) {
                            save = Z_negative(gen);
                        } else if (Objective_diversity[temp_location] < 0) {
                            save = Z_positive(gen);
                        } else {
                            save = z_matrix_random(gen);
                        }
                        while (save != 0 && !done) {
                            ObjF[i][temp_location] = save;
                            done = 1;
                            Number_of_NonZeros--;
                            Obj_nonzeros[i]--;
                            if (save > 0) {
                                Objective_diversity[temp_location]++;
                            } else {
                                Objective_diversity[temp_location]--;
                            }
                        }
                    }
                }
            }
        }
    }



    /*------------------------------------------------------------------------------
 
     -----------------------Generating right hand side------------------------------
 
     -----------------------------------------------------------------------------*/
    for (int i = 0; i < Row_N; i++) {
        RHS[i] = b_matrix_random(gen);
    }


    /*--------------------------------------------------------------------------
 
     ------------------------Generating Model---------------------------
 
   ---------------------------------------------------------------------------*/
    Variable = new GRBVar [Column_N];
    char Name[25];
    remain = Column_N;
    FLOAT = ceil(Column_N * PFL / 100.0);
    int R_FLOAT = FLOAT;
    remain -= FLOAT;
    INT = floor(Column_N * PINT / 100.0);
    int R_INT = INT;
    remain -= INT;
    BOOL = remain;
    int R_BOOL = BOOL;
    uniform_int_distribution<int> One_to_Hundred(1, 99);
    for (int i = 0; i < Column_N; i++) {
        assigned = 0;
        while (assigned == 0) {
            setter = One_to_Hundred(gen);
            if (setter < 34 && FLOAT > 0) {
                sprintf(Name, "x%d", R_FLOAT - FLOAT + 1);
                Variable[i] = Model.addVar(0.0, GRB_INFINITY, 0, GRB_CONTINUOUS, Name);
                FLOAT--;
                assigned = 1;
            } else if (setter >= 34 && setter < 67 && INT > 0) {
                sprintf(Name, "i%d", R_INT - INT + 1);
                Variable[i] = Model.addVar(0.0, GRB_INFINITY, 0, GRB_INTEGER, Name);
                INT--;
                assigned = 1;
            } else if (setter >= 67 && BOOL > 0) {
                sprintf(Name, "b%d", R_BOOL - BOOL + 1);
                Variable[i] = Model.addVar(0.0, 1.0, 0, GRB_BINARY, Name);
                BOOL--;
                assigned = 1;

            }
        }
    }

    Obj_Variables = new GRBVar[N_Objectives];
    GRBConstr Objective_term[N_Objectives];
    for (int i = 0; i < N_Objectives; i++) {
        sprintf(Name, "Obj%d", i + 1);
        Obj_Variables[i] = Model.addVar(-GRB_INFINITY, GRB_INFINITY, 0, GRB_CONTINUOUS, Name);
        Objective_term[i] = Model.addConstr(Obj_Variables[i], GRB_EQUAL, 0.0);
    }
    GRBConstr Objective_constraints[N_Objectives];
    for (int i = 0; i < N_Objectives; i++) {
        Math.clear();
        Math += Obj_Variables[i];
        for (int j = 0; j < Column_N; j++) {
            Math -= ObjF[i][j] * Variable[j];
        }
        Objective_constraints[i] = Model.addConstr(Math, GRB_EQUAL, 0.0);
        Math.clear();
    }
    GRBConstr Constraints[Row_N];
    for (int i = 0; i < Row_N; i++) {
        Math.clear();
        for (int j = 0; j < Column_N; j++) {
            Math += (Matrix[i][j] * Variable[j]);
        }
        Constraints[i] = Model.addConstr(Math, GRB_LESS_EQUAL, RHS[i]);
        Math.clear();
    }

    for (int i = 0; i < N_Objectives; i++) {
        Model.remove(Objective_term[i]);
    }

    
    long double Obj_min[N_Objectives];
    for (int i = 0; i < N_Objectives; i++) {
        Objective = Obj_Variables[i];
        Model.setObjective(Objective, GRB_MAXIMIZE);
        Model.update();
        Model.optimize();
        if (Model.get(GRB_IntAttr_Status) == GRB_OPTIMAL) {
            Obj_min[i]=abs(Model.get(GRB_DoubleAttr_ObjBound));
            if (Obj_min[i] == 0) {
                return 0;
            }
        } else {
            return 0;
        }
    }
    
    for (int i = 0; i < N_Objectives; i++) {
        Model.remove(Objective_constraints[i]);
    }
    for (int i = 0; i < Row_N; i++) {
        Model.remove(Constraints[i]);
    }


    Model.reset(0);
    int counter = 1;
    for (int i = 0; i < N_Objectives; i++) {
        sprintf(Name, "C%d", counter);
        counter++;
        Objective_term[i] = Model.addConstr(Obj_Variables[i], GRB_EQUAL, 0.0, Name);
    }

    uniform_int_distribution<int> zmax_random(1, z_max);
    for (int i = 0; i < N_Objectives; i++) {
        Math.clear();
        for (int j = 0; j < Column_N; j++) {
            Math -= ObjF[i][j] * Variable[j];
        }
        Obj_min[i]+= zmax_random(gen);
        Math += Obj_Variables[i];
        sprintf(Name, "C%d", counter);
        counter++;
        Objective_constraints[i] = Model.addConstr(Math, GRB_EQUAL, Obj_min[i], Name);
        Math.clear();
    }
    for (int i = 0; i < Row_N; i++) {
        Math.clear();
        for (int j = 0; j < Column_N; j++) {
            Math += (Matrix[i][j] * Variable[j]);
        }
        sprintf(Name, "C%d", counter);
        counter++;
        Constraints[i] = Model.addConstr(Math, GRB_LESS_EQUAL, RHS[i], Name);
        Math.clear();
    }
    Objective.clear();
    Objective = 0;
    if (Sense == 1) {
        Model.setObjective(Objective, GRB_MAXIMIZE);
    } else {
        Model.setObjective(Objective, GRB_MINIMIZE);
    }
    Model.update();
    Model.write("Model.lp");

    return 1;
}

