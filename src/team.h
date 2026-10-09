#ifndef TEAM_H
#define TEAM_H

#include <QString>
#include <QStringList>
#include "employee.h"

class Team
{
public:
    enum StaffingStatus {
        CriticallyUnderstaffed, // Less than 4 (cannot cover 4 tables)
        Understaffed,           // Less than targetStaff
        Optimal,                // Equal to targetStaff
        Overstaffed             // Greater than targetStaff
    };

    Team();
    Team(int id, const QString& name, const QString& uniformType = "Standard",
         int targetStaff = 5, int minStaff = 4, int maxStaff = 7,
         const QString& teamType = "Standard");

    // Getters
    int getId() const;
    QString getName() const;
    QString getUniformType() const;
    int getMinStaff() const;
    int getTargetStaff() const;
    int getMaxStaff() const;
    int getTableCount() const;
    QStringList getAssignedTables() const;
    QString getTeamType() const; // "Standard", "Game Show Host"

    bool isGameShowTeam() const;
    bool acceptsEmployee(const Employee& emp) const;

    // Setters
    void setId(int id);
    void setName(const QString& name);
    void setUniformType(const QString& uniformType);
    void setMinStaff(int minStaff);
    void setTargetStaff(int targetStaff);
    void setMaxStaff(int maxStaff);
    void setTeamType(const QString& teamType);

    // Staffing analysis
    StaffingStatus evaluateStaffing(int currentStaffCount) const;
    QString getStaffingStatusString(int currentStaffCount) const;
    bool needsStaff(int currentStaffCount) const;
    bool hasSurplusStaff(int currentStaffCount) const;

private:
    int m_id;
    QString m_name;
    QString m_uniformType;
    int m_minStaff;
    int m_targetStaff;
    int m_maxStaff;
    int m_tableCount; // Fixed at 4 tables per team
    QString m_teamType; // "Standard", "Game Show Host"
};

#endif // TEAM_H
