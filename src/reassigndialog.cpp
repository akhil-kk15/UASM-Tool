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
        QString games = emp.getGameKnowledge().isEmpty() ? "Standard" : emp.getGameKnowledge();
        ui->employeeCombo->addItem(
            QString("[%1] %2 (Role: %3 | Team: %4 | Size: %5 | Games: %6)")
                .arg(emp.getId())
                .arg(emp.getName(), emp.getRoleType(), emp.getDepartment(), emp.getUniformSize(), games),
            emp.getId()
        );
    }

    // Populate teams
    QList<Team> teams = m_dbManager->getAllTeams();
    for (const Team& team : teams) {
        ui->targetTeamCombo->addItem(
            QString("%1 [%2 | Games: %3]")
                .arg(team.getName(), team.getTeamType(), team.getUniqueGames().join(", ")),
            team.getName()
        );
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
    QString games = emp.getGameKnowledge().isEmpty() ? "N/A" : emp.getGameKnowledge();
    ui->currentDetailsLabel->setText(
        QString("<b>#%1 - %2</b> | Role: <b>%3</b> | Team: <b>%4</b> | Size: <b>%5</b><br>Game Knowledge: <b>%6</b>")
        .arg(emp.getId())
        .arg(emp.getName(), emp.getRoleType(), emp.getDepartment(), emp.getUniformSize(), games)
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
    QString targetTeamName = ui->targetTeamCombo->currentData().toString();
    if (empId <= 0 || targetTeamName.isEmpty()) return;

    Employee emp = m_dbManager->getEmployee(empId);
    QString size = emp.getUniformSize().isEmpty() ? "M" : emp.getUniformSize();
    Team targetTeam = m_dbManager->getTeamByName(targetTeamName);

    // 1. Role compatibility check
    if (!targetTeam.hasCompatibleRole(emp)) {
        QString note = targetTeam.isGameShowTeam()
            ? "Game Show Host teams require role 'Game Show Host' or 'Both'."
            : "Standard Dealer teams require role 'Dealer' or 'Both'.";
        ui->stockStatusLabel->setText(
            QString("<font color='red'><b>Incompatible Role:</b> %1 (%2) cannot join %3 (%4).<br><i>%5</i></font>")
            .arg(emp.getName(), emp.getRoleType(), targetTeam.getName(), targetTeam.getTeamType(), note)
        );
        ui->reassignBtn->setEnabled(false);
        return;
    }

    // 2. Game Knowledge compatibility check
    if (!targetTeam.hasRequiredGameKnowledge(emp)) {
        QStringList missing = targetTeam.getMissingGames(emp);
        ui->stockStatusLabel->setText(
            QString("<font color='red'><b>Missing Game Knowledge:</b> %1 knows [%2], but %3 requires [%4].<br>"
                    "<i>Missing games: %5</i></font>")
            .arg(emp.getName(), emp.getGameKnowledge(), targetTeam.getName(),
                 targetTeam.getUniqueGames().join(", "), missing.join(", "))
        );
        ui->reassignBtn->setEnabled(false);
        return;
    }

    ui->reassignBtn->setEnabled(true);

    // 3. Uniform stock check
    int count = m_dbManager->getAvailableUniformCountForTeam(targetTeamName, targetTeam.getUniformType(), size);
    if (count > 0) {
        ui->stockStatusLabel->setText(
            QString("<font color='green'><b>Compatible Role &amp; Game Knowledge | Uniform Available (%1 in stock, Size %2)</b></font>")
            .arg(count).arg(size)
        );
    } else {
        ui->stockStatusLabel->setText(
            QString("<font color='red'><b>Out of Stock in Size %1!</b></font><br>"
                    "<i>System will automatically search and redirect to a compatible alternative team (matching role &amp; game knowledge) with available stock.</i>")
            .arg(size)
        );
    }
}

void ReassignDialog::on_reassignBtn_clicked()
{
    int empId = ui->employeeCombo->currentData().toInt();
    QString targetTeam = ui->targetTeamCombo->currentData().toString();
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
