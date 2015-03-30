#ifndef DSWINDOWMANAGER_H
#define DSWINDOWMANAGER_H

#include <memory>
#include <QObject>
#include "idswindow.hpp"
#include "dsmodel.hpp"

using std::unique_ptr;

class DSWindowManager: public QObject
{
    Q_OBJECT

public:
    DSWindowManager(DSModel* model);
    ~DSWindowManager() {}

    DSModel* getModel();
    void showMainWindow();
    void showSolvingProgressDialog();

public slots:
    void showInitialConditionsDialog();
    void showLayerPairAnalysisWindow();
    void showSummaryDialog();
    void showEquationHelpDialog();

private:
    DSModel* model;

    unique_ptr<IDSWindow> mainWindow;
    unique_ptr<IDSWindow> initialConditionsDialog;
    unique_ptr<IDSWindow> solvingProgressDialog;
    unique_ptr<IDSWindow> layerPairAnalysisWindow;
    unique_ptr<IDSWindow> summaryDialog;
    unique_ptr<IDSWindow> equationHelpDialog;
};

#endif // DSWINDOWMANAGER_H
