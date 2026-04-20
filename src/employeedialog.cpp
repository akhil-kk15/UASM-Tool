#include "employeedialog.h"
#include "ui_employeededialog.h"

EmployeeDialog::EmployeeDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::EmployeeDialog)
{
    ui->setupUi(this);
    this->setWindowTitle("Employee Information");
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
    ui->departmentEdit->setText(employee.getDepartment());
}

void EmployeeDialog::on_okBtn_clicked()
{
    m_employee.setName(ui->nameEdit->text());
    m_employee.setDepartment(ui->departmentEdit->text());
    accept();
}

void EmployeeDialog::on_cancelBtn_clicked()
{
    reject();
}
