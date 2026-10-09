#ifndef REASSIGNDIALOG_H
#define REASSIGNDIALOG_H

#include <QDialog>
#include "databasemanager.h"
#include "rotationscheduler.h"

namespace Ui {
class ReassignDialog;
}

class ReassignDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ReassignDialog(DatabaseManager* dbManager, QWidget *parent = nullptr);
    ~ReassignDialog();

    ReassignmentResult getResult() const;

private slots:
    void onEmployeeChanged(int index);
    void onTargetTeamChanged(int index);
    void on_reassignBtn_clicked();
    void on_cancelBtn_clicked();

private:
    Ui::ReassignDialog *ui;
    DatabaseManager* m_dbManager;
    RotationScheduler m_scheduler;
    ReassignmentResult m_result;

    void updateStockStatus();
};

#endif // REASSIGNDIALOG_H
