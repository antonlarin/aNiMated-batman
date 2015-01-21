#include "dswindowmanager.hpp"

#include "dsmainwindow.hpp"
#include "dsinitconditionsdialog.hpp"
#include "dssolvingprogressdialog.hpp"
#include "dslayerpairanalysiswindow.hpp"
#include "dssummarydialog.hpp"

DSWindowManager::DSWindowManager(DSModel* model) :
    QObject(),
    model(model),
    mainWindow(new DSMainWindow(this)),
    initialConditionsDialog(nullptr),
    solvingProgressDialog(nullptr),
    layerPairAnalysisWindow(nullptr),
    summaryDialog(nullptr)
{
    connect(getModel(), SIGNAL(resultAcquired()),
            this, SLOT(showSummaryDialog()));
}



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

void DSWindowManager::showSummaryDialog()
{
    summaryDialog.reset(new DSSummaryDialog(this));
    summaryDialog->showWindow();
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
