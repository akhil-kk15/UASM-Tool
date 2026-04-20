/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.11.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QTabWidget *tabWidget;
    QWidget *employeesTab;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout;
    QPushButton *addEmployeeBtn;
    QPushButton *editEmployeeBtn;
    QPushButton *deleteEmployeeBtn;
    QPushButton *refreshEmployeesBtn;
    QSpacerItem *horizontalSpacer;
    QTableWidget *employeesTable;
    QWidget *uniformsTab;
    QVBoxLayout *verticalLayout_3;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *addUniformBtn;
    QPushButton *editUniformBtn;
    QPushButton *deleteUniformBtn;
    QPushButton *refreshUniformsBtn;
    QSpacerItem *horizontalSpacer_2;
    QTableWidget *uniformsTable;
    QWidget *issuesTab;
    QVBoxLayout *verticalLayout_4;
    QHBoxLayout *horizontalLayout_3;
    QPushButton *issueUniformBtn;
    QPushButton *returnUniformBtn;
    QPushButton *refreshIssuesBtn;
    QSpacerItem *horizontalSpacer_3;
    QTableWidget *issuesTable;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1200, 700);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setObjectName("verticalLayout");
        tabWidget = new QTabWidget(centralwidget);
        tabWidget->setObjectName("tabWidget");
        employeesTab = new QWidget();
        employeesTab->setObjectName("employeesTab");
        verticalLayout_2 = new QVBoxLayout(employeesTab);
        verticalLayout_2->setObjectName("verticalLayout_2");
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        addEmployeeBtn = new QPushButton(employeesTab);
        addEmployeeBtn->setObjectName("addEmployeeBtn");

        horizontalLayout->addWidget(addEmployeeBtn);

        editEmployeeBtn = new QPushButton(employeesTab);
        editEmployeeBtn->setObjectName("editEmployeeBtn");

        horizontalLayout->addWidget(editEmployeeBtn);

        deleteEmployeeBtn = new QPushButton(employeesTab);
        deleteEmployeeBtn->setObjectName("deleteEmployeeBtn");

        horizontalLayout->addWidget(deleteEmployeeBtn);

        refreshEmployeesBtn = new QPushButton(employeesTab);
        refreshEmployeesBtn->setObjectName("refreshEmployeesBtn");

        horizontalLayout->addWidget(refreshEmployeesBtn);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);


        verticalLayout_2->addLayout(horizontalLayout);

        employeesTable = new QTableWidget(employeesTab);
        if (employeesTable->columnCount() < 8)
            employeesTable->setColumnCount(8);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        employeesTable->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        employeesTable->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        employeesTable->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        employeesTable->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        employeesTable->setHorizontalHeaderItem(4, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        employeesTable->setHorizontalHeaderItem(5, __qtablewidgetitem5);
        QTableWidgetItem *__qtablewidgetitem6 = new QTableWidgetItem();
        employeesTable->setHorizontalHeaderItem(6, __qtablewidgetitem6);
        QTableWidgetItem *__qtablewidgetitem7 = new QTableWidgetItem();
        employeesTable->setHorizontalHeaderItem(7, __qtablewidgetitem7);
        employeesTable->setObjectName("employeesTable");
        employeesTable->setColumnCount(8);
        employeesTable->horizontalHeader()->setStretchLastSection(false);

        verticalLayout_2->addWidget(employeesTable);

        tabWidget->addTab(employeesTab, QString());
        uniformsTab = new QWidget();
        uniformsTab->setObjectName("uniformsTab");
        verticalLayout_3 = new QVBoxLayout(uniformsTab);
        verticalLayout_3->setObjectName("verticalLayout_3");
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        addUniformBtn = new QPushButton(uniformsTab);
        addUniformBtn->setObjectName("addUniformBtn");

        horizontalLayout_2->addWidget(addUniformBtn);

        editUniformBtn = new QPushButton(uniformsTab);
        editUniformBtn->setObjectName("editUniformBtn");

        horizontalLayout_2->addWidget(editUniformBtn);

        deleteUniformBtn = new QPushButton(uniformsTab);
        deleteUniformBtn->setObjectName("deleteUniformBtn");

        horizontalLayout_2->addWidget(deleteUniformBtn);

        refreshUniformsBtn = new QPushButton(uniformsTab);
        refreshUniformsBtn->setObjectName("refreshUniformsBtn");

        horizontalLayout_2->addWidget(refreshUniformsBtn);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);


        verticalLayout_3->addLayout(horizontalLayout_2);

        uniformsTable = new QTableWidget(uniformsTab);
        if (uniformsTable->columnCount() < 7)
            uniformsTable->setColumnCount(7);
        QTableWidgetItem *__qtablewidgetitem8 = new QTableWidgetItem();
        uniformsTable->setHorizontalHeaderItem(0, __qtablewidgetitem8);
        QTableWidgetItem *__qtablewidgetitem9 = new QTableWidgetItem();
        uniformsTable->setHorizontalHeaderItem(1, __qtablewidgetitem9);
        QTableWidgetItem *__qtablewidgetitem10 = new QTableWidgetItem();
        uniformsTable->setHorizontalHeaderItem(2, __qtablewidgetitem10);
        QTableWidgetItem *__qtablewidgetitem11 = new QTableWidgetItem();
        uniformsTable->setHorizontalHeaderItem(3, __qtablewidgetitem11);
        QTableWidgetItem *__qtablewidgetitem12 = new QTableWidgetItem();
        uniformsTable->setHorizontalHeaderItem(4, __qtablewidgetitem12);
        QTableWidgetItem *__qtablewidgetitem13 = new QTableWidgetItem();
        uniformsTable->setHorizontalHeaderItem(5, __qtablewidgetitem13);
        QTableWidgetItem *__qtablewidgetitem14 = new QTableWidgetItem();
        uniformsTable->setHorizontalHeaderItem(6, __qtablewidgetitem14);
        uniformsTable->setObjectName("uniformsTable");
        uniformsTable->setColumnCount(7);
        uniformsTable->horizontalHeader()->setStretchLastSection(false);

        verticalLayout_3->addWidget(uniformsTable);

        tabWidget->addTab(uniformsTab, QString());
        issuesTab = new QWidget();
        issuesTab->setObjectName("issuesTab");
        verticalLayout_4 = new QVBoxLayout(issuesTab);
        verticalLayout_4->setObjectName("verticalLayout_4");
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName("horizontalLayout_3");
        issueUniformBtn = new QPushButton(issuesTab);
        issueUniformBtn->setObjectName("issueUniformBtn");

        horizontalLayout_3->addWidget(issueUniformBtn);

        returnUniformBtn = new QPushButton(issuesTab);
        returnUniformBtn->setObjectName("returnUniformBtn");

        horizontalLayout_3->addWidget(returnUniformBtn);

        refreshIssuesBtn = new QPushButton(issuesTab);
        refreshIssuesBtn->setObjectName("refreshIssuesBtn");

        horizontalLayout_3->addWidget(refreshIssuesBtn);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_3);


        verticalLayout_4->addLayout(horizontalLayout_3);

        issuesTable = new QTableWidget(issuesTab);
        if (issuesTable->columnCount() < 7)
            issuesTable->setColumnCount(7);
        QTableWidgetItem *__qtablewidgetitem15 = new QTableWidgetItem();
        issuesTable->setHorizontalHeaderItem(0, __qtablewidgetitem15);
        QTableWidgetItem *__qtablewidgetitem16 = new QTableWidgetItem();
        issuesTable->setHorizontalHeaderItem(1, __qtablewidgetitem16);
        QTableWidgetItem *__qtablewidgetitem17 = new QTableWidgetItem();
        issuesTable->setHorizontalHeaderItem(2, __qtablewidgetitem17);
        QTableWidgetItem *__qtablewidgetitem18 = new QTableWidgetItem();
        issuesTable->setHorizontalHeaderItem(3, __qtablewidgetitem18);
        QTableWidgetItem *__qtablewidgetitem19 = new QTableWidgetItem();
        issuesTable->setHorizontalHeaderItem(4, __qtablewidgetitem19);
        QTableWidgetItem *__qtablewidgetitem20 = new QTableWidgetItem();
        issuesTable->setHorizontalHeaderItem(5, __qtablewidgetitem20);
        QTableWidgetItem *__qtablewidgetitem21 = new QTableWidgetItem();
        issuesTable->setHorizontalHeaderItem(6, __qtablewidgetitem21);
        issuesTable->setObjectName("issuesTable");
        issuesTable->setColumnCount(7);
        issuesTable->horizontalHeader()->setStretchLastSection(false);

        verticalLayout_4->addWidget(issuesTable);

        tabWidget->addTab(issuesTab, QString());

        verticalLayout->addWidget(tabWidget);

        MainWindow->setCentralWidget(centralwidget);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "Uniform Management System", nullptr));
        addEmployeeBtn->setText(QCoreApplication::translate("MainWindow", "Add Employee", nullptr));
        editEmployeeBtn->setText(QCoreApplication::translate("MainWindow", "Edit Employee", nullptr));
        deleteEmployeeBtn->setText(QCoreApplication::translate("MainWindow", "Delete Employee", nullptr));
        refreshEmployeesBtn->setText(QCoreApplication::translate("MainWindow", "Refresh", nullptr));
        QTableWidgetItem *___qtablewidgetitem = employeesTable->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("MainWindow", "ID", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = employeesTable->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("MainWindow", "Name", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = employeesTable->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QCoreApplication::translate("MainWindow", "Designation", nullptr));
        QTableWidgetItem *___qtablewidgetitem3 = employeesTable->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QCoreApplication::translate("MainWindow", "Department", nullptr));
        QTableWidgetItem *___qtablewidgetitem4 = employeesTable->horizontalHeaderItem(4);
        ___qtablewidgetitem4->setText(QCoreApplication::translate("MainWindow", "Email", nullptr));
        QTableWidgetItem *___qtablewidgetitem5 = employeesTable->horizontalHeaderItem(5);
        ___qtablewidgetitem5->setText(QCoreApplication::translate("MainWindow", "Phone", nullptr));
        QTableWidgetItem *___qtablewidgetitem6 = employeesTable->horizontalHeaderItem(6);
        ___qtablewidgetitem6->setText(QCoreApplication::translate("MainWindow", "Uniform Size", nullptr));
        QTableWidgetItem *___qtablewidgetitem7 = employeesTable->horizontalHeaderItem(7);
        ___qtablewidgetitem7->setText(QCoreApplication::translate("MainWindow", "Joining Date", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(employeesTab), QCoreApplication::translate("MainWindow", "Employees", nullptr));
        addUniformBtn->setText(QCoreApplication::translate("MainWindow", "Add Uniform", nullptr));
        editUniformBtn->setText(QCoreApplication::translate("MainWindow", "Edit Uniform", nullptr));
        deleteUniformBtn->setText(QCoreApplication::translate("MainWindow", "Delete Uniform", nullptr));
        refreshUniformsBtn->setText(QCoreApplication::translate("MainWindow", "Refresh", nullptr));
        QTableWidgetItem *___qtablewidgetitem8 = uniformsTable->horizontalHeaderItem(0);
        ___qtablewidgetitem8->setText(QCoreApplication::translate("MainWindow", "ID", nullptr));
        QTableWidgetItem *___qtablewidgetitem9 = uniformsTable->horizontalHeaderItem(1);
        ___qtablewidgetitem9->setText(QCoreApplication::translate("MainWindow", "Type", nullptr));
        QTableWidgetItem *___qtablewidgetitem10 = uniformsTable->horizontalHeaderItem(2);
        ___qtablewidgetitem10->setText(QCoreApplication::translate("MainWindow", "Team", nullptr));
        QTableWidgetItem *___qtablewidgetitem11 = uniformsTable->horizontalHeaderItem(3);
        ___qtablewidgetitem11->setText(QCoreApplication::translate("MainWindow", "Size", nullptr));
        QTableWidgetItem *___qtablewidgetitem12 = uniformsTable->horizontalHeaderItem(4);
        ___qtablewidgetitem12->setText(QCoreApplication::translate("MainWindow", "Accessory", nullptr));
        QTableWidgetItem *___qtablewidgetitem13 = uniformsTable->horizontalHeaderItem(5);
        ___qtablewidgetitem13->setText(QCoreApplication::translate("MainWindow", "Purchase Date", nullptr));
        QTableWidgetItem *___qtablewidgetitem14 = uniformsTable->horizontalHeaderItem(6);
        ___qtablewidgetitem14->setText(QCoreApplication::translate("MainWindow", "Description", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(uniformsTab), QCoreApplication::translate("MainWindow", "Uniforms", nullptr));
        issueUniformBtn->setText(QCoreApplication::translate("MainWindow", "Issue Uniform", nullptr));
        returnUniformBtn->setText(QCoreApplication::translate("MainWindow", "Return Uniform", nullptr));
        refreshIssuesBtn->setText(QCoreApplication::translate("MainWindow", "Refresh", nullptr));
        QTableWidgetItem *___qtablewidgetitem15 = issuesTable->horizontalHeaderItem(0);
        ___qtablewidgetitem15->setText(QCoreApplication::translate("MainWindow", "ID", nullptr));
        QTableWidgetItem *___qtablewidgetitem16 = issuesTable->horizontalHeaderItem(1);
        ___qtablewidgetitem16->setText(QCoreApplication::translate("MainWindow", "Employee", nullptr));
        QTableWidgetItem *___qtablewidgetitem17 = issuesTable->horizontalHeaderItem(2);
        ___qtablewidgetitem17->setText(QCoreApplication::translate("MainWindow", "Uniform Type", nullptr));
        QTableWidgetItem *___qtablewidgetitem18 = issuesTable->horizontalHeaderItem(3);
        ___qtablewidgetitem18->setText(QCoreApplication::translate("MainWindow", "Quantity", nullptr));
        QTableWidgetItem *___qtablewidgetitem19 = issuesTable->horizontalHeaderItem(4);
        ___qtablewidgetitem19->setText(QCoreApplication::translate("MainWindow", "Issue Date", nullptr));
        QTableWidgetItem *___qtablewidgetitem20 = issuesTable->horizontalHeaderItem(5);
        ___qtablewidgetitem20->setText(QCoreApplication::translate("MainWindow", "Return Date", nullptr));
        QTableWidgetItem *___qtablewidgetitem21 = issuesTable->horizontalHeaderItem(6);
        ___qtablewidgetitem21->setText(QCoreApplication::translate("MainWindow", "Status", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(issuesTab), QCoreApplication::translate("MainWindow", "Issue/Return", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
