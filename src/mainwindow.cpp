#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "employeedialog.h"
#include "uniformdialog.h"
#include "issuedialog.h"
#include "reassigndialog.h"
#include <QMessageBox>
#include <QTableWidgetItem>
#include <QDate>
#include <QTime>
#include <QBrush>
#include <QColor>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_scheduler(&m_db)
{
    ui->setupUi(this);
    initializeDatabase();

    // Configure rotation defaults
    ui->rotationDateEdit->setDate(QDate::currentDate());
    ui->rotationStartTimeEdit->setTime(QTime(8, 0));
    ui->rotationSlotCombo->setCurrentIndex(0); // 30 mins

    loadEmployeesTable();
    loadUniformsTable();
    loadIssuesTable();
    loadTeamsTable();
    populateRotationTeamCombo();

    // Auto-generate rotation for first team if available
    if (ui->rotationTeamCombo->count() > 0) {
        on_generateRotationBtn_clicked();
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initializeDatabase()
{
    if (!m_db.initializeDatabase()) {
        QMessageBox::critical(this, "Database Error", 
                            "Failed to initialize database: " + m_db.getLastError());
    }
}

void MainWindow::loadEmployeesTable()
{
    ui->employeesTable->setRowCount(0);
    ui->employeesTable->setColumnCount(7);
    QStringList headers;
    headers << "ID" << "Name" << "Role Type" << "Designation" << "Team" << "Uniform Size" << "Issued Uniforms";
    ui->employeesTable->setHorizontalHeaderLabels(headers);
    
    QList<Employee> employees = m_db.getAllEmployees();
    
    for (int i = 0; i < employees.size(); ++i) {
        ui->employeesTable->insertRow(i);
        ui->employeesTable->setItem(i, 0, new QTableWidgetItem(QString::number(employees[i].getId())));
        ui->employeesTable->setItem(i, 1, new QTableWidgetItem(employees[i].getName()));
        ui->employeesTable->setItem(i, 2, new QTableWidgetItem(employees[i].getRoleType()));
        ui->employeesTable->setItem(i, 3, new QTableWidgetItem(employees[i].getDesignation()));
        ui->employeesTable->setItem(i, 4, new QTableWidgetItem(employees[i].getDepartment()));
        ui->employeesTable->setItem(i, 5, new QTableWidgetItem(employees[i].getUniformSize()));
        
        // Issued uniforms count
        QList<UniformIssue> issues = m_db.getIssueHistory(employees[i].getId());
        int issuedCount = 0;
        for (const auto& issue : issues) {
            if (issue.getStatus() == "Issued") {
                issuedCount += issue.getQuantity();
            }
        }
        ui->employeesTable->setItem(i, 6, new QTableWidgetItem(QString::number(issuedCount)));
    }
}

void MainWindow::loadUniformsTable()
{
    ui->uniformsTable->setRowCount(0);
    ui->uniformsTable->setColumnCount(8);
    QStringList headers;
    headers << "ID" << "Type" << "Team / Dept" << "Size" << "Total Stock" << "Extra Available" << "Color" << "Description";
    ui->uniformsTable->setHorizontalHeaderLabels(headers);
    
    QList<Uniform> uniforms = m_db.getAllUniforms();
    
    for (int i = 0; i < uniforms.size(); ++i) {
        ui->uniformsTable->insertRow(i);
        ui->uniformsTable->setItem(i, 0, new QTableWidgetItem(QString::number(uniforms[i].getId())));
        ui->uniformsTable->setItem(i, 1, new QTableWidgetItem(uniforms[i].getType()));
        ui->uniformsTable->setItem(i, 2, new QTableWidgetItem(uniforms[i].getTeam().isEmpty() ? "General" : uniforms[i].getTeam()));
        ui->uniformsTable->setItem(i, 3, new QTableWidgetItem(uniforms[i].getSize()));
        ui->uniformsTable->setItem(i, 4, new QTableWidgetItem(QString::number(uniforms[i].getQuantity())));
        
        int extraAvail = m_db.getAvailableStockForUniform(uniforms[i].getId());
        QTableWidgetItem* availItem = new QTableWidgetItem(QString::number(extraAvail));
        if (extraAvail > 0) {
            availItem->setBackground(QColor("#d4edda"));
            availItem->setForeground(QColor("#155724"));
        } else {
            availItem->setBackground(QColor("#f8d7da"));
            availItem->setForeground(QColor("#721c24"));
        }
        ui->uniformsTable->setItem(i, 5, availItem);
        ui->uniformsTable->setItem(i, 6, new QTableWidgetItem(uniforms[i].getColor()));
        ui->uniformsTable->setItem(i, 7, new QTableWidgetItem(uniforms[i].getDescription()));
    }
}

void MainWindow::loadIssuesTable()
{
    ui->issuesTable->setRowCount(0);
    QList<UniformIssue> issues = m_db.getAllIssues();
    
    for (int i = 0; i < issues.size(); ++i) {
        Employee emp = m_db.getEmployee(issues[i].getEmployeeId());
        Uniform uniform = m_db.getUniform(issues[i].getUniformId());
        
        ui->issuesTable->insertRow(i);
        ui->issuesTable->setItem(i, 0, new QTableWidgetItem(QString::number(issues[i].getId())));
        ui->issuesTable->setItem(i, 1, new QTableWidgetItem(emp.getName()));
        ui->issuesTable->setItem(i, 2, new QTableWidgetItem(uniform.getType()));
        ui->issuesTable->setItem(i, 3, new QTableWidgetItem(QString::number(issues[i].getQuantity())));
        ui->issuesTable->setItem(i, 4, new QTableWidgetItem(issues[i].getIssueDate().toString("yyyy-MM-dd")));
        ui->issuesTable->setItem(i, 5, new QTableWidgetItem(issues[i].getReturnDate().toString("yyyy-MM-dd")));
        ui->issuesTable->setItem(i, 6, new QTableWidgetItem(issues[i].getStatus()));
    }
}

void MainWindow::loadTeamsTable()
{
    ui->teamsTable->setRowCount(0);
    ui->teamsTable->setColumnCount(8);
    QStringList headers;
    headers << "ID" << "Team Name" << "Team Type" << "Uniform Type" << "Current Staff" << "Target Staff" << "Staffing Status" << "Assigned Tables";
    ui->teamsTable->setHorizontalHeaderLabels(headers);

    QList<Team> teams = m_db.getAllTeams();

    for (int i = 0; i < teams.size(); ++i) {
        int currentStaff = m_db.getTeamMemberCount(teams[i].getName());
        ui->teamsTable->insertRow(i);
        ui->teamsTable->setItem(i, 0, new QTableWidgetItem(QString::number(teams[i].getId())));
        ui->teamsTable->setItem(i, 1, new QTableWidgetItem(teams[i].getName()));
        ui->teamsTable->setItem(i, 2, new QTableWidgetItem(teams[i].getTeamType()));
        ui->teamsTable->setItem(i, 3, new QTableWidgetItem(teams[i].getUniformType()));
        ui->teamsTable->setItem(i, 4, new QTableWidgetItem(QString::number(currentStaff)));
        ui->teamsTable->setItem(i, 5, new QTableWidgetItem(QString::number(teams[i].getTargetStaff())));

        QString statusStr = teams[i].getStaffingStatusString(currentStaff);
        QTableWidgetItem* statusItem = new QTableWidgetItem(statusStr);

        Team::StaffingStatus status = teams[i].evaluateStaffing(currentStaff);
        if (status == Team::Optimal) {
            statusItem->setBackground(QColor("#d4edda"));
            statusItem->setForeground(QColor("#155724"));
        } else if (status == Team::CriticallyUnderstaffed || status == Team::Understaffed) {
            statusItem->setBackground(QColor("#f8d7da"));
            statusItem->setForeground(QColor("#721c24"));
        } else {
            statusItem->setBackground(QColor("#cce5ff"));
            statusItem->setForeground(QColor("#004085"));
        }
        ui->teamsTable->setItem(i, 6, statusItem);
        ui->teamsTable->setItem(i, 7, new QTableWidgetItem("4 Tables (Tables 1 - 4)"));
    }

    if (teams.size() > 0) {
        ui->teamsTable->selectRow(0);
        loadTeamMembersTable(teams[0].getName());
    }
}

void MainWindow::loadTeamMembersTable(const QString& teamName)
{
    ui->teamMembersTable->setRowCount(0);
    ui->teamMembersTable->setColumnCount(5);
    QStringList headers;
    headers << "ID" << "Name" << "Designation" << "Uniform Size" << "Issued Uniforms";
    ui->teamMembersTable->setHorizontalHeaderLabels(headers);

    QList<Employee> members = m_db.getEmployeesByTeam(teamName);
    for (int i = 0; i < members.size(); ++i) {
        ui->teamMembersTable->insertRow(i);
        ui->teamMembersTable->setItem(i, 0, new QTableWidgetItem(QString::number(members[i].getId())));
        ui->teamMembersTable->setItem(i, 1, new QTableWidgetItem(members[i].getName()));
        ui->teamMembersTable->setItem(i, 2, new QTableWidgetItem(members[i].getDesignation()));
        ui->teamMembersTable->setItem(i, 3, new QTableWidgetItem(members[i].getUniformSize()));

        QList<UniformIssue> issues = m_db.getIssueHistory(members[i].getId());
        int count = 0;
        for (const auto& issue : issues) {
            if (issue.getStatus() == "Issued") count += issue.getQuantity();
        }
        ui->teamMembersTable->setItem(i, 4, new QTableWidgetItem(QString::number(count)));
    }
}

void MainWindow::on_teamsTable_itemSelectionChanged()
{
    int row = ui->teamsTable->currentRow();
    if (row >= 0 && ui->teamsTable->item(row, 1)) {
        QString teamName = ui->teamsTable->item(row, 1)->text();
        loadTeamMembersTable(teamName);
    }
}

void MainWindow::populateRotationTeamCombo()
{
    ui->rotationTeamCombo->clear();
    QList<Team> teams = m_db.getAllTeams();
    for (const Team& team : teams) {
        ui->rotationTeamCombo->addItem(team.getName(), team.getId());
    }
}

void MainWindow::on_generateRotationBtn_clicked()
{
    QString teamName = ui->rotationTeamCombo->currentText();
    if (teamName.isEmpty()) return;

    QDate date = ui->rotationDateEdit->date();
    QTime startTime = ui->rotationStartTimeEdit->time();
    int comboIndex = ui->rotationSlotCombo->currentIndex();
    QString comboText = ui->rotationSlotCombo->currentText();
    int slotMinutes = 30;
    int numSlots = 20;

    if (comboText.contains("40") || comboIndex == 1) {
        slotMinutes = 40;
        numSlots = 20;
    } else {
        slotMinutes = 30;
        numSlots = 20;
    }

    Team team = m_db.getTeamByName(teamName);
    QList<Employee> members = m_db.getEmployeesByTeam(teamName);

    ShiftSchedule schedule = m_scheduler.generate8HourSchedule(team, members, date, startTime, slotMinutes, numSlots);
    displayRotationSchedule(schedule);
}

void MainWindow::on_rotationTeamCombo_currentIndexChanged(int index)
{
    Q_UNUSED(index);
    on_generateRotationBtn_clicked();
}

void MainWindow::displayRotationSchedule(const ShiftSchedule& schedule)
{
    ui->rotationTable->setRowCount(0);
    ui->rotationTable->setColumnCount(6);
    QStringList headers;
    headers << "Time Slot" << "Table 1" << "Table 2" << "Table 3" << "Table 4" << "Table 5 (Break)";
    ui->rotationTable->setHorizontalHeaderLabels(headers);

    ui->rotationStatusLabel->setText(
        QString("<b>Team: %1</b> | Shift: 8 Hours | Slot: %2 mins | Status: <b>%3</b>")
        .arg(schedule.teamName).arg(schedule.slotMinutes).arg(schedule.statusNotes)
    );

    for (int r = 0; r < schedule.rotationSlots.size(); ++r) {
        const RotationSlot& slot = schedule.rotationSlots[r];
        ui->rotationTable->insertRow(r);

        QString timeRange = QString("%1 - %2")
            .arg(slot.startTime.toString("hh:mm"))
            .arg(slot.endTime.toString("hh:mm"));
        ui->rotationTable->setItem(r, 0, new QTableWidgetItem(timeRange));

        // Fill 4 tables
        for (int t = 1; t <= 4; ++t) {
            QString assignedName = "-";
            for (const TableAssignment& assign : slot.tableAssignments) {
                if (assign.tableNumber == t) {
                    assignedName = assign.employeeName;
                    break;
                }
            }
            QTableWidgetItem* tableItem = new QTableWidgetItem(assignedName);
            if (assignedName.contains("UNATTENDED")) {
                tableItem->setBackground(QColor("#f8d7da"));
                tableItem->setForeground(QColor("#721c24"));
            }
            ui->rotationTable->setItem(r, t, tableItem);
        }

        // Fill Break column
        QString breakText = slot.onBreakEmployeeNames.isEmpty() ? "-" : slot.onBreakEmployeeNames.join(", ");
        QTableWidgetItem* breakItem = new QTableWidgetItem(breakText);
        if (!slot.onBreakEmployeeNames.isEmpty()) {
            breakItem->setBackground(QColor("#fff3cd")); // Gentle highlight for break
            breakItem->setForeground(QColor("#856404"));
        }
        ui->rotationTable->setItem(r, 5, breakItem);
    }

    ui->rotationTable->resizeColumnsToContents();
}

void MainWindow::on_reassignStaffBtn_clicked()
{
    ReassignDialog dialog(&m_db, this);
    if (dialog.exec() == QDialog::Accepted) {
        loadEmployeesTable();
        loadUniformsTable();
        loadIssuesTable();
        loadTeamsTable();

        ReassignmentResult res = dialog.getResult();
        if (res.success) {
            int idx = ui->rotationTeamCombo->findText(res.assignedTeam);
            if (idx >= 0) {
                ui->rotationTeamCombo->setCurrentIndex(idx);
            }
            on_generateRotationBtn_clicked();
        }
    }
}

void MainWindow::on_autoBalanceBtn_clicked()
{
    QList<Team> allTeams = m_db.getAllTeams();
    QStringList movesMade;
    QStringList shortages;

    // Search for overstaffed and understaffed teams
    for (const Team& sourceTeam : allTeams) {
        int currentStaff = m_db.getTeamMemberCount(sourceTeam.getName());
        if (sourceTeam.hasSurplusStaff(currentStaff)) {
            QList<Employee> members = m_db.getEmployeesByTeam(sourceTeam.getName());
            int excess = currentStaff - sourceTeam.getTargetStaff();

            for (int i = 0; i < excess && i < members.size(); ++i) {
                Employee emp = members[i];

                // Find an understaffed team
                QString understaffedTarget = "";
                for (const Team& targetTeam : allTeams) {
                    if (targetTeam.getName() != sourceTeam.getName()) {
                        int targetStaff = m_db.getTeamMemberCount(targetTeam.getName());
                        if (targetTeam.needsStaff(targetStaff)) {
                            understaffedTarget = targetTeam.getName();
                            break;
                        }
                    }
                }

                if (!understaffedTarget.isEmpty()) {
                    ReassignmentResult result = m_scheduler.reassignEmployee(emp.getId(), understaffedTarget, "Auto-balance staffing");
                    if (result.success) {
                        movesMade << result.message;
                    } else {
                        shortages << result.message;
                    }
                }
            }
        }
    }

    loadEmployeesTable();
    loadUniformsTable();
    loadIssuesTable();
    loadTeamsTable();
    on_generateRotationBtn_clicked();

    if (movesMade.isEmpty() && shortages.isEmpty()) {
        QMessageBox::information(this, "Auto-Balance", "Teams are already balanced or no transfer candidates found.");
    } else {
        QString report = "<b>Auto-Balance Results:</b><br><br>";
        if (!movesMade.isEmpty()) {
            report += "<b>Successful Reassignments:</b><br>• " + movesMade.join("<br>• ") + "<br><br>";
        }
        if (!shortages.isEmpty()) {
            report += "<b>Uniform Shortages Encountered:</b><br>• " + shortages.join("<br>• ");
        }
        QMessageBox::information(this, "Auto-Balance Report", report);
    }
}

void MainWindow::on_refreshTeamsBtn_clicked()
{
    loadTeamsTable();
    QMessageBox::information(this, "Success", "Teams and staffing table refreshed!");
}

// Employee actions
void MainWindow::on_addEmployeeBtn_clicked()
{
    EmployeeDialog dialog(this);
    QStringList teamNames;
    for (const Team& t : m_db.getAllTeams()) {
        teamNames << t.getName();
    }
    dialog.setTeamList(teamNames);

    if (dialog.exec() == QDialog::Accepted) {
        Employee emp = dialog.getEmployee();
        if (m_db.addEmployee(emp)) {
            loadEmployeesTable();
            loadTeamsTable();
            QMessageBox::information(this, "Success", "Employee added successfully!");
        } else {
            QMessageBox::warning(this, "Error", "Failed to add employee: " + m_db.getLastError());
        }
    }
}

void MainWindow::on_editEmployeeBtn_clicked()
{
    int row = ui->employeesTable->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "Warning", "Please select an employee to edit");
        return;
    }
    
    int employeeId = ui->employeesTable->item(row, 0)->text().toInt();
    Employee emp = m_db.getEmployee(employeeId);
    
    EmployeeDialog dialog(this);
    QStringList teamNames;
    for (const Team& t : m_db.getAllTeams()) {
        teamNames << t.getName();
    }
    dialog.setTeamList(teamNames);
    dialog.setEmployee(emp);
    if (dialog.exec() == QDialog::Accepted) {
        emp = dialog.getEmployee();
        emp.setId(employeeId);
        if (m_db.updateEmployee(emp)) {
            loadEmployeesTable();
            loadTeamsTable();
            QMessageBox::information(this, "Success", "Employee updated successfully!");
        } else {
            QMessageBox::warning(this, "Error", "Failed to update employee: " + m_db.getLastError());
        }
    }
}

void MainWindow::on_deleteEmployeeBtn_clicked()
{
    int row = ui->employeesTable->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "Warning", "Please select an employee to delete");
        return;
    }
    
    int employeeId = ui->employeesTable->item(row, 0)->text().toInt();
    QMessageBox::StandardButton reply = QMessageBox::question(this, "Confirm Delete", 
                                                              "Are you sure you want to delete this employee?",
                                                              QMessageBox::Yes | QMessageBox::No);
    
    if (reply == QMessageBox::Yes) {
        if (m_db.deleteEmployee(employeeId)) {
            loadEmployeesTable();
            loadTeamsTable();
            QMessageBox::information(this, "Success", "Employee deleted successfully!");
        } else {
            QMessageBox::warning(this, "Error", "Failed to delete employee: " + m_db.getLastError());
        }
    }
}

void MainWindow::on_refreshEmployeesBtn_clicked()
{
    loadEmployeesTable();
    QMessageBox::information(this, "Success", "Employees table refreshed!");
}

// Uniform actions
void MainWindow::on_addUniformBtn_clicked()
{
    UniformDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        Uniform uniform = dialog.getUniform();
        if (m_db.addUniform(uniform)) {
            loadUniformsTable();
            QMessageBox::information(this, "Success", "Uniform added successfully!");
        } else {
            QMessageBox::warning(this, "Error", "Failed to add uniform: " + m_db.getLastError());
        }
    }
}

void MainWindow::on_editUniformBtn_clicked()
{
    int row = ui->uniformsTable->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "Warning", "Please select a uniform to edit");
        return;
    }
    
    int uniformId = ui->uniformsTable->item(row, 0)->text().toInt();
    Uniform uniform = m_db.getUniform(uniformId);
    
    UniformDialog dialog(this);
    dialog.setUniform(uniform);
    if (dialog.exec() == QDialog::Accepted) {
        uniform = dialog.getUniform();
        uniform.setId(uniformId);
        if (m_db.updateUniform(uniform)) {
            loadUniformsTable();
            QMessageBox::information(this, "Success", "Uniform updated successfully!");
        } else {
            QMessageBox::warning(this, "Error", "Failed to update uniform: " + m_db.getLastError());
        }
    }
}

void MainWindow::on_deleteUniformBtn_clicked()
{
    int row = ui->uniformsTable->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "Warning", "Please select a uniform to delete");
        return;
    }
    
    int uniformId = ui->uniformsTable->item(row, 0)->text().toInt();
    QMessageBox::StandardButton reply = QMessageBox::question(this, "Confirm Delete", 
                                                              "Are you sure you want to delete this uniform?",
                                                              QMessageBox::Yes | QMessageBox::No);
    
    if (reply == QMessageBox::Yes) {
        if (m_db.deleteUniform(uniformId)) {
            loadUniformsTable();
            QMessageBox::information(this, "Success", "Uniform deleted successfully!");
        } else {
            QMessageBox::warning(this, "Error", "Failed to delete uniform: " + m_db.getLastError());
        }
    }
}

void MainWindow::on_refreshUniformsBtn_clicked()
{
    loadUniformsTable();
    QMessageBox::information(this, "Success", "Uniforms table refreshed!");
}

// Issue / Return actions
void MainWindow::on_issueUniformBtn_clicked()
{
    IssueDialog dialog(this);
    dialog.setEmployeeList(m_db.getAllEmployees());
    dialog.setUniformList(m_db.getAllUniforms());
    
    if (dialog.exec() == QDialog::Accepted) {
        UniformIssue issue = dialog.getIssueData();
        if (m_db.issueUniform(issue)) {
            loadIssuesTable();
            loadUniformsTable();
            loadEmployeesTable();
            QMessageBox::information(this, "Success", "Uniform issued successfully!");
        } else {
            QMessageBox::warning(this, "Error", "Failed to issue uniform: " + m_db.getLastError());
        }
    }
}

void MainWindow::on_returnUniformBtn_clicked()
{
    int row = ui->issuesTable->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "Warning", "Please select an issue to return");
        return;
    }
    
    int issueId = ui->issuesTable->item(row, 0)->text().toInt();
    if (m_db.returnUniform(issueId, QDate::currentDate().toString("yyyy-MM-dd"))) {
        loadIssuesTable();
        loadUniformsTable();
        loadEmployeesTable();
        QMessageBox::information(this, "Success", "Uniform returned successfully!");
    } else {
        QMessageBox::warning(this, "Error", "Failed to return uniform: " + m_db.getLastError());
    }
}

void MainWindow::on_refreshIssuesBtn_clicked()
{
    loadIssuesTable();
    QMessageBox::information(this, "Success", "Issues table refreshed!");
}
