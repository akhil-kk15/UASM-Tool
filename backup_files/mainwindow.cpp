#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "employeedialog.h"
#include "uniformdialog.h"
#include "issuedialog.h"
#include <QMessageBox>
#include <QTableWidgetItem>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    initializeDatabase();
    loadEmployeesTable();
    loadUniformsTable();
    loadIssuesTable();
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
    QList<Employee> employees = m_db.getAllEmployees();
    
    for (int i = 0; i < employees.size(); ++i) {
        ui->employeesTable->insertRow(i);
        ui->employeesTable->setItem(i, 0, new QTableWidgetItem(QString::number(employees[i].getId())));
        ui->employeesTable->setItem(i, 1, new QTableWidgetItem(employees[i].getName()));
        ui->employeesTable->setItem(i, 2, new QTableWidgetItem(employees[i].getDesignation()));
        ui->employeesTable->setItem(i, 3, new QTableWidgetItem(employees[i].getDepartment()));
        ui->employeesTable->setItem(i, 4, new QTableWidgetItem(employees[i].getEmail()));
        ui->employeesTable->setItem(i, 5, new QTableWidgetItem(employees[i].getPhone()));
        ui->employeesTable->setItem(i, 6, new QTableWidgetItem(employees[i].getUniformSize()));
        ui->employeesTable->setItem(i, 7, new QTableWidgetItem(employees[i].getJoiningDate().toString("yyyy-MM-dd")));
    }
}

void MainWindow::loadUniformsTable()
{
    ui->uniformsTable->setRowCount(0);
    QList<Uniform> uniforms = m_db.getAllUniforms();
    
    for (int i = 0; i < uniforms.size(); ++i) {
        ui->uniformsTable->insertRow(i);
        ui->uniformsTable->setItem(i, 0, new QTableWidgetItem(QString::number(uniforms[i].getId())));
        ui->uniformsTable->setItem(i, 1, new QTableWidgetItem(uniforms[i].getType()));
        ui->uniformsTable->setItem(i, 2, new QTableWidgetItem(QString::number(uniforms[i].getQuantity())));
        ui->uniformsTable->setItem(i, 3, new QTableWidgetItem(uniforms[i].getSize()));
        ui->uniformsTable->setItem(i, 4, new QTableWidgetItem(uniforms[i].getColor()));
        ui->uniformsTable->setItem(i, 5, new QTableWidgetItem(uniforms[i].getPurchaseDate().toString("yyyy-MM-dd")));
        ui->uniformsTable->setItem(i, 6, new QTableWidgetItem(uniforms[i].getDescription()));
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

void MainWindow::on_addEmployeeBtn_clicked()
{
    EmployeeDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        Employee emp = dialog.getEmployee();
        if (m_db.addEmployee(emp)) {
            loadEmployeesTable();
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
    dialog.setEmployee(emp);
    if (dialog.exec() == QDialog::Accepted) {
        emp = dialog.getEmployee();
        emp.setId(employeeId);
        if (m_db.updateEmployee(emp)) {
            loadEmployeesTable();
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

void MainWindow::on_issueUniformBtn_clicked()
{
    IssueDialog dialog(this);
    dialog.setEmployeeList(m_db.getAllEmployees());
    dialog.setUniformList(m_db.getAllUniforms());
    
    if (dialog.exec() == QDialog::Accepted) {
        UniformIssue issue = dialog.getIssueData();
        if (m_db.issueUniform(issue)) {
            loadIssuesTable();
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
