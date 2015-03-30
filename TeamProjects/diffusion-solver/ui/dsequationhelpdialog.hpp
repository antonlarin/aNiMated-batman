#ifndef DSEQUATIONHELPDIALOG_H
#define DSEQUATIONHELPDIALOG_H

#include <QDialog>

#include "idswindow.hpp"
#include "dswindowmanager.hpp"

namespace Ui {
class DSEquationHelpDialog;
}

class DSEquationHelpDialog : public QDialog, public IDSWindow
{
    Q_OBJECT

public:
    explicit DSEquationHelpDialog(DSWindowManager* manager,
                                  QWidget* parent = nullptr);
    ~DSEquationHelpDialog();

    virtual void showWindow() override;

private:
    Ui::DSEquationHelpDialog *ui;
};

#endif // DSEQUATIONHELPDIALOG_H
