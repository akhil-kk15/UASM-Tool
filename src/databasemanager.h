#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QString>
#include <QList>
#include <QSqlDatabase>
#include "employee.h"
#include "uniform.h"
#include "uniformissue.h"
#include "team.h"

class DatabaseManager
{
public:
    DatabaseManager();
    ~DatabaseManager();

    // Database initialization
    bool initializeDatabase(const QString& dbPath = "uniformmanagement.db");
    void closeDatabase();

    // Employee operations
    bool addEmployee(const Employee& employee);
    bool updateEmployee(const Employee& employee);
    bool updateEmployeeTeam(int employeeId, const QString& newTeam);
    bool deleteEmployee(int employeeId);
    Employee getEmployee(int employeeId);
    QList<Employee> getAllEmployees();
    QList<Employee> getEmployeesByTeam(const QString& teamName);

    // Team operations
    bool addTeam(const Team& team);
    bool updateTeam(const Team& team);
    bool deleteTeam(int teamId);
    Team getTeam(int teamId);
    Team getTeamByName(const QString& teamName);
    QList<Team> getAllTeams();
    int getTeamMemberCount(const QString& teamName);

    // Uniform operations
    bool addUniform(const Uniform& uniform);
    bool updateUniform(const Uniform& uniform);
    bool deleteUniform(int uniformId);
    Uniform getUniform(int uniformId);
    QList<Uniform> getAllUniforms();

    // Uniform availability & stock calculations
    int getIssuedUniformCount(int uniformId);
    int getAvailableStockForUniform(int uniformId);
    int getAvailableUniformCountForTeam(const QString& teamName, const QString& uniformType, const QString& size);
    Uniform findAvailableUniformForTeam(const QString& teamName, const QString& uniformType, const QString& size);

    // Uniform Issue operations
    bool issueUniform(const UniformIssue& issue);
    bool returnUniform(int issueId, const QString& returnDate);
    QList<UniformIssue> getIssueHistory(int employeeId);
    QList<UniformIssue> getPendingIssues();
    QList<UniformIssue> getAllIssues();

    // Schedule persistence
    bool clearShiftSchedule(const QString& teamName, const QDate& date);
    bool saveScheduleSlot(const QString& teamName, const QDate& date, int slotIndex,
                          const QString& startTime, const QString& endTime,
                          const QString& tableAssignments, const QString& breakEmployees);

    // Error handling
    QString getLastError() const;

private:
    QSqlDatabase m_database;
    QString m_lastError;

    bool createTables();
    bool upgradeTablesIfNeeded();
    void seedDefaultTeamsIfEmpty();
};

#endif // DATABASEMANAGER_H
