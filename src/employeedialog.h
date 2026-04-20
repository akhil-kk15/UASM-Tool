#ifndef EMPLOYEEDIALOG_H
#define EMPLOYEEDIALOG_H

#include <QDialog>
#include "employee.h"

namespace Ui {
class EmployeeDialog;
}

class EmployeeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EmployeeDialog(QWidget *parent = nullptr);
    ~EmployeeDialog();

    Employee getEmployee() const;
    void setEmployee(const Employee& employee);

private slots:
    void on_okBtn_clicked();
    void on_cancelBtn_clicked();

private:
    Ui::EmployeeDialog *ui;
    Employee m_employee;
};

#endif // EMPLOYEEDIALOG_H
