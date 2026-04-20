#include "uniformdialog.h"
#include "ui_uniformdialog.h"
#include <QDate>

UniformDialog::UniformDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::UniformDialog)
{
    ui->setupUi(this);
    this->setWindowTitle("Uniform Information");
    ui->purchaseDateEdit->setDate(QDate::currentDate());
}

UniformDialog::~UniformDialog()
{
    delete ui;
}

Uniform UniformDialog::getUniform() const
{
    return m_uniform;
}

void UniformDialog::setUniform(const Uniform& uniform)
{
    m_uniform = uniform;
    ui->typeEdit->setText(uniform.getType());
    ui->quantitySpinBox->setValue(uniform.getQuantity());
    ui->sizeEdit->setText(uniform.getSize());
    ui->colorEdit->setText(uniform.getColor());
    ui->purchaseDateEdit->setDate(uniform.getPurchaseDate());
    ui->descriptionEdit->setPlainText(uniform.getDescription());
}

void UniformDialog::on_okBtn_clicked()
{
    m_uniform.setType(ui->typeEdit->text());
    m_uniform.setQuantity(ui->quantitySpinBox->value());
    m_uniform.setSize(ui->sizeEdit->text());
    m_uniform.setColor(ui->colorEdit->text());
    m_uniform.setPurchaseDate(ui->purchaseDateEdit->date());
    m_uniform.setDescription(ui->descriptionEdit->toPlainText());
    accept();
}

void UniformDialog::on_cancelBtn_clicked()
{
    reject();
}
