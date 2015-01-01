#include "dslayerpairanalysiswindow.hpp"
#include "ui_dslayerpairanalysiswindow.h"

DSLayerPairAnalysisWindow::DSLayerPairAnalysisWindow(DSModel* model, QWidget* parent) :
    QDialog(parent),
    ui(new Ui::DSLayerPairAnalysisWindow),
    model(model)
{
    ui->setupUi(this);
}

DSLayerPairAnalysisWindow::~DSLayerPairAnalysisWindow()
{
    delete ui;
}
