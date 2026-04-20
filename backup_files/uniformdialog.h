#ifndef UNIFORMDIALOG_H
#define UNIFORMDIALOG_H

#include <QDialog>
#include "uniform.h"

namespace Ui {
class UniformDialog;
}

class UniformDialog : public QDialog
{
    Q_OBJECT

public:
    explicit UniformDialog(QWidget *parent = nullptr);
    ~UniformDialog();

    Uniform getUniform() const;
    void setUniform(const Uniform& uniform);

private slots:
    void on_okBtn_clicked();
    void on_cancelBtn_clicked();

private:
    Ui::UniformDialog *ui;
    Uniform m_uniform;
};

#endif // UNIFORMDIALOG_H
