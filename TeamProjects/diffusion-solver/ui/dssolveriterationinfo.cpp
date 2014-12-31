#include "dssolveriterationinfo.hpp"

DSSolverIterationInfo::DSSolverIterationInfo()
{
}

DSSolverIterationInfo::DSSolverIterationInfo(
        SchemeSolverIterationInfo& info) :
    internalInfo(info)
{
}

DSSolverIterationInfo::~DSSolverIterationInfo()
{
}


int DSSolverIterationInfo::GetCurrentIterationNumber() const
{
    return internalInfo.GetIterationNumber();
}

int DSSolverIterationInfo::GetPlannedIterationsCount() const
{
    return internalInfo.GetTotalIterations();
}

double DSSolverIterationInfo::GetActivatorCurrentAccuracy() const
{
    return internalInfo.GetAccuracyU1();
}

double DSSolverIterationInfo::GetInhibitorCurrentAccuracy() const
{
    return internalInfo.GetAccuracyU2();
}

