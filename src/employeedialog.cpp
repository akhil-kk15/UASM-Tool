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

void EmployeeDialog::setTeamList(const QStringList& teams)
{
    QString current = ui->teamCombo->currentText();
    ui->teamCombo->clear();
    for (const QString& t : teams) {
        if (!t.trimmed().isEmpty()) {
            ui->teamCombo->addItem(t.trimmed());
        }
    }
    if (!current.isEmpty()) {
        ui->teamCombo->setCurrentText(current);
    }
}

void EmployeeDialog::setEmployee(const Employee& employee)
{
    m_employee = employee;
    ui->nameEdit->setText(employee.getName());
    ui->teamCombo->setCurrentText(employee.getTeam());
    ui->designationEdit->setText(employee.getDesignation());
    ui->emailEdit->setText(employee.getEmail());
    ui->phoneEdit->setText(employee.getPhone());

    int sizeIndex = ui->uniformSizeCombo->findText(employee.getUniformSize());
    if (sizeIndex >= 0) {
        ui->uniformSizeCombo->setCurrentIndex(sizeIndex);
    } else {
        ui->uniformSizeCombo->setCurrentText(employee.getUniformSize().isEmpty() ? "M" : employee.getUniformSize());
    }

    QString role = employee.getRoleType();
    int roleIndex = ui->roleCombo->findText(role);
    if (roleIndex >= 0) {
        ui->roleCombo->setCurrentIndex(roleIndex);
    } else {
        ui->roleCombo->setCurrentText(role.isEmpty() ? "Dealer" : role);
    }
}

void EmployeeDialog::on_okBtn_clicked()
{
    m_employee.setName(ui->nameEdit->text().trimmed());
    m_employee.setTeam(ui->teamCombo->currentText().trimmed());
    m_employee.setRoleType(ui->roleCombo->currentText());
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
