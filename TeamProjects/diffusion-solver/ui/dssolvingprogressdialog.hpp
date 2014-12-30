#ifndef DSSOLVINGPROGRESSDIALOG_HPP
#define DSSOLVINGPROGRESSDIALOG_HPP

#include <QDialog>

#include "dsmodel.hpp"

namespace Ui {
class DSSolvingProgressDialog;
}

class DSSolvingProgressDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DSSolvingProgressDialog(DSModel* model, QWidget *parent = 0);
    ~DSSolvingProgressDialog();

private:
    Ui::DSSolvingProgressDialog *ui;

    DSModel* model;

};

#endif // DSSOLVINGPROGRESSDIALOG_HPP
