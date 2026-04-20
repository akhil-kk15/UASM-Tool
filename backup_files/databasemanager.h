#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QString>
#include <QList>
#include <QSqlDatabase>
#include "employee.h"
#include "uniform.h"
#include "uniformissue.h"

class DatabaseManager
{
public:
    DatabaseManager();
    ~DatabaseManager();

    // Database initialization
    bool initializeDatabase();
    void closeDatabase();

    // Employee operations
    bool addEmployee(const Employee& employee);
    bool updateEmployee(const Employee& employee);
    bool deleteEmployee(int employeeId);
    Employee getEmployee(int employeeId);
    QList<Employee> getAllEmployees();

    // Uniform operations
    bool addUniform(const Uniform& uniform);
    bool updateUniform(const Uniform& uniform);
    bool deleteUniform(int uniformId);
    Uniform getUniform(int uniformId);
    QList<Uniform> getAllUniforms();

    // Uniform Issue operations
    bool issueUniform(const UniformIssue& issue);
    bool returnUniform(int issueId, const QString& returnDate);
    QList<UniformIssue> getIssueHistory(int employeeId);
    QList<UniformIssue> getPendingIssues();
    QList<UniformIssue> getAllIssues();

    // Error handling
    QString getLastError() const;

private:
    QSqlDatabase m_database;
    QString m_lastError;

    bool createTables();
};

#endif // DATABASEMANAGER_H
