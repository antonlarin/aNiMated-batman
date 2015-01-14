#include "dsparameterset.hpp"

DSParameterSet::DSParameterSet() :
    lambda1(1.0),
    lambda2(1.0),
    k(1.0),
    c(1.0),
    rho(1.0),
    gamma(1.0),
    nu(1.0),
    timeStep(0.00001),
    activatorAccuracy(0.001),
    inhibitorAccuracy(0.001),
    gridDimension(100),
    iterationsLimit(1000),
    activatorInitConditionsCoeffs(),
    inhibitorInitConditionsCoeffs()
{
    activatorInitConditionsCoeffs.push_back(1.0);
    activatorInitConditionsCoeffs.push_back(0.5);

    inhibitorInitConditionsCoeffs.push_back(1.0);
    inhibitorInitConditionsCoeffs.push_back(-0.5);
}

DSParameterSet::~DSParameterSet() {}



/*
 * Properties implementation
 */

double DSParameterSet::GetLambda1() const
{
    return lambda1;
}

bool DSParameterSet::SetLambda1(double value)
{
    if (value > 0)
    {
        lambda1 = value;
        return true;
    }
    else
        return false;
}

double DSParameterSet::GetLambda2() const
{
    return lambda2;
}

bool DSParameterSet::SetLambda2(double value)
{
    if (value > 0)
    {
        lambda2 = value;
        return true;
    }
    else
        return false;
}

double DSParameterSet::GetK() const
{
    return k;
}

bool DSParameterSet::SetK(double value)
{
    if (value >= 0)
    {
        k = value;
        return true;
    }
    else
        return false;
}

double DSParameterSet::GetC() const
{
    return c;
}

bool DSParameterSet::SetC(double value)
{
    if (value >= 0)
    {
        c = value;
        return true;
    }
    else
        return false;
}

double DSParameterSet::GetRho() const
{
    return rho;
}

bool DSParameterSet::SetRho(double value)
{
    if (value >= 0)
    {
        rho = value;
        return true;
    }
    else
        return false;
}

double DSParameterSet::GetGamma() const
{
    return gamma;
}

bool DSParameterSet::SetGamma(double value)
{
    if (value >= 0)
    {
        gamma = value;
        return true;
    }
    else
        return false;
}

double DSParameterSet::GetNu() const
{
    return nu;
}

bool DSParameterSet::SetNu(double value)
{
    if (value >= 0)
    {
        nu = value;
        return true;
    }
    else
        return false;
}

double DSParameterSet::GetTimeStep() const
{
    return timeStep;
}

bool DSParameterSet::SetTimeStep(double value)
{
    if (value > 0)
    {
        timeStep = value;
        return true;
    }
    else
        return false;
}

double DSParameterSet::GetActivatorAccuracy() const
{
    return activatorAccuracy;
}

bool DSParameterSet::SetActivatorAccuracy(double value)
{
    if (value > 0)
    {
        activatorAccuracy = value;
        return true;
    }
    else
        return false;
}

double DSParameterSet::GetInhibitorAccuracy() const
{
    return inhibitorAccuracy;
}

bool DSParameterSet::SetInhibitorAccuracy(double value)
{
    if (value > 0)
    {
        inhibitorAccuracy = value;
        return true;
    }
    else
        return false;
}

int DSParameterSet::GetGridDimension() const
{
    return gridDimension;
}

bool DSParameterSet::SetGridDimension(int value)
{
    if (value > 1)
    {
        gridDimension = value;
        return true;
    }
    else
        return false;
}

int DSParameterSet::GetIterationsLimit() const
{
    return iterationsLimit;
}

bool DSParameterSet::SetIterationsLimit(int value)
{
    if (value > 0)
    {
        iterationsLimit = value;
        return true;
    }
    else
        return false;
}

vector<double> DSParameterSet::GetActivatorInitialConditions() const
{
    return activatorInitConditionsCoeffs;
}

bool DSParameterSet::SetActivatorInitialConditions(vector<double> value)
{
    activatorInitConditionsCoeffs = value;
    // TODO: perhaps perform validity check here
    return true;
}

vector<double> DSParameterSet::GetInhibitorInitialConditions() const
{
    return inhibitorInitConditionsCoeffs;
}

bool DSParameterSet::SetInhibitorInitialConditions(vector<double> value)
{
    inhibitorInitConditionsCoeffs = value;
    // TODO: perhaps perform validity check here
    return true;
}
