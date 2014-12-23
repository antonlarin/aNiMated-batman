#include "dsmodel.hpp"

#include <functional>
#include <iostream>

using namespace std::placeholders;

DSModel::DSModel() :
    lambda1(1.0),
    lambda2(1.0),
    k(1.0),
    c(1.0),
    rho(1.0),
    gamma(1.0),
    nu(1.0),
    timeStep(0.00001),
    accuracy(0.001),
    gridDimension(100),
    iterationsLimit(1000),
    solver(),
    iconditions(nullptr),
    result(nullptr)
{}

double DSModel::GetLambda1() const
{
    return lambda1;
}

void DSModel::SetLambda1(double value)
{
    lambda1 = value;
}

double DSModel::GetLambda2() const
{
    return lambda2;
}

void DSModel::SetLambda2(double value)
{
    lambda2 = value;
}

double DSModel::GetK() const
{
    return k;
}

void DSModel::SetK(double value)
{
    k = value;
}

double DSModel::GetC() const
{
    return c;
}

void DSModel::SetC(double value)
{
    c = value;
}

double DSModel::GetRho() const
{
    return rho;
}

void DSModel::SetRho(double value)
{
    rho = value;
}

double DSModel::GetGamma() const
{
    return gamma;
}

void DSModel::SetGamma(double value)
{
    gamma = value;
}

double DSModel::GetNu() const
{
    return nu;
}

void DSModel::SetNu(double value)
{
    nu = value;
}

double DSModel::GetTimeStep() const
{
    return timeStep;
}

void DSModel::SetTimeStep(double value)
{
    timeStep = value;
}

double DSModel::GetAccuracy() const
{
    return accuracy;
}

void DSModel::SetAccuracy(double value)
{
    accuracy = value;
}

int DSModel::GetGridDimension() const
{
    return gridDimension;
}

void DSModel::SetGridDimension(int value)
{
    gridDimension = value;
}

int DSModel::GetIterationsLimit() const
{
    return iterationsLimit;
}

void DSModel::SetIterationsLimit(int value)
{
    iterationsLimit = value;
}

void DSModel::SetInitialConditions(vector<double>& U1InitConditions,
                                   vector<double>& U2InitConditions)
{
    iconditions.reset(new DefaultSchemeInitialConditions(U1InitConditions,
                                                         U2InitConditions));
}

void DSModel::StartFiniteRun()
{
    solver.SetLambda1(GetLambda1());
    solver.SetLambda2(GetLambda2());
    solver.SetK(GetK());
    solver.SetC(GetC());
    solver.SetRho(GetRho());
    //solver.SetGamma(GetGamma());
    solver.SetNu(GetNu());
    solver.SetStepTime(GetTimeStep());
    solver.SetAccuracy(GetAccuracy());
    solver.SetIntervalsCount(GetGridDimension());
    solver.SetMaximumIterations(GetIterationsLimit());
    solver.SetInitialConditions(iconditions.get());
    solver.SetSolvingMode(AllLayers);

    std::function<void(SchemeResult&)> acquireResult =
            std::bind(&DSModel::AcquireResult, this, _1);
    solver.BeginSolve(acquireResult);
}

void DSModel::AcquireResult(SchemeResult &result)
{

    this->result.reset(new SchemeResult(result));
    std::cout << this->result->GetSolutionU1(1).Get(1) << '\n';
}
