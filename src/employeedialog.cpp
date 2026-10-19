#include "employeedialog.h"
#include "ui_employeededialog.h"

EmployeeDialog::EmployeeDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::EmployeeDialog)
{
    ui->setupUi(this);
    this->setWindowTitle("Employee Information");

    connect(ui->chkBlackjack, &QCheckBox::toggled, this, &EmployeeDialog::updateGameKnowledgeFromChecks);
    connect(ui->chkRoulette,  &QCheckBox::toggled, this, &EmployeeDialog::updateGameKnowledgeFromChecks);
    connect(ui->chkBaccarat,  &QCheckBox::toggled, this, &EmployeeDialog::updateGameKnowledgeFromChecks);
    connect(ui->chkPoker,     &QCheckBox::toggled, this, &EmployeeDialog::updateGameKnowledgeFromChecks);
    connect(ui->chkCraps,     &QCheckBox::toggled, this, &EmployeeDialog::updateGameKnowledgeFromChecks);
    connect(ui->chkGameShow,  &QCheckBox::toggled, this, &EmployeeDialog::updateGameKnowledgeFromChecks);

    updateGameKnowledgeFromChecks();
}

EmployeeDialog::~EmployeeDialog()
{
    delete ui;
}

void EmployeeDialog::updateGameKnowledgeFromChecks()
{
    QStringList selected;
    if (ui->chkBlackjack->isChecked()) selected << "Blackjack";
    if (ui->chkRoulette->isChecked())  selected << "Roulette";
    if (ui->chkBaccarat->isChecked())  selected << "Baccarat";
    if (ui->chkPoker->isChecked())     selected << "Poker";
    if (ui->chkCraps->isChecked())     selected << "Craps";
    if (ui->chkGameShow->isChecked())  selected << "Game Show";
    ui->gameKnowledgeEdit->setText(selected.join(", "));
}

Employee EmployeeDialog::getEmployee() const
{
    return m_employee;
}

void EmployeeDialog::setTeamList(const QStringList& teams)
{
    QString current = ui->teamCombo->currentText();
    ui->teamCombo->clear();
    for (const QString& t : teams) {
        if (!t.trimmed().isEmpty()) {
            ui->teamCombo->addItem(t.trimmed());
        }
    }
    if (!current.isEmpty()) {
        ui->teamCombo->setCurrentText(current);
    }
}

void EmployeeDialog::setEmployee(const Employee& employee)
{
    m_employee = employee;
    ui->nameEdit->setText(employee.getName());
    ui->teamCombo->setCurrentText(employee.getTeam());
    ui->designationEdit->setText(employee.getDesignation());
    ui->emailEdit->setText(employee.getEmail());
    ui->phoneEdit->setText(employee.getPhone());

    int sizeIndex = ui->uniformSizeCombo->findText(employee.getUniformSize());
    if (sizeIndex >= 0) {
        ui->uniformSizeCombo->setCurrentIndex(sizeIndex);
    } else {
        ui->uniformSizeCombo->setCurrentText(employee.getUniformSize().isEmpty() ? "M" : employee.getUniformSize());
    }

    QString role = employee.getRoleType();
    int roleIndex = ui->roleCombo->findText(role);
    if (roleIndex >= 0) {
        ui->roleCombo->setCurrentIndex(roleIndex);
    } else {
        ui->roleCombo->setCurrentText(role.isEmpty() ? "Dealer" : role);
    }

    QString games = employee.getGameKnowledge();
    ui->chkBlackjack->blockSignals(true);
    ui->chkRoulette->blockSignals(true);
    ui->chkBaccarat->blockSignals(true);
    ui->chkPoker->blockSignals(true);
    ui->chkCraps->blockSignals(true);
    ui->chkGameShow->blockSignals(true);

    ui->chkBlackjack->setChecked(games.contains("Blackjack", Qt::CaseInsensitive));
    ui->chkRoulette->setChecked(games.contains("Roulette", Qt::CaseInsensitive));
    ui->chkBaccarat->setChecked(games.contains("Baccarat", Qt::CaseInsensitive));
    ui->chkPoker->setChecked(games.contains("Poker", Qt::CaseInsensitive));
    ui->chkCraps->setChecked(games.contains("Craps", Qt::CaseInsensitive));
    ui->chkGameShow->setChecked(games.contains("Game Show", Qt::CaseInsensitive));

    ui->chkBlackjack->blockSignals(false);
    ui->chkRoulette->blockSignals(false);
    ui->chkBaccarat->blockSignals(false);
    ui->chkPoker->blockSignals(false);
    ui->chkCraps->blockSignals(false);
    ui->chkGameShow->blockSignals(false);

    ui->gameKnowledgeEdit->setText(games.isEmpty() ? "Blackjack, Roulette" : games);
}

void EmployeeDialog::on_okBtn_clicked()
{
    m_employee.setName(ui->nameEdit->text().trimmed());
    m_employee.setTeam(ui->teamCombo->currentText().trimmed());
    m_employee.setRoleType(ui->roleCombo->currentText());
    m_employee.setDesignation(ui->designationEdit->text().trimmed());
    m_employee.setEmail(ui->emailEdit->text().trimmed());
    m_employee.setPhone(ui->phoneEdit->text().trimmed());
    m_employee.setUniformSize(ui->uniformSizeCombo->currentText());
    m_employee.setGameKnowledge(ui->gameKnowledgeEdit->text().trimmed());
    accept();
}

void EmployeeDialog::on_cancelBtn_clicked()
{
    reject();
}
