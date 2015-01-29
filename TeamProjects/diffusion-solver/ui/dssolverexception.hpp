#ifndef DSSOLVEREXCEPTION_H
#define DSSOLVEREXCEPTION_H

#include <stdexcept>
#include <QException>

class DSSolverException : public QException
{
public:
    DSSolverException();
    DSSolverException(std::exception exceptionSource);
    ~DSSolverException();

    std::exception GetSource() const;

    virtual void raise();

private:

    std::exception mExceptionSource;

};

#endif // DSSOLVEREXCEPTION_H
