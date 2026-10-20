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

int RotationScheduler::recommendSlotMinutes(const Team& team, int staffCount) const
{
    Q_UNUSED(team);
    // Dire situation (critically understaffed: 4 or fewer staff for 4 tables) -> 20 minutes
    if (staffCount <= 4) {
        return 20;
    }
    // Understaffed (5 staff) or Overstaffed (> 7 staff) -> 40 minutes
    if (staffCount == 5 || staffCount > MAX_SHIFT_STAFF) {
        return 40;
    }
    // Standard optimal staffing (6 to 7 staff) -> 30 minutes
    return 30;
}

int RotationScheduler::normalizeSlotMinutes(int requestedMinutes, const Team& team, int staffCount) const
{
    if (requestedMinutes <= 0) {
        return recommendSlotMinutes(team, staffCount);
    }
    if (requestedMinutes == 20 || requestedMinutes == 30 || requestedMinutes == 40) {
        return requestedMinutes;
    }
    // Strictly clamp any other value to {20, 30, 40}
    if (requestedMinutes < 25) {
        return 20;
    }
    if (requestedMinutes >= 35) {
        return 40;
    }
    return 30;
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
    schedule.tableGames = team.getTableGamesList();
    schedule.shiftDate = date;
    schedule.shiftStartTime = startTime;
    schedule.shiftDurationHours = 8;

    int totalAssigned = members.size();
    // Enforce the 8-hour shift limit of at most 7 employees per team
    QList<Employee> shiftMembers = members.mid(0, MAX_SHIFT_STAFF);
    int numEmployees = shiftMembers.size();

    schedule.slotMinutes = normalizeSlotMinutes(slotMinutes, team, totalAssigned);

    // 16 tables per employee = 4 rounds of 4 tables (with Table 5 break) -> 5 slots per round x 4 rounds = 20 slots.
    if (numSlots <= 0) {
        numSlots = 20;
    }

    if (numEmployees == 0) {
        schedule.statusNotes = "No employees assigned to team. Cannot generate rotation.";
        return schedule;
    }

    // Each team has 4 tables
    const int tableCount = 4;

    if (numEmployees < tableCount) {
        schedule.statusNotes = QString("DIRE UNDERSTAFFING (%1/7): Only %2 employee(s) for 4 tables. "
                                       "Rotation adjusted to %3 Mins (Dire Mode). Additional staff required.")
                               .arg(numEmployees).arg(numEmployees).arg(schedule.slotMinutes);
    } else if (numEmployees == tableCount) {
        schedule.statusNotes = QString("DIRE UNDERSTAFFING (4/7): Exactly 4 employees for 4 tables (no break relief). "
                                       "Rotation adjusted to %1 Mins (Dire Mode). Max 7 staff per shift.")
                               .arg(schedule.slotMinutes);
    } else if (totalAssigned > MAX_SHIFT_STAFF) {
        schedule.statusNotes = QString("OVERSTAFFED (%1/7): Capped shift rotation at 7 staff (%2 Overflow should be placed in compatible teams). "
                                       "Rotation Type: %3 Mins | Games: %4.")
                               .arg(totalAssigned).arg(totalAssigned - MAX_SHIFT_STAFF)
                               .arg(schedule.slotMinutes).arg(team.getUniqueGames().join(", "));
    } else if (numEmployees == 5) {
        schedule.statusNotes = QString("UNDERSTAFFED (5/7): 4 tables active + 1 on break. "
                                       "Rotation Type: %1 Mins | Games: %2.")
                               .arg(schedule.slotMinutes).arg(team.getUniqueGames().join(", "));
    } else {
        schedule.statusNotes = QString("OPTIMAL (%1/7 Max): 4 tables active + %2 on break. "
                                       "Rotation Type: %3 Mins | Games: %4.")
                               .arg(numEmployees).arg(numEmployees - tableCount)
                               .arg(schedule.slotMinutes).arg(team.getUniqueGames().join(", "));
    }

    for (int s = 0; s < numSlots; ++s) {
        RotationSlot slot;
        slot.slotIndex = s;
        slot.startTime = startTime.addSecs(s * schedule.slotMinutes * 60);
        slot.endTime = startTime.addSecs((s + 1) * schedule.slotMinutes * 60);

        if (numEmployees >= 5) {
            // Cyclical round-robin rotation for 5..7 employees across 4 tables:
            int breakOffset = s % numEmployees;
            int numBreaks = numEmployees - tableCount;

            QList<int> breakIndices;
            for (int b = 0; b < numBreaks; ++b) {
                breakIndices.append((breakOffset + b) % numEmployees);
            }

            // Assign break employees
            for (int bIdx : breakIndices) {
                slot.onBreakEmployeeIds.append(shiftMembers[bIdx].getId());
                slot.onBreakEmployeeNames.append(shiftMembers[bIdx].getName());
            }

            // Assign remaining to the 4 tables
            int tableNum = 1;
            for (int i = 0; i < numEmployees && tableNum <= tableCount; ++i) {
                int empIdx = (breakOffset + numBreaks + i) % numEmployees;
                if (!breakIndices.contains(empIdx)) {
                    TableAssignment assign;
                    assign.tableNumber = tableNum;
                    assign.tableGame = schedule.tableGames.value(tableNum - 1, "Blackjack");
                    assign.employeeId = shiftMembers[empIdx].getId();
                    assign.employeeName = shiftMembers[empIdx].getName();
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
                assign.tableGame = schedule.tableGames.value(t - 1, "Blackjack");
                assign.employeeId = shiftMembers[empIdx].getId();
                assign.employeeName = shiftMembers[empIdx].getName();
                slot.tableAssignments.append(assign);
            }
            if ((s + 1) % 4 == 0) {
                slot.onBreakEmployeeNames.append("Relief Needed (All 4 staff reached 4 tables)");
            }
        } else {
            // Less than 4 employees: assign as many tables as possible
            for (int i = 0; i < numEmployees; ++i) {
                TableAssignment assign;
                assign.tableNumber = i + 1;
                assign.tableGame = schedule.tableGames.value(i, "Blackjack");
                assign.employeeId = shiftMembers[i].getId();
                assign.employeeName = shiftMembers[i].getName();
                slot.tableAssignments.append(assign);
            }
            for (int unstaffed = numEmployees + 1; unstaffed <= tableCount; ++unstaffed) {
                TableAssignment assign;
                assign.tableNumber = unstaffed;
                assign.tableGame = schedule.tableGames.value(unstaffed - 1, "Blackjack");
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
    Q_UNUSED(reason);
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

    // 1. Role compatibility check for requested target team
    if (!targetTeam.hasCompatibleRole(emp)) {
        if (targetTeam.isGameShowTeam()) {
            result.message = QString("Cannot reassign %1 (Role: '%2') to %3: This team is dedicated to Game Show Hosts.")
                             .arg(emp.getName(), emp.getRoleType(), requestedTargetTeam);
        } else {
            result.message = QString("Cannot reassign %1 (Role: '%2') to %3: This team is a Standard Dealer team.")
                             .arg(emp.getName(), emp.getRoleType(), requestedTargetTeam);
        }
        return result;
    }

    // 2. Game Knowledge compatibility check for requested target team
    if (!targetTeam.hasRequiredGameKnowledge(emp)) {
        QStringList missing = targetTeam.getMissingGames(emp);
        result.message = QString("Cannot reassign %1 (Game Knowledge: '%2') to %3 (Tables: %4): Missing required game knowledge for [%5].")
                         .arg(emp.getName(), emp.getGameKnowledge(), requestedTargetTeam,
                              targetTeam.getUniqueGames().join(", "), missing.join(", "));
        return result;
    }

    // 3. Check if target team is within the 7-person shift limit AND has uniform in employee size
    int currentTargetStaff = m_dbManager->getTeamMemberCount(targetTeam.getName());
    int maxShiftCap = targetTeam.getMaxStaff() > 0 ? qMin(targetTeam.getMaxStaff(), MAX_SHIFT_STAFF) : MAX_SHIFT_STAFF;
    bool targetUnderShiftCap = (currentTargetStaff < maxShiftCap);

    int availForRequested = m_dbManager->getAvailableUniformCountForTeam(
        targetTeam.getName(), targetTeam.getUniformType(), result.employeeSize);

    if (targetUnderShiftCap && availForRequested > 0) {
        Uniform uni = m_dbManager->findAvailableUniformForTeam(
            targetTeam.getName(), targetTeam.getUniformType(), result.employeeSize);

        if (m_dbManager->updateEmployeeTeam(employeeId, requestedTargetTeam)) {
            if (uni.getId() > 0) {
                UniformIssue issue;
                issue.setEmployeeId(employeeId);
                issue.setUniformId(uni.getId());
                issue.setQuantity(1);
                issue.setIssueDate(QDate::currentDate());
                issue.setStatus("Issued");
                m_dbManager->issueUniform(issue);
            }

            adjustTeamRotation(result.sourceTeam);
            adjustTeamRotation(requestedTargetTeam);

            result.success = true;
            result.assignedTeam = requestedTargetTeam;
            result.requestedTeamHadUniform = true;
            result.fallbackUsed = false;
            result.message = QString("[#%1] %2: %3 -> %4 (%5/%6 staff) | Uniform: %7 (Size %8) | Games: %9")
                             .arg(employeeId)
                             .arg(result.employeeName, result.sourceTeam, requestedTargetTeam)
                             .arg(currentTargetStaff + 1).arg(maxShiftCap)
                             .arg(uni.getType(), result.employeeSize, emp.getGameKnowledge());
            return result;
        } else {
            result.message = "Failed to update employee team in database: " + m_dbManager->getLastError();
            return result;
        }
    }

    // 4. Target team either exceeded the 7-person shift limit OR had no uniform in employee size:
    // Redirect employee to another compatible team based on their Game Knowledge and Role!
    result.requestedTeamHadUniform = (availForRequested > 0);

    QList<Team> allTeams = m_dbManager->getAllTeams();
    Team fallbackTeam;
    int bestScore = -1;

    for (const Team& candidate : allTeams) {
        if (candidate.getName() == result.sourceTeam || candidate.getName() == requestedTargetTeam) {
            continue;
        }

        // Candidate team must accept this employee based on BOTH role and game knowledge
        if (!candidate.acceptsEmployee(emp)) {
            continue;
        }

        int currentStaff = m_dbManager->getTeamMemberCount(candidate.getName());
        int candidateMax = candidate.getMaxStaff() > 0 ? qMin(candidate.getMaxStaff(), MAX_SHIFT_STAFF) : MAX_SHIFT_STAFF;
        if (currentStaff >= candidateMax) {
            continue; // Already at 7-person shift limit
        }

        int availCount = m_dbManager->getAvailableUniformCountForTeam(
            candidate.getName(), candidate.getUniformType(), result.employeeSize);

        if (availCount > 0) {
            int score = 0;
            if (currentStaff < candidate.getMinStaff()) {
                score = 100 - currentStaff; // Dire / critically understaffed - highest priority
            } else if (currentStaff < candidate.getTargetStaff()) {
                score = 50 - currentStaff;  // Understaffed (< 7) - high priority
            } else {
                score = 10;
            }

            if (score > bestScore) {
                bestScore = score;
                fallbackTeam = candidate;
            }
        }
    }

    if (fallbackTeam.getId() > 0) {
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
            if (!targetUnderShiftCap) {
                result.fallbackReason = QString("[#%1] %2: %3 -> %4 (%5 was at %6-staff shift limit; matched games: %7, Size: %8)")
                                        .arg(employeeId)
                                        .arg(result.employeeName, result.sourceTeam, fallbackTeam.getName(), requestedTargetTeam)
                                        .arg(maxShiftCap)
                                        .arg(emp.getGameKnowledge(), result.employeeSize);
            } else {
                result.fallbackReason = QString("[#%1] %2: %3 -> %4 (No size '%5' uniform in %6; redirected by game knowledge, Type: %7)")
                                        .arg(employeeId)
                                        .arg(result.employeeName, result.sourceTeam, fallbackTeam.getName(),
                                             result.employeeSize, requestedTargetTeam, fallbackUni.getType());
            }
            result.message = result.fallbackReason;
            return result;
        } else {
            result.message = "Failed to update employee team for fallback: " + m_dbManager->getLastError();
            return result;
        }
    }

    result.success = false;
    if (!targetUnderShiftCap) {
        result.message = QString("Cannot assign %1 to %2: Team is at the 7-person shift limit, and no other compatible team matching game knowledge (%3) has open slots and size '%4' uniforms.")
                         .arg(result.employeeName, requestedTargetTeam, emp.getGameKnowledge(), result.employeeSize);
    } else {
        result.message = QString("Cannot reassign %1 to %2: No uniform available in size '%3' for %2, "
                                 "and no compatible alternative team matching game knowledge has available uniforms in this size!")
                         .arg(result.employeeName, requestedTargetTeam, result.employeeSize);
    }
    return result;
}

QList<ReassignmentResult> RotationScheduler::relocateOverflowStaff(const QString& teamName)
{
    QList<ReassignmentResult> results;
    if (!m_dbManager) return results;

    QList<Team> teamsToCheck;
    if (!teamName.isEmpty()) {
        Team t = m_dbManager->getTeamByName(teamName);
        if (t.getId() > 0) teamsToCheck.append(t);
    } else {
        teamsToCheck = m_dbManager->getAllTeams();
    }

    for (const Team& sourceTeam : teamsToCheck) {
        QList<Employee> members = m_dbManager->getEmployeesByTeam(sourceTeam.getName());
        int maxCap = sourceTeam.getMaxStaff() > 0 ? qMin(sourceTeam.getMaxStaff(), MAX_SHIFT_STAFF) : MAX_SHIFT_STAFF;
        int overflowCount = members.size() - maxCap;
        if (overflowCount <= 0) {
            continue;
        }

        // Move overflow employees to compatible teams (< 7 staff) based on their game knowledge.
        // Prefer selecting from the tail of the roster (index >= maxCap), falling back to earlier
        // multi-game members if a tail member only knows basic games and basic teams are full.
        for (int m = 0; m < overflowCount; ++m) {
            QList<Employee> currentMembers = m_dbManager->getEmployeesByTeam(sourceTeam.getName());
            if (currentMembers.size() <= maxCap) {
                break;
            }

            QList<Team> allTeams = m_dbManager->getAllTeams();
            int chosenEmpId = -1;
            QString chosenTarget = "";
            int bestOverallScore = -1;

            for (int idx = currentMembers.size() - 1; idx >= 0; --idx) {
                const Employee& candEmp = currentMembers[idx];
                QString empSize = candEmp.getUniformSize().isEmpty() ? "M" : candEmp.getUniformSize();
                int tailBonus = (idx >= maxCap) ? 500 : 0;

                for (const Team& candidate : allTeams) {
                    if (candidate.getName() == sourceTeam.getName()) continue;
                    if (!candidate.acceptsEmployee(candEmp)) continue;

                    int count = m_dbManager->getTeamMemberCount(candidate.getName());
                    int candMax = candidate.getMaxStaff() > 0 ? qMin(candidate.getMaxStaff(), MAX_SHIFT_STAFF) : MAX_SHIFT_STAFF;
                    if (count >= candMax) continue;

                    int availUni = m_dbManager->getAvailableUniformCountForTeam(
                        candidate.getName(), candidate.getUniformType(), empSize);
                    if (availUni <= 0) continue;

                    int priority = tailBonus + (candMax - count) * 15;
                    if (count < candidate.getMinStaff()) {
                        priority += 100; // Dire understaffing priority
                    }
                    if (candidate.isGameShowTeam() && candEmp.canHostGameShow()) {
                        priority += 200; // Ensure Game Show studios get dual-role/host overflow first
                    }
                    priority += candidate.getUniqueGames().size() * 5;

                    if (priority > bestOverallScore) {
                        bestOverallScore = priority;
                        chosenEmpId = candEmp.getId();
                        chosenTarget = candidate.getName();
                    }
                }
            }

            if (chosenEmpId > 0 && !chosenTarget.isEmpty()) {
                results.append(reassignEmployee(chosenEmpId, chosenTarget,
                                                "Relocate overflow beyond 7-staff shift limit"));
            }
        }
    }

    return results;
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
    int effectiveSlotMinutes = normalizeSlotMinutes(slotMinutes, team, members.size());
    ShiftSchedule schedule = generate8HourSchedule(team, members, date, startTime, effectiveSlotMinutes, numSlots);

    // Persist to database
    m_dbManager->clearShiftSchedule(teamName, date);
    for (const RotationSlot& slot : schedule.rotationSlots) {
        QStringList assignStrings;
        for (const TableAssignment& assign : slot.tableAssignments) {
            assignStrings << QString("Table %1 (%2): %3").arg(assign.tableNumber).arg(assign.tableGame, assign.employeeName);
        }
        m_dbManager->saveScheduleSlot(teamName, date, slot.slotIndex,
                                     slot.startTime.toString("hh:mm"),
                                     slot.endTime.toString("hh:mm"),
                                     assignStrings.join(" | "),
                                     slot.onBreakEmployeeNames.join(", "));
    }

    return schedule;
}
