#ifndef DSSOLVINGPROGRESSDIALOG_HPP
#define DSSOLVINGPROGRESSDIALOG_HPP

#include <QDialog>

#include "dswindowmanager.hpp"

namespace Ui {
class DSSolvingProgressDialog;
}

class DSSolvingProgressDialog : public QDialog, public IDSWindow
{
    Q_OBJECT

public:
    explicit DSSolvingProgressDialog(DSWindowManager* manager, QWidget *parent = 0);
    virtual ~DSSolvingProgressDialog();

    virtual void showWindow();

public slots:
    void updateResult(const SchemeSolverIterationInfo&);

private:
    Ui::DSSolvingProgressDialog *ui;
};

#endif // DSSOLVINGPROGRESSDIALOG_HPP
