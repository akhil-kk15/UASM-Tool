#include "rotationscheduler.h"
#include "databasemanager.h"
#include <QVector>
#include <QDebug>

RotationScheduler::RotationScheduler(DatabaseManager* dbManager)
    : m_dbManager(dbManager)
{
}

void RotationScheduler::setDatabaseManager(DatabaseManager* dbManager)
{
    m_dbManager = dbManager;
}

ShiftSchedule RotationScheduler::generate8HourSchedule(const Team& team,
                                                       const QList<Employee>& members,
                                                       const QDate& date,
                                                       const QTime& startTime,
                                                       int slotMinutes,
                                                       int numSlots)
{
    ShiftSchedule schedule;
    schedule.teamName = team.getName();
    schedule.shiftDate = date;
    schedule.shiftStartTime = startTime;
    schedule.shiftDurationHours = 8;
    schedule.slotMinutes = slotMinutes > 0 ? slotMinutes : 30;

    // 16 tables per employee = 4 rounds of 4 tables (with Table 5 break) -> 5 slots per round x 4 rounds = 20 slots.
    if (numSlots <= 0) {
        if (schedule.slotMinutes == 30 || schedule.slotMinutes == 40) {
            numSlots = 20;
        } else {
            numSlots = 8;
        }
    }

    int numEmployees = members.size();
    if (numEmployees == 0) {
        schedule.statusNotes = "No employees assigned to team. Cannot generate rotation.";
        return schedule;
    }

    // Each team has 4 tables
    const int tableCount = 4;

    if (numEmployees < tableCount) {
        schedule.statusNotes = QString("CRITICALLY UNDERSTAFFED: Team has only %1 employee(s) for 4 tables. "
                                       "Tables cannot all be staffed. Additional staff required from overstaffed teams.")
                               .arg(numEmployees);
    } else if (numEmployees == tableCount) {
        schedule.statusNotes = "UNDERSTAFFED: Exactly 4 employees for 4 tables. "
                               "No relief staff available for breaks without taking tables offline. "
                               "Recommended target: 5-6 employees.";
    } else {
        schedule.statusNotes = QString("16 Tables per Employee: 4 rounds of 4 tables with Table 5 as break. Rotation Type: %1 Mins. Staffing: %2 employees.")
                               .arg(schedule.slotMinutes).arg(numEmployees);
    }

    // Consecutive tables attended counter for each employee index
    QVector<int> consecutiveWorked(numEmployees, 0);

    for (int s = 0; s < numSlots; ++s) {
        RotationSlot slot;
        slot.slotIndex = s;
        slot.startTime = startTime.addSecs(s * schedule.slotMinutes * 60);
        slot.endTime = startTime.addSecs((s + 1) * schedule.slotMinutes * 60);

        if (numEmployees >= 5) {
            // Elegant cyclical round-robin rotation:
            // For N employees and 4 tables, in slot s:
            // Break employee is offset by s % N (or multiple if N > 5)
            // Each employee works 4 consecutive slots, then takes 1 slot break!
            int breakOffset = s % numEmployees;
            int numBreaks = numEmployees - tableCount;

            QList<int> breakIndices;
            for (int b = 0; b < numBreaks; ++b) {
                breakIndices.append((breakOffset + b) % numEmployees);
            }

            // Assign break employees
            for (int bIdx : breakIndices) {
                slot.onBreakEmployeeIds.append(members[bIdx].getId());
                slot.onBreakEmployeeNames.append(members[bIdx].getName());
            }

            // Assign remaining to the 4 tables
            int tableNum = 1;
            for (int i = 0; i < numEmployees && tableNum <= tableCount; ++i) {
                int empIdx = (breakOffset + numBreaks + i) % numEmployees;
                if (!breakIndices.contains(empIdx)) {
                    TableAssignment assign;
                    assign.tableNumber = tableNum;
                    assign.employeeId = members[empIdx].getId();
                    assign.employeeName = members[empIdx].getName();
                    slot.tableAssignments.append(assign);
                    tableNum++;
                }
            }
        } else if (numEmployees == 4) {
            // 4 employees for 4 tables: all work tables, rotate table numbers
            for (int t = 1; t <= tableCount; ++t) {
                int empIdx = (s + t - 1) % numEmployees;
                TableAssignment assign;
                assign.tableNumber = t;
                assign.employeeId = members[empIdx].getId();
                assign.employeeName = members[empIdx].getName();
                slot.tableAssignments.append(assign);
            }
            // After every 4 slots, note that break is due
            if ((s + 1) % 4 == 0) {
                slot.onBreakEmployeeNames.append("Relief Needed (All 4 staff reached 4 tables)");
            }
        } else {
            // Less than 4 employees: assign as many tables as possible
            for (int i = 0; i < numEmployees; ++i) {
                TableAssignment assign;
                assign.tableNumber = i + 1;
                assign.employeeId = members[i].getId();
                assign.employeeName = members[i].getName();
                slot.tableAssignments.append(assign);
            }
            for (int unstaffed = numEmployees + 1; unstaffed <= tableCount; ++unstaffed) {
                TableAssignment assign;
                assign.tableNumber = unstaffed;
                assign.employeeId = -1;
                assign.employeeName = "[UNATTENDED - UNDERSTAFFED]";
                slot.tableAssignments.append(assign);
            }
        }

        schedule.rotationSlots.append(slot);
    }

    return schedule;
}

ReassignmentResult RotationScheduler::reassignEmployee(int employeeId,
                                                      const QString& requestedTargetTeam,
                                                      const QString& reason)
{
    ReassignmentResult result;
    result.success = false;
    result.employeeId = employeeId;
    result.requestedTeam = requestedTargetTeam;
    result.fallbackUsed = false;

    if (!m_dbManager) {
        result.message = "Database manager not initialized.";
        return result;
    }

    Employee emp = m_dbManager->getEmployee(employeeId);
    if (emp.getId() <= 0) {
        result.message = QString("Employee ID %1 not found.").arg(employeeId);
        return result;
    }

    result.employeeName = emp.getName();
    result.employeeSize = emp.getUniformSize().isEmpty() ? "M" : emp.getUniformSize();
    result.sourceTeam = emp.getDepartment();

    if (result.sourceTeam == requestedTargetTeam) {
        result.message = QString("Employee is already assigned to %1.").arg(requestedTargetTeam);
        return result;
    }

    Team targetTeam = m_dbManager->getTeamByName(requestedTargetTeam);
    if (targetTeam.getId() <= 0) {
        result.message = QString("Requested team '%1' does not exist.").arg(requestedTargetTeam);
        return result;
    }

    // Role compatibility check for requested target team
    if (!targetTeam.acceptsEmployee(emp)) {
        if (targetTeam.isGameShowTeam()) {
            result.message = QString("Cannot reassign %1 (Role: '%2') to %3: This team is dedicated to Game Show Hosts.")
                             .arg(emp.getName(), emp.getRoleType(), requestedTargetTeam);
        } else {
            result.message = QString("Cannot reassign %1 (Role: '%2') to %3: This team is a Standard Dealer team.")
                             .arg(emp.getName(), emp.getRoleType(), requestedTargetTeam);
        }
        return result;
    }

    // 1. Check if uniform in employee size is available for requested target team
    int availForRequested = m_dbManager->getAvailableUniformCountForTeam(
        targetTeam.getName(), targetTeam.getUniformType(), result.employeeSize);

    if (availForRequested > 0) {
        // Uniform is available! Reassign to requested team
        Uniform uni = m_dbManager->findAvailableUniformForTeam(
            targetTeam.getName(), targetTeam.getUniformType(), result.employeeSize);

        if (m_dbManager->updateEmployeeTeam(employeeId, requestedTargetTeam)) {
            // Allocate uniform
            if (uni.getId() > 0) {
                UniformIssue issue;
                issue.setEmployeeId(employeeId);
                issue.setUniformId(uni.getId());
                issue.setQuantity(1);
                issue.setIssueDate(QDate::currentDate());
                issue.setStatus("Issued");
                m_dbManager->issueUniform(issue);
            }

            // Adjust rotations for both teams
            adjustTeamRotation(result.sourceTeam);
            adjustTeamRotation(requestedTargetTeam);

            result.success = true;
            result.assignedTeam = requestedTargetTeam;
            result.requestedTeamHadUniform = true;
            result.fallbackUsed = false;
            result.message = QString("Employee %1 successfully assigned to %2. Uniform issued (Type: %3, Size: %4). Team rotations adjusted.")
                             .arg(result.employeeName, requestedTargetTeam, uni.getType(), result.employeeSize);
            return result;
        } else {
            result.message = "Failed to update employee team in database: " + m_dbManager->getLastError();
            return result;
        }
    }

    // 2. Uniform is NOT available for requested team! Fallback redirection
    result.requestedTeamHadUniform = false;

    QList<Team> allTeams = m_dbManager->getAllTeams();
    Team fallbackTeam;
    int bestScore = -1; // Higher score = better candidate

    for (const Team& candidate : allTeams) {
        if (candidate.getName() == result.sourceTeam || candidate.getName() == requestedTargetTeam) {
            continue;
        }

        // Candidate team must accept this employee based on role
        if (!candidate.acceptsEmployee(emp)) {
            continue;
        }

        int availCount = m_dbManager->getAvailableUniformCountForTeam(
            candidate.getName(), candidate.getUniformType(), result.employeeSize);

        if (availCount > 0) {
            int currentStaff = m_dbManager->getTeamMemberCount(candidate.getName());
            int score = 0;

            if (currentStaff < candidate.getMinStaff()) {
                score = 100; // Critically understaffed - highest priority
            } else if (currentStaff < candidate.getTargetStaff()) {
                score = 50;  // Understaffed - high priority
            } else if (currentStaff < candidate.getMaxStaff()) {
                score = 10;  // Can take more staff
            } else {
                continue; // At capacity
            }

            if (score > bestScore) {
                bestScore = score;
                fallbackTeam = candidate;
            }
        }
    }

    if (fallbackTeam.getId() > 0) {
        // Fallback team found with available uniform
        Uniform fallbackUni = m_dbManager->findAvailableUniformForTeam(
            fallbackTeam.getName(), fallbackTeam.getUniformType(), result.employeeSize);

        if (m_dbManager->updateEmployeeTeam(employeeId, fallbackTeam.getName())) {
            if (fallbackUni.getId() > 0) {
                UniformIssue issue;
                issue.setEmployeeId(employeeId);
                issue.setUniformId(fallbackUni.getId());
                issue.setQuantity(1);
                issue.setIssueDate(QDate::currentDate());
                issue.setStatus("Issued");
                m_dbManager->issueUniform(issue);
            }

            adjustTeamRotation(result.sourceTeam);
            adjustTeamRotation(fallbackTeam.getName());

            result.success = true;
            result.assignedTeam = fallbackTeam.getName();
            result.fallbackUsed = true;
            result.fallbackReason = QString("No uniform was available in office inventory for %1 in size '%2'. "
                                            "System redirected %3 to %4 which has available uniform stock (Type: %5, Size: %2).")
                                    .arg(requestedTargetTeam, result.employeeSize, result.employeeName,
                                         fallbackTeam.getName(), fallbackUni.getType());
            result.message = result.fallbackReason + " Rotations for both teams have been automatically adjusted.";
            return result;
        } else {
            result.message = "Failed to update employee team for fallback: " + m_dbManager->getLastError();
            return result;
        }
    }

    // No team has an available uniform in the employee's size
    result.success = false;
    result.message = QString("Cannot reassign %1 to %2: No uniform available in size '%3' for %2, "
                             "and no compatible alternative team has available uniforms in this size in office inventory!")
                     .arg(result.employeeName, requestedTargetTeam, result.employeeSize);
    return result;
}

ShiftSchedule RotationScheduler::adjustTeamRotation(const QString& teamName,
                                                    const QDate& date,
                                                    const QTime& startTime,
                                                    int slotMinutes,
                                                    int numSlots)
{
    if (!m_dbManager) return ShiftSchedule();

    Team team = m_dbManager->getTeamByName(teamName);
    if (team.getId() <= 0) return ShiftSchedule();

    QList<Employee> members = m_dbManager->getEmployeesByTeam(teamName);
    ShiftSchedule schedule = generate8HourSchedule(team, members, date, startTime, slotMinutes, numSlots);

    // Persist to database
    m_dbManager->clearShiftSchedule(teamName, date);
    for (const RotationSlot& slot : schedule.rotationSlots) {
        QStringList assignStrings;
        for (const TableAssignment& assign : slot.tableAssignments) {
            assignStrings << QString("Table %1: %2").arg(assign.tableNumber).arg(assign.employeeName);
        }
        m_dbManager->saveScheduleSlot(teamName, date, slot.slotIndex,
                                     slot.startTime.toString("hh:mm"),
                                     slot.endTime.toString("hh:mm"),
                                     assignStrings.join(" | "),
                                     slot.onBreakEmployeeNames.join(", "));
    }

    return schedule;
}
