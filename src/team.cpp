#include "team.h"

Team::Team()
    : m_id(-1), m_name(""), m_uniformType("Standard"),
      m_minStaff(4), m_targetStaff(5), m_maxStaff(7), m_tableCount(4)
{
}

Team::Team(int id, const QString& name, const QString& uniformType,
           int targetStaff, int minStaff, int maxStaff)
    : m_id(id), m_name(name), m_uniformType(uniformType),
      m_minStaff(minStaff), m_targetStaff(targetStaff), m_maxStaff(maxStaff),
      m_tableCount(4)
{
}

int Team::getId() const { return m_id; }
QString Team::getName() const { return m_name; }
QString Team::getUniformType() const { return m_uniformType; }
int Team::getMinStaff() const { return m_minStaff; }
int Team::getTargetStaff() const { return m_targetStaff; }
int Team::getMaxStaff() const { return m_maxStaff; }
int Team::getTableCount() const { return m_tableCount; }

QStringList Team::getAssignedTables() const
{
    QStringList tables;
    for (int i = 1; i <= m_tableCount; ++i) {
        tables << QString("%1 - Table %2").arg(m_name).arg(i);
    }
    return tables;
}

void Team::setId(int id) { m_id = id; }
void Team::setName(const QString& name) { m_name = name; }
void Team::setUniformType(const QString& uniformType) { m_uniformType = uniformType; }
void Team::setMinStaff(int minStaff) { m_minStaff = minStaff; }
void Team::setTargetStaff(int targetStaff) { m_targetStaff = targetStaff; }
void Team::setMaxStaff(int maxStaff) { m_maxStaff = maxStaff; }

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
