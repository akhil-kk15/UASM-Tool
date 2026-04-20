#include "databasemanager.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QDate>
#include <QDebug>

DatabaseManager::DatabaseManager() : m_lastError("")
{
}

DatabaseManager::~DatabaseManager()
{
    closeDatabase();
}

bool DatabaseManager::initializeDatabase()
{
    m_database = QSqlDatabase::addDatabase("QSQLITE");
    m_database.setDatabaseName("uniformmanagement.db");

    if (!m_database.open()) {
        m_lastError = "Failed to open database: " + m_database.lastError().text();
        return false;
    }

    if (!createTables()) {
        m_lastError = "Failed to create tables";
        return false;
    }

    return true;
}

void DatabaseManager::closeDatabase()
{
    if (m_database.isOpen()) {
        m_database.close();
    }
}

bool DatabaseManager::createTables()
{
    QSqlQuery query;

    // Create Employees table
    if (!query.exec("CREATE TABLE IF NOT EXISTS employees ("
                    "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                    "name TEXT NOT NULL,"
                    "department TEXT"
                    ")")) {
        m_lastError = query.lastError().text();
        return false;
    }

    // Create Uniforms table
    if (!query.exec("CREATE TABLE IF NOT EXISTS uniforms ("
                    "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                    "type TEXT NOT NULL,"
                    "quantity INTEGER,"
                    "size TEXT,"
                    "color TEXT,"
                    "description TEXT"
                    ")")) {
        m_lastError = query.lastError().text();
        return false;
    }

    // Create Uniform Issues table
    if (!query.exec("CREATE TABLE IF NOT EXISTS uniform_issues ("
                    "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                    "employee_id INTEGER NOT NULL,"
                    "uniform_id INTEGER NOT NULL,"
                    "quantity INTEGER,"
                    "issue_date DATE,"
                    "return_date DATE,"
                    "status TEXT DEFAULT 'Issued',"
                    "FOREIGN KEY(employee_id) REFERENCES employees(id),"
                    "FOREIGN KEY(uniform_id) REFERENCES uniforms(id)"
                    ")")) {
        m_lastError = query.lastError().text();
        return false;
    }

    return true;
}

// Employee operations
bool DatabaseManager::addEmployee(const Employee& employee)
{
    QSqlQuery query;
    query.prepare("INSERT INTO employees (name, department) "
                  "VALUES (:name, :department)");
    query.addBindValue(employee.getName());
    query.addBindValue(employee.getDepartment());

    if (!query.exec()) {
        m_lastError = query.lastError().text();
        return false;
    }
    return true;
}

bool DatabaseManager::updateEmployee(const Employee& employee)
{
    QSqlQuery query;
    query.prepare("UPDATE employees SET name=:name, department=:department WHERE id=:id");
    query.addBindValue(employee.getName());
    query.addBindValue(employee.getDepartment());
    query.addBindValue(employee.getId());

    if (!query.exec()) {
        m_lastError = query.lastError().text();
        return false;
    }
    return true;
}

bool DatabaseManager::deleteEmployee(int employeeId)
{
    QSqlQuery query;
    query.prepare("DELETE FROM employees WHERE id=:id");
    query.addBindValue(employeeId);

    if (!query.exec()) {
        m_lastError = query.lastError().text();
        return false;
    }
    return true;
}

Employee DatabaseManager::getEmployee(int employeeId)
{
    QSqlQuery query;
    query.prepare("SELECT * FROM employees WHERE id=:id");
    query.addBindValue(employeeId);

    if (query.exec() && query.next()) {
        Employee emp;
        emp.setId(query.value("id").toInt());
        emp.setName(query.value("name").toString());
        emp.setDepartment(query.value("department").toString());
        return emp;
    }
    m_lastError = query.lastError().text();
    return Employee();
}

QList<Employee> DatabaseManager::getAllEmployees()
{
    QList<Employee> employees;
    QSqlQuery query("SELECT * FROM employees");

    while (query.next()) {
        Employee emp;
        emp.setId(query.value("id").toInt());
        emp.setName(query.value("name").toString());
        emp.setDepartment(query.value("department").toString());
        employees.append(emp);
    }

    return employees;
}

// Uniform operations
bool DatabaseManager::addUniform(const Uniform& uniform)
{
    QSqlQuery query;
    query.prepare("INSERT INTO uniforms (type, quantity, size, color, description) "
                  "VALUES (:type, :quantity, :size, :color, :description)");
    query.addBindValue(uniform.getType());
    query.addBindValue(uniform.getQuantity());
    query.addBindValue(uniform.getSize());
    query.addBindValue(uniform.getColor());
    query.addBindValue(uniform.getDescription());

    if (!query.exec()) {
        m_lastError = query.lastError().text();
        return false;
    }
    return true;
}

bool DatabaseManager::updateUniform(const Uniform& uniform)
{
    QSqlQuery query;
    query.prepare("UPDATE uniforms SET type=:type, quantity=:quantity, size=:size, "
                  "color=:color, description=:description WHERE id=:id");
    query.addBindValue(uniform.getType());
    query.addBindValue(uniform.getQuantity());
    query.addBindValue(uniform.getSize());
    query.addBindValue(uniform.getColor());
    query.addBindValue(uniform.getDescription());
    query.addBindValue(uniform.getId());

    if (!query.exec()) {
        m_lastError = query.lastError().text();
        return false;
    }
    return true;
}

bool DatabaseManager::deleteUniform(int uniformId)
{
    QSqlQuery query;
    query.prepare("DELETE FROM uniforms WHERE id=:id");
    query.addBindValue(uniformId);

    if (!query.exec()) {
        m_lastError = query.lastError().text();
        return false;
    }
    return true;
}

Uniform DatabaseManager::getUniform(int uniformId)
{
    QSqlQuery query;
    query.prepare("SELECT * FROM uniforms WHERE id=:id");
    query.addBindValue(uniformId);

    if (query.exec() && query.next()) {
        Uniform uniform;
        uniform.setId(query.value("id").toInt());
        uniform.setType(query.value("type").toString());
        uniform.setQuantity(query.value("quantity").toInt());
        uniform.setSize(query.value("size").toString());
        uniform.setColor(query.value("color").toString());
        uniform.setDescription(query.value("description").toString());
        return uniform;
    }
    m_lastError = query.lastError().text();
    return Uniform();
}

QList<Uniform> DatabaseManager::getAllUniforms()
{
    QList<Uniform> uniforms;
    QSqlQuery query("SELECT * FROM uniforms");

    while (query.next()) {
        Uniform uniform;
        uniform.setId(query.value("id").toInt());
        uniform.setType(query.value("type").toString());
        uniform.setQuantity(query.value("quantity").toInt());
        uniform.setSize(query.value("size").toString());
        uniform.setColor(query.value("color").toString());
        uniform.setDescription(query.value("description").toString());
        uniforms.append(uniform);
    }

    return uniforms;
}

// Uniform Issue operations
bool DatabaseManager::issueUniform(const UniformIssue& issue)
{
    QSqlQuery query;
    query.prepare("INSERT INTO uniform_issues (employee_id, uniform_id, quantity, issue_date, status) "
                  "VALUES (:employee_id, :uniform_id, :quantity, :issue_date, :status)");
    query.addBindValue(issue.getEmployeeId());
    query.addBindValue(issue.getUniformId());
    query.addBindValue(issue.getQuantity());
    query.addBindValue(issue.getIssueDate());
    query.addBindValue(issue.getStatus());

    if (!query.exec()) {
        m_lastError = query.lastError().text();
        return false;
    }
    return true;
}

bool DatabaseManager::returnUniform(int issueId, const QString& returnDate)
{
    QSqlQuery query;
    query.prepare("UPDATE uniform_issues SET return_date=:return_date, status='Returned' WHERE id=:id");
    query.addBindValue(returnDate);
    query.addBindValue(issueId);

    if (!query.exec()) {
        m_lastError = query.lastError().text();
        return false;
    }
    return true;
}

QList<UniformIssue> DatabaseManager::getIssueHistory(int employeeId)
{
    QList<UniformIssue> issues;
    QSqlQuery query;
    query.prepare("SELECT * FROM uniform_issues WHERE employee_id=:employee_id");
    query.addBindValue(employeeId);

    if (query.exec()) {
        while (query.next()) {
            UniformIssue issue;
            issue.setId(query.value("id").toInt());
            issue.setEmployeeId(query.value("employee_id").toInt());
            issue.setUniformId(query.value("uniform_id").toInt());
            issue.setQuantity(query.value("quantity").toInt());
            issue.setIssueDate(query.value("issue_date").toDate());
            issue.setReturnDate(query.value("return_date").toDate());
            issue.setStatus(query.value("status").toString());
            issues.append(issue);
        }
    } else {
        m_lastError = query.lastError().text();
    }

    return issues;
}

QList<UniformIssue> DatabaseManager::getPendingIssues()
{
    QList<UniformIssue> issues;
    QSqlQuery query("SELECT * FROM uniform_issues WHERE status='Issued'");

    while (query.next()) {
        UniformIssue issue;
        issue.setId(query.value("id").toInt());
        issue.setEmployeeId(query.value("employee_id").toInt());
        issue.setUniformId(query.value("uniform_id").toInt());
        issue.setQuantity(query.value("quantity").toInt());
        issue.setIssueDate(query.value("issue_date").toDate());
        issue.setReturnDate(query.value("return_date").toDate());
        issue.setStatus(query.value("status").toString());
        issues.append(issue);
    }

    return issues;
}

QList<UniformIssue> DatabaseManager::getAllIssues()
{
    QList<UniformIssue> issues;
    QSqlQuery query("SELECT * FROM uniform_issues");

    while (query.next()) {
        UniformIssue issue;
        issue.setId(query.value("id").toInt());
        issue.setEmployeeId(query.value("employee_id").toInt());
        issue.setUniformId(query.value("uniform_id").toInt());
        issue.setQuantity(query.value("quantity").toInt());
        issue.setIssueDate(query.value("issue_date").toDate());
        issue.setReturnDate(query.value("return_date").toDate());
        issue.setStatus(query.value("status").toString());
        issues.append(issue);
    }

    return issues;
}

QString DatabaseManager::getLastError() const
{
    return m_lastError;
}
