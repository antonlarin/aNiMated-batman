#include "dssolverexception.hpp"

DSSolverException::DSSolverException()
{

}

DSSolverException::DSSolverException(std::exception exceptionSource)
{
    mExceptionSource = exceptionSource;
}

DSSolverException::~DSSolverException()
{

}


std::exception DSSolverException::GetSource() const
{
    return mExceptionSource;
}

void DSSolverException::raise()
{
    throw mExceptionSource;
}
