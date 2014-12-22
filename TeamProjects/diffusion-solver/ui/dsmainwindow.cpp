#include "dsmainwindow.hpp"
#include "ui_dsmainwindow.h"

#include <iostream>

#include "SchemeSolver.hpp"

DSMainWindow::DSMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::DSMainWindow)
{
    ui->setupUi(this);

    connect(ui->lambda1Edit, SIGNAL(textEdited(QString)),
            this, SLOT(lambda1CoeffChanged(QString)));
    connect(ui->lambda2Edit, SIGNAL(textEdited(QString)),
            this, SLOT(lambda2CoeffChanged(QString)));
    connect(ui->kEdit, SIGNAL(textEdited(QString)),
            this, SLOT(kCoeffChanged(QString)));
    connect(ui->cEdit, SIGNAL(textEdited(QString)),
            this, SLOT(cCoeffChanged(QString)));
    connect(ui->rhoEdit, SIGNAL(textEdited(QString)),
            this, SLOT(rhoCoeffChanged(QString)));
    connect(ui->nuEdit, SIGNAL(textEdited(QString)),
            this, SLOT(nuCoeffChanged(QString)));
    connect(ui->gammaEdit, SIGNAL(textEdited(QString)),
            this, SLOT(gammaCoeffChanged(QString)));
    connect(ui->gridDimEdit, SIGNAL(textEdited(QString)),
            this, SLOT(gridDimensionChanged(QString)));
    connect(ui->timeStepEdit, SIGNAL(textEdited(QString)),
            this, SLOT(timeStepChanged(QString)));
    connect(ui->accuracyEdit, SIGNAL(textEdited(QString)),
            this, SLOT(accuracyChanged(QString)));
    connect(ui->iterationsEdit, SIGNAL(textEdited(QString)),
            this, SLOT(iterationsLimitChanged(QString)));
}

DSMainWindow::~DSMainWindow()
{
    delete ui;
}

/*
 * Slots implementations
 */
void DSMainWindow::lambda1CoeffChanged(const QString& newLambda1)
{
    std::cout << newLambda1.toStdString() << '\n';
}

void DSMainWindow::lambda2CoeffChanged(const QString& newLambda2)
{
    std::cout << newLambda2.toStdString() << '\n';
}

void DSMainWindow::kCoeffChanged(const QString& newK)
{

}

void DSMainWindow::cCoeffChanged(const QString& newC)
{

}

void DSMainWindow::rhoCoeffChanged(const QString& newRho)
{

}

void DSMainWindow::nuCoeffChanged(const QString& newNu)
{

}

void DSMainWindow::gammaCoeffChanged(const QString& newGamma)
{

}

void DSMainWindow::gridDimensionChanged(const QString& newGridDimension)
{

}

void DSMainWindow::timeStepChanged(const QString& newTimeStep)
{

}

void DSMainWindow::accuracyChanged(const QString& newAccuracy)
{

}

void DSMainWindow::iterationsLimitChanged(const QString& newIterationsLimit)
{

}
