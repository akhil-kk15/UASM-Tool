/********************************************************************************
** Form generated from reading UI file 'issuedialog.ui'
**
** Created by: Qt User Interface Compiler version 6.11.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ISSUEDIALOG_H
#define UI_ISSUEDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDateEdit>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_IssueDialog
{
public:
    QVBoxLayout *verticalLayout;
    QFormLayout *formLayout;
    QLabel *label;
    QComboBox *employeeCombo;
    QLabel *label_2;
    QComboBox *uniformCombo;
    QLabel *label_3;
    QSpinBox *quantitySpinBox;
    QLabel *label_4;
    QDateEdit *issueDateEdit;
    QSpacerItem *verticalSpacer;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *okBtn;
    QPushButton *cancelBtn;

    void setupUi(QDialog *IssueDialog)
    {
        if (IssueDialog->objectName().isEmpty())
            IssueDialog->setObjectName("IssueDialog");
        IssueDialog->resize(400, 250);
        verticalLayout = new QVBoxLayout(IssueDialog);
        verticalLayout->setObjectName("verticalLayout");
        formLayout = new QFormLayout();
        formLayout->setObjectName("formLayout");
        label = new QLabel(IssueDialog);
        label->setObjectName("label");

        formLayout->setWidget(0, QFormLayout::ItemRole::LabelRole, label);

        employeeCombo = new QComboBox(IssueDialog);
        employeeCombo->setObjectName("employeeCombo");

        formLayout->setWidget(0, QFormLayout::ItemRole::FieldRole, employeeCombo);

        label_2 = new QLabel(IssueDialog);
        label_2->setObjectName("label_2");

        formLayout->setWidget(1, QFormLayout::ItemRole::LabelRole, label_2);

        uniformCombo = new QComboBox(IssueDialog);
        uniformCombo->setObjectName("uniformCombo");

        formLayout->setWidget(1, QFormLayout::ItemRole::FieldRole, uniformCombo);

        label_3 = new QLabel(IssueDialog);
        label_3->setObjectName("label_3");

        formLayout->setWidget(2, QFormLayout::ItemRole::LabelRole, label_3);

        quantitySpinBox = new QSpinBox(IssueDialog);
        quantitySpinBox->setObjectName("quantitySpinBox");
        quantitySpinBox->setValue(1);

        formLayout->setWidget(2, QFormLayout::ItemRole::FieldRole, quantitySpinBox);

        label_4 = new QLabel(IssueDialog);
        label_4->setObjectName("label_4");

        formLayout->setWidget(3, QFormLayout::ItemRole::LabelRole, label_4);

        issueDateEdit = new QDateEdit(IssueDialog);
        issueDateEdit->setObjectName("issueDateEdit");
        issueDateEdit->setCalendarPopup(true);

        formLayout->setWidget(3, QFormLayout::ItemRole::FieldRole, issueDateEdit);


        verticalLayout->addLayout(formLayout);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        okBtn = new QPushButton(IssueDialog);
        okBtn->setObjectName("okBtn");

        horizontalLayout->addWidget(okBtn);

        cancelBtn = new QPushButton(IssueDialog);
        cancelBtn->setObjectName("cancelBtn");

        horizontalLayout->addWidget(cancelBtn);


        verticalLayout->addLayout(horizontalLayout);


        retranslateUi(IssueDialog);

        QMetaObject::connectSlotsByName(IssueDialog);
    } // setupUi

    void retranslateUi(QDialog *IssueDialog)
    {
        IssueDialog->setWindowTitle(QCoreApplication::translate("IssueDialog", "Issue/Return Uniform", nullptr));
        label->setText(QCoreApplication::translate("IssueDialog", "Employee:", nullptr));
        label_2->setText(QCoreApplication::translate("IssueDialog", "Uniform Type:", nullptr));
        label_3->setText(QCoreApplication::translate("IssueDialog", "Quantity:", nullptr));
        label_4->setText(QCoreApplication::translate("IssueDialog", "Issue Date:", nullptr));
        okBtn->setText(QCoreApplication::translate("IssueDialog", "OK", nullptr));
        cancelBtn->setText(QCoreApplication::translate("IssueDialog", "Cancel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class IssueDialog: public Ui_IssueDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ISSUEDIALOG_H
