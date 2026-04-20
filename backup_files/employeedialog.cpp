#include "employeedialog.h"
#include "ui_employeededialog.h"
#include <QDate>

EmployeeDialog::EmployeeDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::EmployeeDialog)
{
    ui->setupUi(this);
    this->setWindowTitle("Employee Information");

    // Set date to today
    ui->joiningDateEdit->setDate(QDate::currentDate());
}

EmployeeDialog::~EmployeeDialog()
{
    delete ui;
}

Employee EmployeeDialog::getEmployee() const
{
    return m_employee;
}

void EmployeeDialog::setEmployee(const Employee& employee)
{
    m_employee = employee;
    ui->nameEdit->setText(employee.getName());
    ui->designationEdit->setText(employee.getDesignation());
    ui->departmentEdit->setText(employee.getDepartment());
    ui->emailEdit->setText(employee.getEmail());
    ui->phoneEdit->setText(employee.getPhone());
    ui->uniformSizeCombo->setCurrentText(employee.getUniformSize());
    ui->joiningDateEdit->setDate(employee.getJoiningDate());
}

void EmployeeDialog::on_okBtn_clicked()
{
    m_employee.setName(ui->nameEdit->text());
    m_employee.setDesignation(ui->designationEdit->text());
    m_employee.setDepartment(ui->departmentEdit->text());
    m_employee.setEmail(ui->emailEdit->text());
    m_employee.setPhone(ui->phoneEdit->text());
    m_employee.setUniformSize(ui->uniformSizeCombo->currentText());
    m_employee.setJoiningDate(ui->joiningDateEdit->date());
    accept();
}

void EmployeeDialog::on_cancelBtn_clicked()
{
    reject();
}
