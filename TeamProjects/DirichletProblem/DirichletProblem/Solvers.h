#ifndef SOLVERS_H
#define SOLVERS_H
#include "D_TestTask.h"

void SolveWithZeidel(D_TestTask* task, int Nmax, double eps, double& reachedEps, int &stepNum, D_TaskStatus& stat);
void SolveWithJacoby(D_TestTask* task, int Nmax, double eps, double& reachedEps, int &stepNum, D_TaskStatus& stat);
void SolveWithURelax(D_TestTask* task, int Nmax, double eps, double& reachedEps, int &stepNum, double omega, D_TaskStatus& stat);

#endif