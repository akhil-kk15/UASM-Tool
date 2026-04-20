/********************************************************************************
** Form generated from reading UI file 'uniformdialog.ui'
**
** Created by: Qt User Interface Compiler version 6.11.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_UNIFORMDIALOG_H
#define UI_UNIFORMDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDateEdit>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_UniformDialog
{
public:
    QVBoxLayout *verticalLayout;
    QFormLayout *formLayout;
    QLabel *label;
    QLineEdit *typeEdit;
    QLabel *label_2;
    QSpinBox *quantitySpinBox;
    QLabel *label_3;
    QLineEdit *sizeEdit;
    QLabel *label_4;
    QLineEdit *colorEdit;
    QLabel *label_5;
    QDateEdit *purchaseDateEdit;
    QLabel *label_51;
    QPlainTextEdit *descriptionEdit;
    QSpacerItem *verticalSpacer;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *okBtn;
    QPushButton *cancelBtn;

    void setupUi(QDialog *UniformDialog)
    {
        if (UniformDialog->objectName().isEmpty())
            UniformDialog->setObjectName("UniformDialog");
        UniformDialog->resize(400, 350);
        verticalLayout = new QVBoxLayout(UniformDialog);
        verticalLayout->setObjectName("verticalLayout");
        formLayout = new QFormLayout();
        formLayout->setObjectName("formLayout");
        label = new QLabel(UniformDialog);
        label->setObjectName("label");

        formLayout->setWidget(0, QFormLayout::ItemRole::LabelRole, label);

        typeEdit = new QLineEdit(UniformDialog);
        typeEdit->setObjectName("typeEdit");

        formLayout->setWidget(0, QFormLayout::ItemRole::FieldRole, typeEdit);

        label_2 = new QLabel(UniformDialog);
        label_2->setObjectName("label_2");

        formLayout->setWidget(1, QFormLayout::ItemRole::LabelRole, label_2);

        quantitySpinBox = new QSpinBox(UniformDialog);
        quantitySpinBox->setObjectName("quantitySpinBox");

        formLayout->setWidget(1, QFormLayout::ItemRole::FieldRole, quantitySpinBox);

        label_3 = new QLabel(UniformDialog);
        label_3->setObjectName("label_3");

        formLayout->setWidget(2, QFormLayout::ItemRole::LabelRole, label_3);

        sizeEdit = new QLineEdit(UniformDialog);
        sizeEdit->setObjectName("sizeEdit");

        formLayout->setWidget(2, QFormLayout::ItemRole::FieldRole, sizeEdit);

        label_4 = new QLabel(UniformDialog);
        label_4->setObjectName("label_4");

        formLayout->setWidget(3, QFormLayout::ItemRole::LabelRole, label_4);

        colorEdit = new QLineEdit(UniformDialog);
        colorEdit->setObjectName("colorEdit");

        formLayout->setWidget(3, QFormLayout::ItemRole::FieldRole, colorEdit);

        label_5 = new QLabel(UniformDialog);
        label_5->setObjectName("label_5");

        formLayout->setWidget(4, QFormLayout::ItemRole::LabelRole, label_5);

        purchaseDateEdit = new QDateEdit(UniformDialog);
        purchaseDateEdit->setObjectName("purchaseDateEdit");
        purchaseDateEdit->setCalendarPopup(true);

        formLayout->setWidget(4, QFormLayout::ItemRole::FieldRole, purchaseDateEdit);

        label_51 = new QLabel(UniformDialog);
        label_51->setObjectName("label_51");

        formLayout->setWidget(5, QFormLayout::ItemRole::LabelRole, label_51);

        descriptionEdit = new QPlainTextEdit(UniformDialog);
        descriptionEdit->setObjectName("descriptionEdit");
        descriptionEdit->setMaximumHeight(100);

        formLayout->setWidget(5, QFormLayout::ItemRole::FieldRole, descriptionEdit);


        verticalLayout->addLayout(formLayout);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        okBtn = new QPushButton(UniformDialog);
        okBtn->setObjectName("okBtn");

        horizontalLayout->addWidget(okBtn);

        cancelBtn = new QPushButton(UniformDialog);
        cancelBtn->setObjectName("cancelBtn");

        horizontalLayout->addWidget(cancelBtn);


        verticalLayout->addLayout(horizontalLayout);


        retranslateUi(UniformDialog);

        QMetaObject::connectSlotsByName(UniformDialog);
    } // setupUi

    void retranslateUi(QDialog *UniformDialog)
    {
        UniformDialog->setWindowTitle(QCoreApplication::translate("UniformDialog", "Uniform Information", nullptr));
        label->setText(QCoreApplication::translate("UniformDialog", "Type:", nullptr));
        label_2->setText(QCoreApplication::translate("UniformDialog", "Quantity:", nullptr));
        label_3->setText(QCoreApplication::translate("UniformDialog", "Size:", nullptr));
        label_4->setText(QCoreApplication::translate("UniformDialog", "Color:", nullptr));
        label_5->setText(QCoreApplication::translate("UniformDialog", "Purchase Date:", nullptr));
        label_51->setText(QCoreApplication::translate("UniformDialog", "Description:", nullptr));
        okBtn->setText(QCoreApplication::translate("UniformDialog", "OK", nullptr));
        cancelBtn->setText(QCoreApplication::translate("UniformDialog", "Cancel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class UniformDialog: public Ui_UniformDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_UNIFORMDIALOG_H
