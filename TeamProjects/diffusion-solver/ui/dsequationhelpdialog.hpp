#ifndef DSEQUATIONHELPDIALOG_H
#define DSEQUATIONHELPDIALOG_H

#include <QDialog>

namespace Ui {
class DSEquationHelpDialog;
}

class DSEquationHelpDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DSEquationHelpDialog(QWidget *parent = 0);
    ~DSEquationHelpDialog();

private:
    Ui::DSEquationHelpDialog *ui;
};

#endif // DSEQUATIONHELPDIALOG_H
