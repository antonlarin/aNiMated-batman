#ifndef DSVIEWMODEL_HPP
#define DSVIEWMODEL_HPP

#include <vector>
#include <memory>

#include "CoreGlobal.hpp"
#include "ExplicitSchemeSolver.hpp"
#include "DefaultSchemeInitialConditions.hpp"

using std::unique_ptr;
using std::vector;
using namespace diffusioncore;

class DSModel
{
public:
    explicit DSModel();

    PROPERTY(double, Lambda1)
    PROPERTY(double, Lambda2)
    PROPERTY(double, K)
    PROPERTY(double, C)
    PROPERTY(double, Rho)
    PROPERTY(double, Gamma)
    PROPERTY(double, Nu)
    PROPERTY(double, TimeStep)
    PROPERTY(double, Accuracy)
    PROPERTY(int, GridDimension)
    PROPERTY(int, IterationsLimit)

    void SetInitialConditions(vector<double>& U1InitConditions,
                              vector<double>& U2InitConditions);

    void StartFiniteRun();
    void AcquireResult(SchemeResult& result);

private:
    double lambda1;
    double lambda2;
    double k;
    double c;
    double rho;
    double gamma;
    double nu;
    double timeStep;
    double accuracy;
    int gridDimension;
    int iterationsLimit;

    ExplicitSchemeSolver solver;
    unique_ptr<DefaultSchemeInitialConditions> iconditions;
    unique_ptr<SchemeResult> result;
};

#endif // DSVIEWMODEL_HPP

