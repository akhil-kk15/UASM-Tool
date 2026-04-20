/********************************************************************************
** Form generated from reading UI file 'employeededialog.ui'
**
** Created by: Qt User Interface Compiler version 6.11.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_EMPLOYEEDEDIALOG_H
#define UI_EMPLOYEEDEDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDateEdit>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_EmployeeDialog
{
public:
    QVBoxLayout *verticalLayout;
    QFormLayout *formLayout;
    QLabel *label;
    QLineEdit *nameEdit;
    QLabel *label_2;
    QLineEdit *designationEdit;
    QLabel *label_3;
    QLineEdit *departmentEdit;
    QLabel *label_4;
    QLineEdit *emailEdit;
    QLabel *label_5;
    QLineEdit *phoneEdit;
    QLabel *label_6;
    QComboBox *uniformSizeCombo;
    QLabel *label_7;
    QDateEdit *joiningDateEdit;
    QSpacerItem *verticalSpacer;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *okBtn;
    QPushButton *cancelBtn;

    void setupUi(QDialog *EmployeeDialog)
    {
        if (EmployeeDialog->objectName().isEmpty())
            EmployeeDialog->setObjectName("EmployeeDialog");
        EmployeeDialog->resize(400, 350);
        verticalLayout = new QVBoxLayout(EmployeeDialog);
        verticalLayout->setObjectName("verticalLayout");
        formLayout = new QFormLayout();
        formLayout->setObjectName("formLayout");
        label = new QLabel(EmployeeDialog);
        label->setObjectName("label");

        formLayout->setWidget(0, QFormLayout::ItemRole::LabelRole, label);

        nameEdit = new QLineEdit(EmployeeDialog);
        nameEdit->setObjectName("nameEdit");

        formLayout->setWidget(0, QFormLayout::ItemRole::FieldRole, nameEdit);

        label_2 = new QLabel(EmployeeDialog);
        label_2->setObjectName("label_2");

        formLayout->setWidget(1, QFormLayout::ItemRole::LabelRole, label_2);

        designationEdit = new QLineEdit(EmployeeDialog);
        designationEdit->setObjectName("designationEdit");

        formLayout->setWidget(1, QFormLayout::ItemRole::FieldRole, designationEdit);

        label_3 = new QLabel(EmployeeDialog);
        label_3->setObjectName("label_3");

        formLayout->setWidget(2, QFormLayout::ItemRole::LabelRole, label_3);

        departmentEdit = new QLineEdit(EmployeeDialog);
        departmentEdit->setObjectName("departmentEdit");

        formLayout->setWidget(2, QFormLayout::ItemRole::FieldRole, departmentEdit);

        label_4 = new QLabel(EmployeeDialog);
        label_4->setObjectName("label_4");

        formLayout->setWidget(3, QFormLayout::ItemRole::LabelRole, label_4);

        emailEdit = new QLineEdit(EmployeeDialog);
        emailEdit->setObjectName("emailEdit");

        formLayout->setWidget(3, QFormLayout::ItemRole::FieldRole, emailEdit);

        label_5 = new QLabel(EmployeeDialog);
        label_5->setObjectName("label_5");

        formLayout->setWidget(4, QFormLayout::ItemRole::LabelRole, label_5);

        phoneEdit = new QLineEdit(EmployeeDialog);
        phoneEdit->setObjectName("phoneEdit");

        formLayout->setWidget(4, QFormLayout::ItemRole::FieldRole, phoneEdit);

        label_6 = new QLabel(EmployeeDialog);
        label_6->setObjectName("label_6");

        formLayout->setWidget(5, QFormLayout::ItemRole::LabelRole, label_6);

        uniformSizeCombo = new QComboBox(EmployeeDialog);
        uniformSizeCombo->addItem(QString());
        uniformSizeCombo->addItem(QString());
        uniformSizeCombo->addItem(QString());
        uniformSizeCombo->addItem(QString());
        uniformSizeCombo->addItem(QString());
        uniformSizeCombo->addItem(QString());
        uniformSizeCombo->setObjectName("uniformSizeCombo");

        formLayout->setWidget(5, QFormLayout::ItemRole::FieldRole, uniformSizeCombo);

        label_7 = new QLabel(EmployeeDialog);
        label_7->setObjectName("label_7");

        formLayout->setWidget(6, QFormLayout::ItemRole::LabelRole, label_7);

        joiningDateEdit = new QDateEdit(EmployeeDialog);
        joiningDateEdit->setObjectName("joiningDateEdit");
        joiningDateEdit->setCalendarPopup(true);

        formLayout->setWidget(6, QFormLayout::ItemRole::FieldRole, joiningDateEdit);


        verticalLayout->addLayout(formLayout);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        okBtn = new QPushButton(EmployeeDialog);
        okBtn->setObjectName("okBtn");

        horizontalLayout->addWidget(okBtn);

        cancelBtn = new QPushButton(EmployeeDialog);
        cancelBtn->setObjectName("cancelBtn");

        horizontalLayout->addWidget(cancelBtn);


        verticalLayout->addLayout(horizontalLayout);


        retranslateUi(EmployeeDialog);

        QMetaObject::connectSlotsByName(EmployeeDialog);
    } // setupUi

    void retranslateUi(QDialog *EmployeeDialog)
    {
        EmployeeDialog->setWindowTitle(QCoreApplication::translate("EmployeeDialog", "Employee Information", nullptr));
        label->setText(QCoreApplication::translate("EmployeeDialog", "Name:", nullptr));
        label_2->setText(QCoreApplication::translate("EmployeeDialog", "Designation:", nullptr));
        label_3->setText(QCoreApplication::translate("EmployeeDialog", "Department:", nullptr));
        label_4->setText(QCoreApplication::translate("EmployeeDialog", "Email:", nullptr));
        label_5->setText(QCoreApplication::translate("EmployeeDialog", "Phone:", nullptr));
        label_6->setText(QCoreApplication::translate("EmployeeDialog", "Uniform Size:", nullptr));
        uniformSizeCombo->setItemText(0, QCoreApplication::translate("EmployeeDialog", "XS", nullptr));
        uniformSizeCombo->setItemText(1, QCoreApplication::translate("EmployeeDialog", "S", nullptr));
        uniformSizeCombo->setItemText(2, QCoreApplication::translate("EmployeeDialog", "M", nullptr));
        uniformSizeCombo->setItemText(3, QCoreApplication::translate("EmployeeDialog", "L", nullptr));
        uniformSizeCombo->setItemText(4, QCoreApplication::translate("EmployeeDialog", "XL", nullptr));
        uniformSizeCombo->setItemText(5, QCoreApplication::translate("EmployeeDialog", "XXL", nullptr));

        label_7->setText(QCoreApplication::translate("EmployeeDialog", "Joining Date:", nullptr));
        okBtn->setText(QCoreApplication::translate("EmployeeDialog", "OK", nullptr));
        cancelBtn->setText(QCoreApplication::translate("EmployeeDialog", "Cancel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class EmployeeDialog: public Ui_EmployeeDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_EMPLOYEEDEDIALOG_H
