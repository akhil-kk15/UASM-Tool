#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "employeedialog.h"
#include "uniformdialog.h"
#include "issuedialog.h"
#include "reassigndialog.h"
#include <QMessageBox>
#include <QTableWidgetItem>
#include <QListWidgetItem>
#include <QHeaderView>
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
        syncRotationSlotToTeam();
        on_generateRotationBtn_clicked();
    }

    logRecentChange(QString("Loaded %1 employees across %2 teams (70/30 Game Knowledge, 5-digit IDs, max 7 staff/shift).")
                    .arg(m_db.getAllEmployees().size())
                    .arg(m_db.getAllTeams().size()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::logRecentChange(const QString& message, bool isError)
{
    if (!ui->recentChangesList) return;

    QString timestamp = QTime::currentTime().toString("hh:mm:ss");
    QListWidgetItem* item = new QListWidgetItem(QString("[%1] %2").arg(timestamp, message));
    if (isError) {
        item->setBackground(QColor("#f8d7da"));
        item->setForeground(QColor("#721c24"));
    } else if (message.contains("->") || message.contains("Auto-Balance")) {
        item->setBackground(QColor("#d4edda"));
        item->setForeground(QColor("#155724"));
    }
    ui->recentChangesList->insertItem(0, item);
    ui->recentChangesList->scrollToTop();
    ui->statusbar->showMessage(message, 4000);
}

void MainWindow::on_clearChangesBtn_clicked()
{
    ui->recentChangesList->clear();
    ui->statusbar->showMessage("Cleared recent changes log", 2000);
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
    ui->employeesTable->setColumnCount(8);
    QStringList headers;
    headers << "ID" << "Name" << "Role Type" << "Designation" << "Team" << "Game Knowledge" << "Uniform Size" << "Issued Uniforms";
    ui->employeesTable->setHorizontalHeaderLabels(headers);
    ui->employeesTable->horizontalHeader()->setVisible(true);
    ui->employeesTable->horizontalHeader()->setStretchLastSection(true);
    
    QList<Employee> employees = m_db.getAllEmployees();
    
    for (int i = 0; i < employees.size(); ++i) {
        ui->employeesTable->insertRow(i);
        ui->employeesTable->setItem(i, 0, new QTableWidgetItem(QString::number(employees[i].getId())));
        ui->employeesTable->setItem(i, 1, new QTableWidgetItem(employees[i].getName()));
        ui->employeesTable->setItem(i, 2, new QTableWidgetItem(employees[i].getRoleType()));
        ui->employeesTable->setItem(i, 3, new QTableWidgetItem(employees[i].getDesignation()));
        ui->employeesTable->setItem(i, 4, new QTableWidgetItem(employees[i].getDepartment()));
        ui->employeesTable->setItem(i, 5, new QTableWidgetItem(employees[i].getGameKnowledge()));
        ui->employeesTable->setItem(i, 6, new QTableWidgetItem(employees[i].getUniformSize()));
        
        // Issued uniforms count
        QList<UniformIssue> issues = m_db.getIssueHistory(employees[i].getId());
        int issuedCount = 0;
        for (const auto& issue : issues) {
            if (issue.getStatus() == "Issued") {
                issuedCount += issue.getQuantity();
            }
        }
        ui->employeesTable->setItem(i, 7, new QTableWidgetItem(QString::number(issuedCount)));
    }
    ui->employeesTable->resizeColumnsToContents();
}

void MainWindow::loadUniformsTable()
{
    ui->uniformsTable->setRowCount(0);
    ui->uniformsTable->setColumnCount(8);
    QStringList headers;
    headers << "ID" << "Type" << "Team / Dept" << "Size" << "Total Stock" << "Extra Available" << "Color" << "Description";
    ui->uniformsTable->setHorizontalHeaderLabels(headers);
    ui->uniformsTable->horizontalHeader()->setVisible(true);
    ui->uniformsTable->horizontalHeader()->setStretchLastSection(true);
    
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
    ui->uniformsTable->resizeColumnsToContents();
}

void MainWindow::loadIssuesTable()
{
    ui->issuesTable->setRowCount(0);
    ui->issuesTable->horizontalHeader()->setVisible(true);
    ui->issuesTable->horizontalHeader()->setStretchLastSection(true);
    QList<UniformIssue> issues = m_db.getAllIssues();
    
    for (int i = 0; i < issues.size(); ++i) {
        Employee emp = m_db.getEmployee(issues[i].getEmployeeId());
        Uniform uniform = m_db.getUniform(issues[i].getUniformId());
        
        ui->issuesTable->insertRow(i);
        ui->issuesTable->setItem(i, 0, new QTableWidgetItem(QString::number(issues[i].getId())));
        ui->issuesTable->setItem(i, 1, new QTableWidgetItem(QString("[#%1] %2").arg(emp.getId()).arg(emp.getName())));
        ui->issuesTable->setItem(i, 2, new QTableWidgetItem(uniform.getType()));
        ui->issuesTable->setItem(i, 3, new QTableWidgetItem(QString::number(issues[i].getQuantity())));
        ui->issuesTable->setItem(i, 4, new QTableWidgetItem(issues[i].getIssueDate().toString("yyyy-MM-dd")));
        ui->issuesTable->setItem(i, 5, new QTableWidgetItem(issues[i].getReturnDate().toString("yyyy-MM-dd")));
        ui->issuesTable->setItem(i, 6, new QTableWidgetItem(issues[i].getStatus()));
    }
    ui->issuesTable->resizeColumnsToContents();
}

void MainWindow::loadTeamsTable()
{
    int previousRow = ui->teamsTable->currentRow();
    ui->teamsTable->setRowCount(0);
    ui->teamsTable->setColumnCount(8);
    QStringList headers;
    headers << "ID" << "Team Name" << "Team Type" << "Uniform Type" << "Current Staff" << "Target Staff" << "Staffing Status" << "Table Games (Tables 1-4)";
    ui->teamsTable->setHorizontalHeaderLabels(headers);
    ui->teamsTable->horizontalHeader()->setVisible(true);
    ui->teamsTable->horizontalHeader()->setStretchLastSection(true);

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
        ui->teamsTable->setItem(i, 7, new QTableWidgetItem(teams[i].getTableGames()));
    }
    ui->teamsTable->resizeColumnsToContents();

    if (teams.size() > 0) {
        int rowToSelect = (previousRow >= 0 && previousRow < teams.size()) ? previousRow : 0;
        ui->teamsTable->selectRow(rowToSelect);
        loadTeamMembersTable(teams[rowToSelect].getName());
    }
}

void MainWindow::loadTeamMembersTable(const QString& teamName)
{
    ui->teamMembersTable->setRowCount(0);
    ui->teamMembersTable->setColumnCount(6);
    QStringList headers;
    headers << "ID" << "Name" << "Role Type" << "Game Knowledge" << "Uniform Size" << "Issued Uniforms";
    ui->teamMembersTable->setHorizontalHeaderLabels(headers);
    ui->teamMembersTable->horizontalHeader()->setVisible(true);
    ui->teamMembersTable->horizontalHeader()->setStretchLastSection(true);

    QList<Employee> members = m_db.getEmployeesByTeam(teamName);
    for (int i = 0; i < members.size(); ++i) {
        ui->teamMembersTable->insertRow(i);
        ui->teamMembersTable->setItem(i, 0, new QTableWidgetItem(QString::number(members[i].getId())));
        ui->teamMembersTable->setItem(i, 1, new QTableWidgetItem(members[i].getName()));
        ui->teamMembersTable->setItem(i, 2, new QTableWidgetItem(members[i].getRoleType()));
        ui->teamMembersTable->setItem(i, 3, new QTableWidgetItem(members[i].getGameKnowledge()));
        ui->teamMembersTable->setItem(i, 4, new QTableWidgetItem(members[i].getUniformSize()));

        QList<UniformIssue> issues = m_db.getIssueHistory(members[i].getId());
        int count = 0;
        for (const auto& issue : issues) {
            if (issue.getStatus() == "Issued") count += issue.getQuantity();
        }
        ui->teamMembersTable->setItem(i, 5, new QTableWidgetItem(QString::number(count)));
    }
    ui->teamMembersTable->resizeColumnsToContents();
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

    if (comboText.contains("20") || comboIndex == 2) {
        slotMinutes = 20;
        numSlots = 20;
    } else if (comboText.contains("40") || comboIndex == 1) {
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

void MainWindow::syncRotationSlotToTeam()
{
    QString teamName = ui->rotationTeamCombo->currentText();
    if (!teamName.isEmpty()) {
        Team team = m_db.getTeamByName(teamName);
        int memberCount = m_db.getTeamMemberCount(teamName);
        int recMinutes = m_scheduler.recommendSlotMinutes(team, memberCount);
        if (recMinutes == 20) {
            ui->rotationSlotCombo->setCurrentIndex(2); // 20 Mins (Dire)
        } else if (recMinutes == 40) {
            ui->rotationSlotCombo->setCurrentIndex(1); // 40 Mins
        } else {
            ui->rotationSlotCombo->setCurrentIndex(0); // 30 Mins
        }
    }
}

void MainWindow::on_rotationTeamCombo_currentIndexChanged(int index)
{
    Q_UNUSED(index);
    syncRotationSlotToTeam();
    on_generateRotationBtn_clicked();
}

void MainWindow::on_autoBalanceRotationBtn_clicked()
{
    on_autoBalanceBtn_clicked();
}

void MainWindow::displayRotationSchedule(const ShiftSchedule& schedule)
{
    ui->rotationTable->setRowCount(0);
    ui->rotationTable->setColumnCount(6);

    QString g1 = schedule.tableGames.size() >= 1 ? schedule.tableGames[0] : "Blackjack";
    QString g2 = schedule.tableGames.size() >= 2 ? schedule.tableGames[1] : "Blackjack";
    QString g3 = schedule.tableGames.size() >= 3 ? schedule.tableGames[2] : "Blackjack";
    QString g4 = schedule.tableGames.size() >= 4 ? schedule.tableGames[3] : "Blackjack";

    QStringList headers;
    headers << "Time Slot"
            << QString("Table 1 (%1)").arg(g1)
            << QString("Table 2 (%1)").arg(g2)
            << QString("Table 3 (%1)").arg(g3)
            << QString("Table 4 (%1)").arg(g4)
            << "Break";
    ui->rotationTable->setHorizontalHeaderLabels(headers);
    for (int col = 0; col < headers.size(); ++col) {
        QTableWidgetItem* hItem = ui->rotationTable->horizontalHeaderItem(col);
        if (!hItem) {
            hItem = new QTableWidgetItem(headers[col]);
            ui->rotationTable->setHorizontalHeaderItem(col, hItem);
        } else {
            hItem->setText(headers[col]);
        }
    }
    ui->rotationTable->horizontalHeader()->setVisible(true);
    ui->rotationTable->horizontalHeader()->setMinimumSectionSize(110);
    ui->rotationTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    ui->rotationStatusLabel->setText(
        QString("<b>Team: %1</b> | Games: %2 | Shift: 8 Hours (Max 7 Staff) | Slot: %3 mins | Status: <b>%4</b>")
        .arg(schedule.teamName)
        .arg(schedule.tableGames.join(", "))
        .arg(schedule.slotMinutes)
        .arg(schedule.statusNotes)
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
            logRecentChange(res.message);
            int idx = ui->rotationTeamCombo->findText(res.assignedTeam);
            if (idx >= 0) {
                ui->rotationTeamCombo->setCurrentIndex(idx);
            }
            syncRotationSlotToTeam();
            on_generateRotationBtn_clicked();
        } else if (!res.message.isEmpty()) {
            logRecentChange(res.message, true);
        }
    }
}

void MainWindow::on_autoBalanceBtn_clicked()
{
    QStringList movesMade;
    QStringList shortages;

    // 1. Relocate any employees exceeding the 7-person shift limit to compatible teams
    QList<ReassignmentResult> overflowMoves = m_scheduler.relocateOverflowStaff();
    for (const ReassignmentResult& res : overflowMoves) {
        if (res.success) {
            movesMade << res.message;
        } else {
            shortages << res.message;
        }
    }

    // 2. Balance any remaining understaffed teams (< 6 staff) from teams that have 7 staff
    QList<Team> allTeams = m_db.getAllTeams();
    for (const Team& sourceTeam : allTeams) {
        int currentStaff = m_db.getTeamMemberCount(sourceTeam.getName());
        if (currentStaff >= 7) {
            QList<Employee> members = m_db.getEmployeesByTeam(sourceTeam.getName());
            for (int i = members.size() - 1; i >= 0 && currentStaff > 6; --i) {
                Employee emp = members[i];
                QString understaffedTarget = "";
                int lowestStaff = 999;

                for (const Team& targetTeam : allTeams) {
                    if (targetTeam.getName() != sourceTeam.getName()) {
                        int targetStaff = m_db.getTeamMemberCount(targetTeam.getName());
                        if (targetStaff < 6 && targetStaff < lowestStaff && targetTeam.acceptsEmployee(emp)) {
                            lowestStaff = targetStaff;
                            understaffedTarget = targetTeam.getName();
                        }
                    }
                }

                if (!understaffedTarget.isEmpty()) {
                    ReassignmentResult result = m_scheduler.reassignEmployee(emp.getId(), understaffedTarget, "Auto-balance staffing");
                    if (result.success) {
                        movesMade << result.message;
                        currentStaff--;
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
    syncRotationSlotToTeam();
    on_generateRotationBtn_clicked();

    if (movesMade.isEmpty() && shortages.isEmpty()) {
        logRecentChange("Auto-Balance: All 15 teams are already balanced at Optimal 7/7 shift staffing (30 Mins rotation).");
    } else {
        for (const QString& move : movesMade) {
            logRecentChange(move, false);
        }
        for (const QString& err : shortages) {
            logRecentChange(err, true);
        }
        logRecentChange(QString("Auto-Balance complete: %1 staff reassigned -> All 15 teams balanced to Optimal 7/7 (30 Mins shift rotation).")
                        .arg(movesMade.size()), false);
    }
}

void MainWindow::on_refreshBtn_clicked()
{
    QString currentRotTeam = ui->rotationTeamCombo->currentText();

    loadEmployeesTable();
    loadUniformsTable();
    loadIssuesTable();
    loadTeamsTable();

    ui->rotationTeamCombo->blockSignals(true);
    populateRotationTeamCombo();
    int idx = ui->rotationTeamCombo->findText(currentRotTeam);
    if (idx >= 0) {
        ui->rotationTeamCombo->setCurrentIndex(idx);
    }
    ui->rotationTeamCombo->blockSignals(false);

    if (ui->rotationTeamCombo->count() > 0) {
        syncRotationSlotToTeam();
        on_generateRotationBtn_clicked();
    }

    logRecentChange("Refreshed all tables, uniform inventory, and team rotation schedules.");
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
        Team selectedTeam = m_db.getTeamByName(emp.getDepartment());
        int currentTeamCount = m_db.getTeamMemberCount(emp.getDepartment());
        QString placementNote;

        // If selected team does not match employee's game knowledge or already has 7 people,
        // automatically place them in a compatible team (< 7 staff) based on their game knowledge
        if (selectedTeam.getId() > 0 && (!selectedTeam.acceptsEmployee(emp) || currentTeamCount >= RotationScheduler::MAX_SHIFT_STAFF)) {
            QString reason = !selectedTeam.acceptsEmployee(emp)
                ? QString("%1 requires games [%2]").arg(selectedTeam.getName(), selectedTeam.getUniqueGames().join(", "))
                : QString("%1 is at 7-staff shift limit").arg(selectedTeam.getName());

            for (const Team& candidate : m_db.getAllTeams()) {
                if (candidate.acceptsEmployee(emp) && m_db.getTeamMemberCount(candidate.getName()) < RotationScheduler::MAX_SHIFT_STAFF) {
                    emp.setDepartment(candidate.getName());
                    placementNote = QString(" (%1 -> placed in %2 by game knowledge)").arg(reason, candidate.getName());
                    break;
                }
            }
        }

        if (m_db.addEmployee(emp)) {
            loadEmployeesTable();
            loadTeamsTable();
            syncRotationSlotToTeam();
            on_generateRotationBtn_clicked();
            logRecentChange(QString("Added employee '%1' to %2 [Games: %3]%4")
                            .arg(emp.getName(), emp.getDepartment(), emp.getGameKnowledge(), placementNote));
        } else {
            logRecentChange("Failed to add employee: " + m_db.getLastError(), true);
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
            syncRotationSlotToTeam();
            on_generateRotationBtn_clicked();
            logRecentChange(QString("Updated employee [#%1] %2 (Team: %3 | Games: %4)")
                            .arg(emp.getId()).arg(emp.getName(), emp.getDepartment(), emp.getGameKnowledge()));
        } else {
            logRecentChange("Failed to update employee: " + m_db.getLastError(), true);
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
    QString empName = ui->employeesTable->item(row, 1)->text();
    QMessageBox::StandardButton reply = QMessageBox::question(this, "Confirm Delete", 
                                                              "Are you sure you want to delete this employee?",
                                                              QMessageBox::Yes | QMessageBox::No);
    
    if (reply == QMessageBox::Yes) {
        if (m_db.deleteEmployee(employeeId)) {
            loadEmployeesTable();
            loadTeamsTable();
            syncRotationSlotToTeam();
            on_generateRotationBtn_clicked();
            logRecentChange(QString("Deleted employee [#%1] %2").arg(employeeId).arg(empName));
        } else {
            logRecentChange("Failed to delete employee: " + m_db.getLastError(), true);
        }
    }
}

// Uniform actions
void MainWindow::on_addUniformBtn_clicked()
{
    UniformDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        Uniform uniform = dialog.getUniform();
        if (m_db.addUniform(uniform)) {
            loadUniformsTable();
            logRecentChange(QString("Added uniform '%1' (Team: %2, Size: %3, Qty: %4)")
                            .arg(uniform.getType(), uniform.getTeam(), uniform.getSize())
                            .arg(uniform.getQuantity()));
        } else {
            logRecentChange("Failed to add uniform: " + m_db.getLastError(), true);
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
            logRecentChange(QString("Updated uniform '%1' (Size: %2, Qty: %3)")
                            .arg(uniform.getType(), uniform.getSize()).arg(uniform.getQuantity()));
        } else {
            logRecentChange("Failed to update uniform: " + m_db.getLastError(), true);
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
    QString uniType = ui->uniformsTable->item(row, 1)->text();
    QMessageBox::StandardButton reply = QMessageBox::question(this, "Confirm Delete", 
                                                              "Are you sure you want to delete this uniform?",
                                                              QMessageBox::Yes | QMessageBox::No);
    
    if (reply == QMessageBox::Yes) {
        if (m_db.deleteUniform(uniformId)) {
            loadUniformsTable();
            logRecentChange(QString("Deleted uniform '%1' (ID: %2)").arg(uniType).arg(uniformId));
        } else {
            logRecentChange("Failed to delete uniform: " + m_db.getLastError(), true);
        }
    }
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
            Employee emp = m_db.getEmployee(issue.getEmployeeId());
            Uniform uni = m_db.getUniform(issue.getUniformId());
            logRecentChange(QString("Issued '%1' (Size %2) to [#%3] %4")
                            .arg(uni.getType(), uni.getSize()).arg(emp.getId()).arg(emp.getName()));
        } else {
            logRecentChange("Failed to issue uniform: " + m_db.getLastError(), true);
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
    QString empLabel = ui->issuesTable->item(row, 1)->text();
    QString uniType = ui->issuesTable->item(row, 2)->text();
    if (m_db.returnUniform(issueId, QDate::currentDate().toString("yyyy-MM-dd"))) {
        loadIssuesTable();
        loadUniformsTable();
        loadEmployeesTable();
        logRecentChange(QString("Returned uniform '%1' from %2").arg(uniType, empLabel));
    } else {
        logRecentChange("Failed to return uniform: " + m_db.getLastError(), true);
    }
}
