#ifndef ISSUEDIALOG_H
#define ISSUEDIALOG_H

#include <QDialog>
#include <QList>
#include "uniformissue.h"
#include "employee.h"
#include "uniform.h"

namespace Ui {
class IssueDialog;
}

class IssueDialog : public QDialog
{
    Q_OBJECT

public:
    explicit IssueDialog(QWidget *parent = nullptr);
    ~IssueDialog();

    UniformIssue getIssueData() const;
    void setEmployeeList(const QList<Employee>& employees);
    void setUniformList(const QList<Uniform>& uniforms);

private slots:
    void on_okBtn_clicked();
    void on_cancelBtn_clicked();

private:
    Ui::IssueDialog *ui;
    UniformIssue m_issue;
    QList<Employee> m_employees;
    QList<Uniform> m_uniforms;
};

#endif // ISSUEDIALOG_H
