#include <cassert>
#include <iostream>
#include <QCoreApplication>
#include <QDate>
#include <QTime>
#include <QFile>
#include "../src/databasemanager.h"
#include "../src/rotationscheduler.h"
#include "../src/employee.h"
#include "../src/uniform.h"
#include "../src/team.h"

int main(int argc, char* argv[])
{
    QCoreApplication app(argc, argv);

    // Remove any previous test database
    QFile::remove("uniformmanagement.db");

    DatabaseManager db;
    bool dbInit = db.initializeDatabase();
    assert(dbInit && "Database initialization must succeed");

    std::cout << "[PASS] Database initialized successfully." << std::endl;

    // 1. Verify default teams seeded
    QList<Team> defaultTeams = db.getAllTeams();
    assert(defaultTeams.size() >= 3 && "At least 3 default teams should be seeded");
    std::cout << "[PASS] Default teams seeded: " << defaultTeams.size() << " teams." << std::endl;

    // 2. Add uniforms to inventory
    // Team Alpha uniform: 2 units in size 'L'
    Uniform uniAlpha(0, "Alpha Uniform", 2, "L", "Navy", "Team Alpha Uniform", "Team Alpha");
    assert(db.addUniform(uniAlpha));

    // Team Beta uniform: 0 units in size 'L' (out of stock in size L!)
    Uniform uniBeta(0, "Beta Uniform", 0, "L", "White", "Team Beta Uniform", "Team Beta");
    assert(db.addUniform(uniBeta));

    // Team Gamma uniform: 3 units in size 'L'
    Uniform uniGamma(0, "Gamma Uniform", 3, "L", "Gray", "Team Gamma Uniform", "Team Gamma");
    assert(db.addUniform(uniGamma));

    std::cout << "[PASS] Test uniforms added to inventory." << std::endl;

    // Verify stock counts
    assert(db.getAvailableUniformCountForTeam("Team Alpha", "Alpha Uniform", "L") == 2);
    assert(db.getAvailableUniformCountForTeam("Team Beta", "Beta Uniform", "L") == 0);
    assert(db.getAvailableUniformCountForTeam("Team Gamma", "Gamma Uniform", "L") == 3);
    std::cout << "[PASS] Stock verification passed (Alpha=2, Beta=0, Gamma=3)." << std::endl;

    // 3. Add employees
    // Add 6 employees to Team Alpha (making it overstaffed: target is 5)
    for (int i = 1; i <= 6; ++i) {
        Employee e(0, QString("Emp_Alpha_%1").arg(i), "Team Alpha", "Dealer", "L");
        assert(db.addEmployee(e));
    }

    // Add 3 employees to Team Beta (making it understaffed: target is 5, min is 4)
    for (int i = 1; i <= 3; ++i) {
        Employee e(0, QString("Emp_Beta_%1").arg(i), "Team Beta", "Dealer", "L");
        assert(db.addEmployee(e));
    }

    // Add 3 employees to Team Gamma (understaffed: target is 5, min is 4)
    for (int i = 1; i <= 3; ++i) {
        Employee e(0, QString("Emp_Gamma_%1").arg(i), "Team Gamma", "Dealer", "L");
        assert(db.addEmployee(e));
    }

    assert(db.getTeamMemberCount("Team Alpha") == 6);
    assert(db.getTeamMemberCount("Team Beta") == 3);
    assert(db.getTeamMemberCount("Team Gamma") == 3);
    std::cout << "[PASS] Employees assigned. Team Alpha is overstaffed (6), Beta (3) & Gamma (3) understaffed." << std::endl;

    // 4. Test Reassignment with Uniform Availability Checking & Automatic Fallback
    RotationScheduler scheduler(&db);

    // Get an employee from overstaffed Team Alpha
    QList<Employee> alphaEmps = db.getEmployeesByTeam("Team Alpha");
    assert(!alphaEmps.isEmpty());
    Employee excessEmp = alphaEmps.last();

    // Attempt to move employee to Team Beta (which is understaffed, BUT has 0 uniforms in size 'L'!)
    std::cout << "Attempting to reassign " << excessEmp.getName().toStdString() 
              << " (Size " << excessEmp.getUniformSize().toStdString() 
              << ") to Team Beta (out of stock)..." << std::endl;

    ReassignmentResult result = scheduler.reassignEmployee(excessEmp.getId(), "Team Beta", "Understaffing rebalance");

    // The system MUST detect that Team Beta has no uniform in size L,
    // and automatically redirect to Team Gamma which has size L in stock!
    assert(result.success && "Reassignment should succeed via fallback redirection");
    assert(!result.requestedTeamHadUniform && "Target team Beta did not have uniform");
    assert(result.fallbackUsed && "Fallback to alternative team must be used");
    assert(result.assignedTeam == "Team Gamma" && "Employee should be redirected to Team Gamma");
    std::cout << "[PASS] Fallback redirection successful! " << result.message.toStdString() << std::endl;

    // Verify employee's team in database is now Team Gamma
    Employee updatedEmp = db.getEmployee(excessEmp.getId());
    assert(updatedEmp.getDepartment() == "Team Gamma");
    assert(db.getTeamMemberCount("Team Alpha") == 5); // Now optimal
    assert(db.getTeamMemberCount("Team Gamma") == 4); // Increased
    std::cout << "[PASS] Team memberships updated in database." << std::endl;

    // Verify that available uniform in Team Gamma was decremented by 1 (issued to employee)
    assert(db.getAvailableUniformCountForTeam("Team Gamma", "Gamma Uniform", "L") == 2);
    std::cout << "[PASS] Uniform inventory stock correctly decremented after issue." << std::endl;

    // 5. Test 8-Hour Shift 4-Table Rotation with 30-min Break Cycle
    Team gammaTeam = db.getTeamByName("Team Gamma");
    // Add one more employee to Team Gamma so it has 5 employees (optimal team for 4 tables)
    Employee gammaEmp5(0, "Emp_Gamma_5", "Team Gamma", "Dealer", "L");
    db.addEmployee(gammaEmp5);
    QList<Employee> gammaMembers = db.getEmployeesByTeam("Team Gamma");
    assert(gammaMembers.size() == 5);

    ShiftSchedule schedule = scheduler.generate8HourSchedule(
        gammaTeam, gammaMembers, QDate::currentDate(), QTime(8, 0), 30);

    assert(schedule.shiftDurationHours == 8);
    assert(schedule.slotMinutes == 30);
    assert(schedule.rotationSlots.size() == 16); // 8 hours * (60/30) = 16 slots

    // Verify that at each slot:
    // - Exactly 4 tables are staffed (Table 1, Table 2, Table 3, Table 4)
    // - Exactly 1 employee is on Break
    // - An employee works 4 tables, then takes a 30-minute break!
    for (int s = 0; s < schedule.rotationSlots.size(); ++s) {
        const RotationSlot& slot = schedule.rotationSlots[s];
        assert(slot.tableAssignments.size() == 4);
        assert(slot.onBreakEmployeeNames.size() == 1);

        // Check that all 4 table numbers are present: 1, 2, 3, 4
        QList<int> assignedTables;
        for (const auto& a : slot.tableAssignments) {
            assignedTables.append(a.tableNumber);
            assert(!a.employeeName.isEmpty());
        }
        assert(assignedTables.contains(1));
        assert(assignedTables.contains(2));
        assert(assignedTables.contains(3));
        assert(assignedTables.contains(4));
    }
    std::cout << "[PASS] 8-Hour Shift (16 slots): All 4 tables continuously staffed at every slot." << std::endl;

    // Check individual employee break cycle:
    // With 5 employees, in every 5 slots, an employee is on break on slot (s % 5 == offset),
    // meaning exactly 4 consecutive slots on tables followed by 1 slot (30-min) break!
    int emp0Id = gammaMembers[0].getId();
    int consecutiveWorked = 0;
    int maxConsecutiveWorked = 0;
    int breaksCount = 0;

    for (int s = 0; s < schedule.rotationSlots.size(); ++s) {
        const RotationSlot& slot = schedule.rotationSlots[s];
        bool onBreak = slot.onBreakEmployeeIds.contains(emp0Id);
        if (onBreak) {
            breaksCount++;
            consecutiveWorked = 0;
        } else {
            consecutiveWorked++;
            if (consecutiveWorked > maxConsecutiveWorked) {
                maxConsecutiveWorked = consecutiveWorked;
            }
        }
        // At no point should consecutive table slots exceed 4!
        assert(consecutiveWorked <= 4 && "Employee must not work more than 4 consecutive table sessions without a break");
    }
    std::cout << "[PASS] Break Cycle Verified: Employee worked max " << maxConsecutiveWorked 
              << " tables before taking 30-min break (total " << breaksCount << " breaks in 8h shift)." << std::endl;

    std::cout << "\nALL VERIFICATION TESTS COMPLETED SUCCESSFULLY!" << std::endl;
    return 0;
}
