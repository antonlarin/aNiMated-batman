#ifndef DSLAYERPAIRANALYSISWINDOW_HPP
#define DSLAYERPAIRANALYSISWINDOW_HPP

#include <QDialog>

#include "dsmodel.hpp"

namespace Ui {
class DSLayerPairAnalysisWindow;
}

class DSLayerPairAnalysisWindow : public QDialog
{
    Q_OBJECT

public:
    explicit DSLayerPairAnalysisWindow(DSModel* model, QWidget *parent = 0);
    ~DSLayerPairAnalysisWindow();

public slots:
    void firstLayerIndexChanged(const QString& newIndex);
    void secondLayerIndexChanged(const QString& newIndex);
    void updateComparisonInfo();

private:
    static double GetMaxPlotPointsCount() { return 100; }

    void initPlots();

    Ui::DSLayerPairAnalysisWindow *ui;
    DSModel* model;
};

#endif // DSLAYERPAIRANALYSISWINDOW_HPP
