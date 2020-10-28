/* Copyright 2018, Gurobi Optimization, LLC */

/* Want to cover three different sets but subject to a common budget of
 * elements allowed to be used. However, the sets have different priorities to
 * be covered; and we tackle this by using multi-objective optimization. */

#include "gurobi_c++.h"
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sys/time.h>
#include <ctime>
#include <math.h>
#include <unistd.h>
#include <iomanip>  

using namespace std;

#define Output_Precision 10
#define Relative_Gap 1.0e-6
// Tolerance for barrier convergence.
#define CONVTOL 1.0e-12
#define Num_threads 1 
#define Denomiator 1.0e-12
#define Time_Limit 3600 

char* IP_file_name;
int N_Variables;
int N_Objectives;
ofstream Final_Result;
double Objective_Value(-1);
struct timeval startTime, endTime;
double totalTime(0);
double clock_Run_time(0);
double clock_start_time;
double check = 1;
int K;
int pow_val;
int Temp_sum_W(0);
int counter(0);
int N_Constraints;
double Global_UB;
bool IS_MIP;
double error;

double tune_Run_time(0);
double tune_start_time;

GRBEnv env(true);
GRBModel* Model;
GRBVar* Variable;
GRBLinExpr* ObjF;
GRBConstr* Constraints;
GRBVar** Tau_var;
GRBVar Ratio_var;
GRBVar Small_lambda_var;
GRBLinExpr Objective;
GRBVar Large_lambda_var;


int main(int argc, char *argv[]) {
    IP_file_name = argv[1];
    N_Objectives = atoi(argv[2]);
    env.start();
    env.set(GRB_IntParam_OutputFlag, 0);
    env.set(GRB_IntParam_Threads, 1);
    Model = new GRBModel(env, IP_file_name);
    Variable = Model->getVars();
    N_Variables = Model->get(GRB_IntAttr_NumVars) - N_Objectives;
    N_Constraints = Model->get(GRB_IntAttr_NumConstrs) - 2 * N_Objectives;
    ObjF = new GRBLinExpr [N_Objectives];
    for (int i = 0; i < N_Objectives; i++) {
        ObjF[i].clear();
        ObjF[i] = Model->getRow(Model->getConstr(i));
        Model->remove(Model->getConstr(i));
    }

    Small_lambda_var = Model->addVar(0.0, GRB_INFINITY, 0, GRB_CONTINUOUS);
    Large_lambda_var = Model->addVar(0.0, GRB_INFINITY, 0, GRB_CONTINUOUS);
    Objective = Small_lambda_var;

    Model->addConstr(Small_lambda_var, GRB_LESS_EQUAL, Large_lambda_var);

    for (int i = 0; i <= N_Objectives; i++) {
        pow_val = pow(2, i);
        if (pow_val >= N_Objectives) {
            K = i;
            break;
        }
    }

    Tau_var = new GRBVar* [K];
    for (int i = 0; i < K; i++) {
        Tau_var[i] = new GRBVar [pow_val + 1];
    }
    for (int i = 0; i < K; i++) {
        for (int j = 0; j <= pow_val; j++) {
            Tau_var[i][j] = Model->addVar(0.0, GRB_INFINITY, 0, GRB_CONTINUOUS);
        }
    }
    Model->addQConstr(Large_lambda_var * Large_lambda_var, GRB_LESS_EQUAL, Tau_var[K - 1][1] * Tau_var[K - 1][2]);
    for (int i = 1; i <= K - 1; i++) {
        for (int j = 1; j <= pow(2, K - i); j++) {
            Model->addQConstr(Tau_var[i][j] * Tau_var[i][j], GRB_LESS_EQUAL, Tau_var[i - 1][2 * j - 1] * Tau_var[i - 1][2 * j ]);
        }
    }

    for (int j = 1; j <= pow_val; j++) {
        if (counter < N_Objectives) {
            Model->addConstr(Tau_var[0][j], GRB_EQUAL, ObjF[counter]);
            counter++;
        } else {
            Model->addConstr(Tau_var[0][j], GRB_EQUAL, Large_lambda_var);
        }
    }
    Model->setObjective(Objective, GRB_MAXIMIZE);

    
    IS_MIP = Model->get(GRB_IntAttr_IsMIP);
    Model->set(GRB_DoubleParam_TimeLimit, Time_Limit);
    Model->set(GRB_IntParam_Threads, Num_threads);
    if(IS_MIP){
    Model->set(GRB_DoubleParam_MIPGap,0.0);
    Model->set(GRB_DoubleParam_MIPGapAbs,0.0);}
    //	Model->set(GRB_IntParam_NumericFocus, 1);


    try {
    	clock_start_time = clock();
        Model->update();
	
        Model->optimize();

        clock_Run_time += (clock() - clock_start_time);
        clock_Run_time /= CLOCKS_PER_SEC;


        if (Model->get(GRB_IntAttr_Status) == GRB_OPTIMAL || Model->get(GRB_IntAttr_Status) == GRB_TIME_LIMIT) {
            ofstream Output;
            Output.open("SOCP_Result.txt", ios_base::app);
            Output << argv[1] << " Obj_value= " << setprecision(floor(N_Objectives*log10(Model->get(GRB_DoubleAttr_ObjVal))) + 7) << pow(Model->get(GRB_DoubleAttr_ObjVal),N_Objectives);
            if(IS_MIP){
            Output << " Gap= " << setprecision(3) << Model->get(GRB_DoubleAttr_MIPGap);
            if(Model->get(GRB_DoubleAttr_MIPGap) >= 1e-6){
                clock_Run_time=Time_Limit;
            }
            }else{
                Output << " Gap= " << setprecision(3) << "NA";
            }
            Output << " Sol_Time= " << setprecision(6) << clock_Run_time << " N_var= " << N_Variables << " N_const= " << N_Constraints;
            long double temp_multiplication=1;
            for(int i=0;i<N_Objectives;i++){
                temp_multiplication*=ObjF[i].getValue();
            }
            Output<< " Value= "<< setprecision(floor(log10(temp_multiplication)) + 7) <<temp_multiplication << endl;
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
