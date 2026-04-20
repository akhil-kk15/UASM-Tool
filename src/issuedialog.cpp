#include "issuedialog.h"
#include "ui_issuedialog.h"
#include <QDate>

IssueDialog::IssueDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::IssueDialog)
{
    ui->setupUi(this);
    this->setWindowTitle("Issue/Return Uniform");
    ui->issueDateEdit->setDate(QDate::currentDate());
}

IssueDialog::~IssueDialog()
{
    delete ui;
}

UniformIssue IssueDialog::getIssueData() const
{
    return m_issue;
}

void IssueDialog::setEmployeeList(const QList<Employee>& employees)
{
    m_employees = employees;
    ui->employeeCombo->clear();
    for (const Employee& emp : employees) {
        ui->employeeCombo->addItem(emp.getName(), emp.getId());
    }
}

void IssueDialog::setUniformList(const QList<Uniform>& uniforms)
{
    m_uniforms = uniforms;
    ui->uniformCombo->clear();
    for (const Uniform& uni : uniforms) {
        ui->uniformCombo->addItem(uni.getType(), uni.getId());
    }
}

void IssueDialog::on_okBtn_clicked()
{
    m_issue.setEmployeeId(ui->employeeCombo->currentData().toInt());
    m_issue.setUniformId(ui->uniformCombo->currentData().toInt());
    m_issue.setQuantity(ui->quantitySpinBox->value());
    m_issue.setIssueDate(ui->issueDateEdit->date());
    m_issue.setStatus("Issued");
    accept();
}

void IssueDialog::on_cancelBtn_clicked()
{
    reject();
}
