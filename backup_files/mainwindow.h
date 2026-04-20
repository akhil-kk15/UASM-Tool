#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidgetItem>
#include "databasemanager.h"

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
    void on_addEmployeeBtn_clicked();
    void on_editEmployeeBtn_clicked();
    void on_deleteEmployeeBtn_clicked();
    void on_refreshEmployeesBtn_clicked();
    void on_addUniformBtn_clicked();
    void on_editUniformBtn_clicked();
    void on_deleteUniformBtn_clicked();
    void on_refreshUniformsBtn_clicked();
    void on_issueUniformBtn_clicked();
    void on_returnUniformBtn_clicked();
    void on_refreshIssuesBtn_clicked();

private:
    Ui::MainWindow *ui;
    DatabaseManager m_db;

    void loadEmployeesTable();
    void loadUniformsTable();
    void loadIssuesTable();
    void initializeDatabase();
};

#endif // MAINWINDOW_H

