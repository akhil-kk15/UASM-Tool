#include "team.h"

Team::Team()
    : m_id(-1), m_name(""), m_uniformType("Standard"),
      m_minStaff(5), m_targetStaff(7), m_maxStaff(7), m_tableCount(4),
      m_teamType("Standard"), m_tableGames("")
{
}

Team::Team(int id, const QString& name, const QString& uniformType,
           int targetStaff, int minStaff, int maxStaff,
           const QString& teamType, const QString& tableGames)
    : m_id(id), m_name(name), m_uniformType(uniformType),
      m_minStaff(minStaff), m_targetStaff(targetStaff), m_maxStaff(maxStaff),
      m_tableCount(4),
      m_teamType(teamType.isEmpty() ? "Standard" : teamType),
      m_tableGames(tableGames)
{
}

int Team::getId() const { return m_id; }
QString Team::getName() const { return m_name; }
QString Team::getUniformType() const { return m_uniformType; }
int Team::getMinStaff() const { return m_minStaff; }
int Team::getTargetStaff() const { return m_targetStaff; }
int Team::getMaxStaff() const { return m_maxStaff; }
int Team::getTableCount() const { return m_tableCount; }
QString Team::getTeamType() const { return m_teamType; }
QString Team::getTableGames() const
{
    if (!m_tableGames.trimmed().isEmpty()) {
        return m_tableGames;
    }
    return isGameShowTeam()
        ? QStringLiteral("Game Show, Game Show, Game Show, Game Show")
        : QStringLiteral("Blackjack, Blackjack, Blackjack, Blackjack");
}

QStringList Team::getTableGamesList() const
{
    QStringList list;
    const QStringList parts = getTableGames().split(',', Qt::SkipEmptyParts);
    for (const QString& p : parts) {
        QString trimmed = p.trimmed();
        if (!trimmed.isEmpty()) {
            list.append(trimmed);
        }
    }
    QString fallback = isGameShowTeam() ? QStringLiteral("Game Show") : QStringLiteral("Blackjack");
    while (list.size() < m_tableCount) {
        list.append(list.isEmpty() ? fallback : list.last());
    }
    return list.mid(0, m_tableCount);
}

QStringList Team::getUniqueGames() const
{
    QStringList unique;
    for (const QString& g : getTableGamesList()) {
        bool exists = false;
        for (const QString& u : unique) {
            if (u.compare(g, Qt::CaseInsensitive) == 0) {
                exists = true;
                break;
            }
        }
        if (!exists) {
            unique.append(g);
        }
    }
    return unique;
}

bool Team::isGameShowTeam() const
{
    QString t = m_teamType.trimmed().toLower();
    return (t.contains("game show") || t.contains("host"));
}

bool Team::hasCompatibleRole(const Employee& emp) const
{
    if (isGameShowTeam()) {
        return emp.canHostGameShow();
    } else {
        return emp.canDeal();
    }
}

QStringList Team::getMissingGames(const Employee& emp) const
{
    QStringList missing;
    if (emp.getGameKnowledge().trimmed().isEmpty()) {
        return missing;
    }

    if (isGameShowTeam()) {
        if (!emp.canHostGameShow() && !emp.knowsGame("Game Show")) {
            missing.append("Game Show");
        }
        return missing;
    }

    for (const QString& game : getUniqueGames()) {
        if (!emp.knowsGame(game)) {
            missing.append(game);
        }
    }
    return missing;
}

bool Team::hasRequiredGameKnowledge(const Employee& emp) const
{
    return getMissingGames(emp).isEmpty();
}

bool Team::acceptsEmployee(const Employee& emp) const
{
    return hasCompatibleRole(emp) && hasRequiredGameKnowledge(emp);
}

QStringList Team::getAssignedTables() const
{
    QStringList tables;
    QStringList games = getTableGamesList();
    for (int i = 0; i < m_tableCount; ++i) {
        tables << QString("Table %1 (%2)").arg(i + 1).arg(games.value(i, "Blackjack"));
    }
    return tables;
}

void Team::setId(int id) { m_id = id; }
void Team::setName(const QString& name) { m_name = name; }
void Team::setUniformType(const QString& uniformType) { m_uniformType = uniformType; }
void Team::setMinStaff(int minStaff) { m_minStaff = minStaff; }
void Team::setTargetStaff(int targetStaff) { m_targetStaff = targetStaff; }
void Team::setMaxStaff(int maxStaff) { m_maxStaff = maxStaff; }
void Team::setTeamType(const QString& teamType) { m_teamType = teamType; }
void Team::setTableGames(const QString& tableGames) { m_tableGames = tableGames; }

Team::StaffingStatus Team::evaluateStaffing(int currentStaffCount) const
{
    if (currentStaffCount < m_minStaff) {
        return CriticallyUnderstaffed;
    } else if (currentStaffCount < m_targetStaff) {
        return Understaffed;
    } else if (currentStaffCount == m_targetStaff) {
        return Optimal;
    } else {
        return Overstaffed;
    }
}

QString Team::getStaffingStatusString(int currentStaffCount) const
{
    StaffingStatus status = evaluateStaffing(currentStaffCount);
    switch (status) {
        case CriticallyUnderstaffed:
            return QString("Critically Understaffed (%1/%2)").arg(currentStaffCount).arg(m_targetStaff);
        case Understaffed:
            return QString("Understaffed (%1/%2)").arg(currentStaffCount).arg(m_targetStaff);
        case Optimal:
            return QString("Optimal (%1/%2)").arg(currentStaffCount).arg(m_targetStaff);
        case Overstaffed:
            return QString("Overstaffed (%1/%2)").arg(currentStaffCount).arg(m_targetStaff);
    }
    return "Unknown";
}

bool Team::needsStaff(int currentStaffCount) const
{
    return currentStaffCount < m_targetStaff;
}

bool Team::hasSurplusStaff(int currentStaffCount) const
{
    return currentStaffCount > m_targetStaff;
}
