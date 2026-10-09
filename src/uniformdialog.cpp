#include "uniformdialog.h"
#include "ui_uniformdialog.h"

UniformDialog::UniformDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::UniformDialog)
{
    ui->setupUi(this);
    this->setWindowTitle("Uniform Information");
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
    ui->teamEdit->setText(uniform.getTeam());
    ui->quantitySpinBox->setValue(uniform.getQuantity());
    ui->sizeEdit->setText(uniform.getSize());
    ui->colorEdit->setText(uniform.getColor());
    ui->descriptionEdit->setPlainText(uniform.getDescription());
}

void UniformDialog::on_okBtn_clicked()
{
    m_uniform.setType(ui->typeEdit->text().trimmed());
    m_uniform.setTeam(ui->teamEdit->text().trimmed());
    m_uniform.setQuantity(ui->quantitySpinBox->value());
    m_uniform.setSize(ui->sizeEdit->text().trimmed());
    m_uniform.setColor(ui->colorEdit->text().trimmed());
    m_uniform.setDescription(ui->descriptionEdit->toPlainText().trimmed());
    accept();
}

void UniformDialog::on_cancelBtn_clicked()
{
    reject();
}
