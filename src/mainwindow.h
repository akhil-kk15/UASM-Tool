#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidgetItem>
#include "databasemanager.h"
#include "rotationscheduler.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // Universal refresh & sidebar slots
    void on_refreshBtn_clicked();
    void on_clearChangesBtn_clicked();

    // Employee slots
    void on_addEmployeeBtn_clicked();
    void on_editEmployeeBtn_clicked();
    void on_deleteEmployeeBtn_clicked();

    // Uniform slots
    void on_addUniformBtn_clicked();
    void on_editUniformBtn_clicked();
    void on_deleteUniformBtn_clicked();

    // Issue slots
    void on_issueUniformBtn_clicked();
    void on_returnUniformBtn_clicked();

    // Team & Staffing slots
    void on_reassignStaffBtn_clicked();
    void on_autoBalanceBtn_clicked();
    void on_teamsTable_itemSelectionChanged();

    // Rotation schedule slots
    void on_generateRotationBtn_clicked();
    void on_autoBalanceRotationBtn_clicked();
    void on_rotationTeamCombo_currentIndexChanged(int index);

private:
    Ui::MainWindow *ui;
    DatabaseManager m_db;
    RotationScheduler m_scheduler;

    void initializeDatabase();
    void loadEmployeesTable();
    void loadUniformsTable();
    void loadIssuesTable();
    void loadTeamsTable();
    void loadTeamMembersTable(const QString& teamName);
    void populateRotationTeamCombo();
    void syncRotationSlotToTeam();
    void displayRotationSchedule(const ShiftSchedule& schedule);
    void logRecentChange(const QString& message, bool isError = false);
};

#endif // MAINWINDOW_H
