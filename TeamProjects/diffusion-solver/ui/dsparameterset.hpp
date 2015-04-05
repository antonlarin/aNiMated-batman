#ifndef DSPARAMETERSET_H
#define DSPARAMETERSET_H

#include <vector>
#include <diffusioncore>

using std::vector;
using diffusioncore::SchemeSolverMode;

#define VALIDATED_PROPERTY(T, N) \
    T Get ## N() const;          \
    bool Set ## N(T value);

class DSParameterSet
{
public:
    DSParameterSet();
    ~DSParameterSet();

    VALIDATED_PROPERTY(double, Lambda1)
    VALIDATED_PROPERTY(double, Lambda2)
    VALIDATED_PROPERTY(double, K)
    VALIDATED_PROPERTY(double, C)
    VALIDATED_PROPERTY(double, Rho)
    VALIDATED_PROPERTY(double, Gamma)
    VALIDATED_PROPERTY(double, Nu)
    VALIDATED_PROPERTY(double, TimeStep)
    VALIDATED_PROPERTY(double, ActivatorAccuracy)
    VALIDATED_PROPERTY(double, InhibitorAccuracy)
    VALIDATED_PROPERTY(int, GridDimension)
    VALIDATED_PROPERTY(int, IterationsLimit)
    VALIDATED_PROPERTY(vector<double>, ActivatorInitialConditions)
    VALIDATED_PROPERTY(vector<double>, InhibitorInitialConditions)
    VALIDATED_PROPERTY(SchemeSolverMode, SolvingMode)
    VALIDATED_PROPERTY(int, LayerSavingStep)

private:
    double lambda1;
    double lambda2;
    double k;
    double c;
    double rho;
    double gamma;
    double nu;
    double timeStep;
    double activatorAccuracy;
    double inhibitorAccuracy;
    int gridDimension;
    int iterationsLimit;
    vector<double> activatorInitConditionsCoeffs;
    vector<double> inhibitorInitConditionsCoeffs;
    SchemeSolverMode solvingMode;
    int layerSavingStep;
};

#endif // DSPARAMETERSET_H
