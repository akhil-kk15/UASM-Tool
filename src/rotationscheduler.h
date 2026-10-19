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
    QString tableGame;      // e.g. "Blackjack", "Roulette", "Baccarat"
    int employeeId;
    QString employeeName;
};

struct RotationSlot {
    int slotIndex;
    QTime startTime;
    QTime endTime;
    QList<TableAssignment> tableAssignments; // 4 tables
    QList<int> onBreakEmployeeIds;
    QStringList onBreakEmployeeNames;        // Table 5 (Break)
};

struct ShiftSchedule {
    QString teamName;
    QStringList tableGames; // Games for Tables 1..4
    QDate shiftDate;
    QTime shiftStartTime;
    int shiftDurationHours; // Standard: 8 hours
    int slotMinutes;        // Strictly 30, 40, or 20 (dire) minutes
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
    static constexpr int MAX_SHIFT_STAFF = 7;

    RotationScheduler(DatabaseManager* dbManager = nullptr);

    void setDatabaseManager(DatabaseManager* dbManager);

    // Strictly recommends 30, 40, or 20 (dire) minutes based on staffing
    int recommendSlotMinutes(const Team& team, int staffCount) const;

    // Enforces that slotMinutes is strictly 30, 40, or 20 minutes
    int normalizeSlotMinutes(int requestedMinutes, const Team& team, int staffCount) const;

    // Schedule generation: up to 7 employees in an 8-hour shift, 4 tables + break
    ShiftSchedule generate8HourSchedule(const Team& team,
                                       const QList<Employee>& members,
                                       const QDate& date = QDate::currentDate(),
                                       const QTime& startTime = QTime(8, 0),
                                       int slotMinutes = 30,
                                       int numSlots = 20);

    // Reassignment with uniform availability, 7-staff shift cap, and game knowledge checking
    ReassignmentResult reassignEmployee(int employeeId,
                                        const QString& requestedTargetTeam,
                                        const QString& reason = "Staffing adjustment");

    // Automatically relocates any employees exceeding the 7-person shift limit to compatible teams
    QList<ReassignmentResult> relocateOverflowStaff(const QString& teamName = "");

    // Dynamic rotation adjustment helper (auto-selects 30/40/20 mins when slotMinutes <= 0)
    ShiftSchedule adjustTeamRotation(const QString& teamName,
                                     const QDate& date = QDate::currentDate(),
                                     const QTime& startTime = QTime(8, 0),
                                     int slotMinutes = 0,
                                     int numSlots = 20);

private:
    DatabaseManager* m_dbManager;
};

#endif // ROTATIONSCHEDULER_H
