#include "reassigndialog.h"
#include "ui_reassigndialog.h"
#include <QMessageBox>

ReassignDialog::ReassignDialog(DatabaseManager* dbManager, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ReassignDialog)
    , m_dbManager(dbManager)
    , m_scheduler(dbManager)
{
    ui->setupUi(this);

    // Populate employees
    QList<Employee> employees = m_dbManager->getAllEmployees();
    for (const Employee& emp : employees) {
        ui->employeeCombo->addItem(
            QString("%1 (Current: %2 | Size: %3)").arg(emp.getName(), emp.getDepartment(), emp.getUniformSize()),
            emp.getId()
        );
    }

    // Populate teams
    QList<Team> teams = m_dbManager->getAllTeams();
    for (const Team& team : teams) {
        ui->targetTeamCombo->addItem(team.getName(), team.getId());
    }

    connect(ui->employeeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &ReassignDialog::onEmployeeChanged);
    connect(ui->targetTeamCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &ReassignDialog::onTargetTeamChanged);

    if (!employees.isEmpty()) {
        onEmployeeChanged(0);
    }
}

ReassignDialog::~ReassignDialog()
{
    delete ui;
}

ReassignmentResult ReassignDialog::getResult() const
{
    return m_result;
}

void ReassignDialog::onEmployeeChanged(int index)
{
    Q_UNUSED(index);
    int empId = ui->employeeCombo->currentData().toInt();
    if (empId <= 0) return;

    Employee emp = m_dbManager->getEmployee(empId);
    ui->currentDetailsLabel->setText(
        QString("<b>%1</b> | Team: <b>%2</b> | Uniform Size: <b>%3</b>")
        .arg(emp.getName(), emp.getDepartment(), emp.getUniformSize())
    );

    updateStockStatus();
}

void ReassignDialog::onTargetTeamChanged(int index)
{
    Q_UNUSED(index);
    updateStockStatus();
}

void ReassignDialog::updateStockStatus()
{
    int empId = ui->employeeCombo->currentData().toInt();
    QString targetTeamName = ui->targetTeamCombo->currentText();
    if (empId <= 0 || targetTeamName.isEmpty()) return;

    Employee emp = m_dbManager->getEmployee(empId);
    QString size = emp.getUniformSize().isEmpty() ? "M" : emp.getUniformSize();
    Team targetTeam = m_dbManager->getTeamByName(targetTeamName);

    int count = m_dbManager->getAvailableUniformCountForTeam(targetTeamName, targetTeam.getUniformType(), size);
    if (count > 0) {
        ui->stockStatusLabel->setText(
            QString("<font color='green'><b>Available (%1 in stock)</b></font> - Uniform matches Size %2")
            .arg(count).arg(size)
        );
    } else {
        ui->stockStatusLabel->setText(
            QString("<font color='red'><b>Out of Stock in Size %1!</b></font><br>"
                    "<i>System will automatically search and redirect to an alternative team with available stock.</i>")
            .arg(size)
        );
    }
}

void ReassignDialog::on_reassignBtn_clicked()
{
    int empId = ui->employeeCombo->currentData().toInt();
    QString targetTeam = ui->targetTeamCombo->currentText();
    QString reason = ui->reasonEdit->text().trimmed();

    if (empId <= 0) {
        QMessageBox::warning(this, "Warning", "Please select an employee.");
        return;
    }

    if (targetTeam.isEmpty()) {
        QMessageBox::warning(this, "Warning", "Please select a target team.");
        return;
    }

    m_result = m_scheduler.reassignEmployee(empId, targetTeam, reason);

    if (m_result.success) {
        if (m_result.fallbackUsed) {
            QMessageBox::warning(
                this, "Uniform Shortage - Reassigned to Alternative Team",
                m_result.message
            );
        } else {
            QMessageBox::information(
                this, "Reassignment Successful",
                m_result.message
            );
        }
        accept();
    } else {
        QMessageBox::critical(
            this, "Reassignment Failed",
            m_result.message
        );
    }
}

void ReassignDialog::on_cancelBtn_clicked()
{
    reject();
}
