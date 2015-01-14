#include "dswindowmanager.hpp"

#include "dsmainwindow.hpp"
#include "dsinitconditionsdialog.hpp"
#include "dssolvingprogressdialog.hpp"
#include "dslayerpairanalysiswindow.hpp"

DSWindowManager::DSWindowManager(DSModel* model) :
    QObject(),
    model(model),
    mainWindow(new DSMainWindow(this)),
    initialConditionsDialog(nullptr),
    solvingProgressDialog(nullptr),
    layerPairAnalysisWindow(nullptr)
{}



/*
 * Slots implementation
 */
void DSWindowManager::showInitialConditionsDialog()
{
    initialConditionsDialog.reset(new DSInitConditionsDialog(this));
    initialConditionsDialog->showWindow();
}

void DSWindowManager::showLayerPairAnalysisWindow()
{
    layerPairAnalysisWindow.reset(new DSLayerPairAnalysisWindow(this));
    layerPairAnalysisWindow->showWindow();
}



/*
 * Other methods implementation
 */
DSModel* DSWindowManager::getModel()
{
    return model;
}

void DSWindowManager::showMainWindow()
{
    mainWindow->showWindow();
}

void DSWindowManager::showSolvingProgressDialog()
{
    solvingProgressDialog.reset(new DSSolvingProgressDialog(this));
    solvingProgressDialog->showWindow();
}
