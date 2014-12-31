#ifndef DSSOLVERITERATIONINFO_H
#define DSSOLVERITERATIONINFO_H

#include <diffusioncore>
using namespace diffusioncore;

class DSSolverIterationInfo final
{
public:
    DSSolverIterationInfo();
    DSSolverIterationInfo(SchemeSolverIterationInfo&);
    ~DSSolverIterationInfo();

    int GetCurrentIterationNumber() const;
    int GetPlannedIterationsCount() const;
    double GetActivatorCurrentAccuracy() const;
    double GetInhibitorCurrentAccuracy() const;

private:
    SchemeSolverIterationInfo internalInfo;
};

#endif // DSSOLVERITERATIONINFO_H
