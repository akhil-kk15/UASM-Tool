#ifndef ROTATIONSCHEDULER_H
#define ROTATIONSCHEDULER_H

#include <QString>
#include <QStringList>
#include <QList>
#include <QDate>
#include <QTime>
#include "employee.h"
#include "team.h"
#include "uniform.h"

struct TableAssignment {
    int tableNumber;        // 1 to 4
    int employeeId;
    QString employeeName;
};

struct RotationSlot {
    int slotIndex;
    QTime startTime;
    QTime endTime;
    QList<TableAssignment> tableAssignments; // 4 tables
    QList<int> onBreakEmployeeIds;
    QStringList onBreakEmployeeNames;        // Table 5 (30-Min Break)
};

struct ShiftSchedule {
    QString teamName;
    QDate shiftDate;
    QTime shiftStartTime;
    int shiftDurationHours; // Standard: 8 hours
    int slotMinutes;        // Standard: 30 minutes
    QList<RotationSlot> rotationSlots;
    QString statusNotes;
};

struct ReassignmentResult {
    bool success;
    int employeeId;
    QString employeeName;
    QString employeeSize;
    QString sourceTeam;
    QString requestedTeam;
    QString assignedTeam;
    bool requestedTeamHadUniform;
    bool fallbackUsed;
    QString fallbackReason;
    QString message;
};

class DatabaseManager; // Forward declaration

class RotationScheduler
{
public:
    RotationScheduler(DatabaseManager* dbManager = nullptr);

    void setDatabaseManager(DatabaseManager* dbManager);

    // Schedule generation: 16 tables per employee across 4 rounds of 2 hours, Table 5 is a 30-min break
    ShiftSchedule generate8HourSchedule(const Team& team,
                                       const QList<Employee>& members,
                                       const QDate& date = QDate::currentDate(),
                                       const QTime& startTime = QTime(8, 0),
                                       int slotMinutes = 30,
                                       int numSlots = 20);

    // Reassignment with uniform availability checking & fallback redirection
    ReassignmentResult reassignEmployee(int employeeId,
                                        const QString& requestedTargetTeam,
                                        const QString& reason = "Staffing adjustment");

    // Dynamic rotation adjustment helper
    ShiftSchedule adjustTeamRotation(const QString& teamName,
                                     const QDate& date = QDate::currentDate(),
                                     const QTime& startTime = QTime(8, 0),
                                     int slotMinutes = 30,
                                     int numSlots = 20);

private:
    DatabaseManager* m_dbManager;
};

#endif // ROTATIONSCHEDULER_H
