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
    ui->designationEdit->setText(employee.getDesignation());
    ui->emailEdit->setText(employee.getEmail());
    ui->phoneEdit->setText(employee.getPhone());

    int sizeIndex = ui->uniformSizeCombo->findText(employee.getUniformSize());
    if (sizeIndex >= 0) {
        ui->uniformSizeCombo->setCurrentIndex(sizeIndex);
    } else {
        ui->uniformSizeCombo->setCurrentText(employee.getUniformSize().isEmpty() ? "M" : employee.getUniformSize());
    }
}

void EmployeeDialog::on_okBtn_clicked()
{
    m_employee.setName(ui->nameEdit->text().trimmed());
    m_employee.setDepartment(ui->departmentEdit->text().trimmed());
    m_employee.setDesignation(ui->designationEdit->text().trimmed());
    m_employee.setEmail(ui->emailEdit->text().trimmed());
    m_employee.setPhone(ui->phoneEdit->text().trimmed());
    m_employee.setUniformSize(ui->uniformSizeCombo->currentText());
    accept();
}

void EmployeeDialog::on_cancelBtn_clicked()
{
    reject();
}
