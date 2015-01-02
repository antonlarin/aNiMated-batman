#ifndef DSLAYERPAIRANALYSISWINDOW_HPP
#define DSLAYERPAIRANALYSISWINDOW_HPP

#include <QDialog>

#include "dswindowmanager.hpp"

namespace Ui {
class DSLayerPairAnalysisWindow;
}

class DSLayerPairAnalysisWindow : public QDialog, public IDSWindow
{
    Q_OBJECT

public:
    explicit DSLayerPairAnalysisWindow(DSWindowManager* manager,
                                       QWidget *parent = 0);
    virtual ~DSLayerPairAnalysisWindow();

    virtual void showWindow();

public slots:
    void firstLayerIndexChanged(const QString& newIndex);
    void secondLayerIndexChanged(const QString& newIndex);
    void updateComparisonInfo();
    void updateComparedLayerEditsText();

private:
    static double GetMaxPlotPointsCount() { return 100; }

    void initPlots();

    Ui::DSLayerPairAnalysisWindow *ui;
};

#endif // DSLAYERPAIRANALYSISWINDOW_HPP
