#ifndef DSSUMMARYDIALOG_HPP
#define DSSUMMARYDIALOG_HPP

#include <QDialog>

#include "dswindowmanager.hpp"

namespace Ui {
class DSSummaryDialog;
}

class DSSummaryDialog : public QDialog, public IDSWindow
{
    Q_OBJECT

public:
    explicit DSSummaryDialog(DSWindowManager* manager, QWidget *parent = 0);
    virtual ~DSSummaryDialog();

    virtual void showWindow();

private:
    Ui::DSSummaryDialog *ui;
};

#endif // DSSUMMARYDIALOG_HPP
