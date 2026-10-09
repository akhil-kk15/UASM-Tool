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

    // Use dedicated isolated test database to protect sample database
    const QString testDbName = "test_uniformmanagement.db";
    QFile::remove(testDbName);

    DatabaseManager db;
    bool dbInit = db.initializeDatabase(testDbName);
    assert(dbInit && "Database initialization must succeed");

    std::cout << "[PASS] Database initialized successfully." << std::endl;

    // 1. Verify default teams seeded
    QList<Team> defaultTeams = db.getAllTeams();
    assert(defaultTeams.size() >= 3 && "At least 3 default teams should be seeded");
    std::cout << "[PASS] Default teams seeded: " << defaultTeams.size() << " teams." << std::endl;

    // 2. Verify seeded Game Show Host team and add another
    Team fetchedStudio = db.getTeamByName("Studio Live");
    assert(fetchedStudio.getId() > 0);
    assert(fetchedStudio.isGameShowTeam());
    assert(fetchedStudio.getTeamType() == "Game Show Host");

    Team vipTeam(0, "VIP Studio", "VIP Gold", 5, 4, 7, "Game Show Host");
    assert(db.addTeam(vipTeam));
    assert(db.getTeamByName("VIP Studio").isGameShowTeam());
    std::cout << "[PASS] Game Show Host team created & verified." << std::endl;

    // 3. Verify Role Capability Helpers
    Employee dealerEmp(0, "Dave Dealer", "Team Alpha", "Dealer", "L", "", "", "Dealer");
    Employee hostEmp(0, "Holly Host", "Studio Live", "Host", "L", "", "", "Game Show Host");
    Employee bothEmp(0, "Ben Both", "Team Alpha", "Dealer & Host", "L", "", "", "Both");

    assert(dealerEmp.canDeal() && !dealerEmp.canHostGameShow());
    assert(!hostEmp.canDeal() && hostEmp.canHostGameShow());
    assert(bothEmp.canDeal() && bothEmp.canHostGameShow());

    Team standardTeam = db.getTeamByName("Team Alpha");
    assert(standardTeam.acceptsEmployee(dealerEmp));
    assert(!standardTeam.acceptsEmployee(hostEmp));
    assert(standardTeam.acceptsEmployee(bothEmp));

    assert(!fetchedStudio.acceptsEmployee(dealerEmp));
    assert(fetchedStudio.acceptsEmployee(hostEmp));
    assert(fetchedStudio.acceptsEmployee(bothEmp));
    std::cout << "[PASS] Employee and Team role compatibility verified." << std::endl;

    // 4. Add uniforms to inventory
    // Team Alpha uniform: 2 units in size 'L'
    Uniform uniAlpha(0, "Alpha Uniform", 2, "L", "Navy", "Team Alpha Uniform", "Team Alpha");
    assert(db.addUniform(uniAlpha));

    // Team Beta uniform: 0 units in size 'L' (out of stock in size L!)
    Uniform uniBeta(0, "Beta Uniform", 0, "L", "White", "Team Beta Uniform", "Team Beta");
    assert(db.addUniform(uniBeta));

    // Team Gamma uniform: 3 units in size 'L'
    Uniform uniGamma(0, "Gamma Uniform", 3, "L", "Gray", "Team Gamma Uniform", "Team Gamma");
    assert(db.addUniform(uniGamma));

    // Studio Live uniform: 5 units in size 'L'
    Uniform uniStudio(0, "Studio Gold", 5, "L", "Gold", "Studio Gold Uniform", "Studio Live");
    assert(db.addUniform(uniStudio));

    std::cout << "[PASS] Test uniforms added to inventory." << std::endl;

    // 5. Add employees
    // 6 employees to Team Alpha (overstaffed, role = Dealer)
    for (int i = 1; i <= 6; ++i) {
        Employee e(0, QString("Emp_Alpha_%1").arg(i), "Team Alpha", "Dealer", "L", "", "", "Dealer");
        assert(db.addEmployee(e));
    }

    // 3 employees to Team Beta (understaffed, role = Dealer)
    for (int i = 1; i <= 3; ++i) {
        Employee e(0, QString("Emp_Beta_%1").arg(i), "Team Beta", "Dealer", "L", "", "", "Dealer");
        assert(db.addEmployee(e));
    }

    // 3 employees to Team Gamma (understaffed, role = Dealer)
    for (int i = 1; i <= 3; ++i) {
        Employee e(0, QString("Emp_Gamma_%1").arg(i), "Team Gamma", "Dealer", "L", "", "", "Dealer");
        assert(db.addEmployee(e));
    }

    // 1 host to Studio Live
    Employee host1(0, "Host_1", "Studio Live", "Host", "L", "", "", "Game Show Host");
    assert(db.addEmployee(host1));

    RotationScheduler scheduler(&db);

    // 6. Test Role Rejection: Pure Dealer cannot be moved to Game Show Host team
    QList<Employee> alphaEmps = db.getEmployeesByTeam("Team Alpha");
    assert(!alphaEmps.isEmpty());
    Employee excessDealer = alphaEmps.last();

    ReassignmentResult rejResult = scheduler.reassignEmployee(excessDealer.getId(), "Studio Live", "Test host move");
    assert(!rejResult.success && "Dealer must not be accepted into Game Show Host team");
    std::cout << "[PASS] Role restriction verified: Reassignment to incompatible team rejected (" 
              << rejResult.message.toStdString() << ")." << std::endl;

    // 7. Test Role Rejection: Pure Host cannot be moved to Standard Dealer team
    QList<Employee> studioEmps = db.getEmployeesByTeam("Studio Live");
    assert(!studioEmps.isEmpty());
    Employee hostMember = studioEmps.first();
    ReassignmentResult rejResult2 = scheduler.reassignEmployee(hostMember.getId(), "Team Alpha", "Test dealer move");
    assert(!rejResult2.success && "Host must not be accepted into Standard Dealer team");
    std::cout << "[PASS] Role restriction verified: Game Show Host rejected from Standard Dealer team." << std::endl;

    // 8. Test Uniform Fallback with Role Filtering:
    // Move excessDealer (pure Dealer) to Team Beta (size L out of stock).
    // Notice Studio Live has 5 size L uniforms and is understaffed (only 1 member), BUT it's a Game Show Host team!
    // System MUST NOT fallback to Studio Live; it MUST fallback to Team Gamma (Standard team with size L)!
    ReassignmentResult fbResult = scheduler.reassignEmployee(excessDealer.getId(), "Team Beta", "Understaffing rebalance");
    assert(fbResult.success);
    assert(fbResult.fallbackUsed);
    assert(fbResult.assignedTeam == "Team Gamma" && "Fallback must pick compatible Team Gamma, NOT Studio Live");
    std::cout << "[PASS] Fallback role filtering verified: Dealer redirected to Team Gamma, avoiding Game Show Host team." << std::endl;

    // 9. Test Employee with role "Both" can be assigned to Game Show Host team
    Employee flexEmp(0, "Flex Dealer-Host", "Team Alpha", "Flex", "L", "", "", "Both");
    assert(db.addEmployee(flexEmp));
    QList<Employee> alphaEmpsAfter = db.getEmployeesByTeam("Team Alpha");
    Employee flexFromDb = alphaEmpsAfter.last();
    assert(flexFromDb.getRoleType() == "Both");

    ReassignmentResult flexResult = scheduler.reassignEmployee(flexFromDb.getId(), "Studio Live", "Promotion to host");
    assert(flexResult.success && "Employee with role Both can join Game Show Host team");
    assert(flexResult.assignedTeam == "Studio Live");
    std::cout << "[PASS] Dual-role 'Both' employee successfully assigned to Game Show Host team." << std::endl;

    // 10. Test 30 Mins Rotation Type
    Team gammaTeam = db.getTeamByName("Team Gamma");
    // Ensure Team Gamma has 5 employees
    while (db.getTeamMemberCount("Team Gamma") < 5) {
        int idx = db.getTeamMemberCount("Team Gamma") + 1;
        Employee ge(0, QString("Emp_Gamma_Extra_%1").arg(idx), "Team Gamma", "Dealer", "L", "", "", "Dealer");
        db.addEmployee(ge);
    }
    QList<Employee> gammaMembers = db.getEmployeesByTeam("Team Gamma");
    assert(gammaMembers.size() >= 5);

    ShiftSchedule sched30 = scheduler.generate8HourSchedule(
        gammaTeam, gammaMembers, QDate::currentDate(), QTime(8, 0), 30);
    assert(sched30.slotMinutes == 30);
    assert(sched30.rotationSlots.size() == 20); // 20 slots of 30 mins
    assert(sched30.rotationSlots[0].startTime == QTime(8, 0));
    assert(sched30.rotationSlots[0].endTime == QTime(8, 30));
    std::cout << "[PASS] 30 Mins Rotation Type generated (20 slots, 30-min duration)." << std::endl;

    // 11. Test 40 Mins Rotation Type
    ShiftSchedule sched40 = scheduler.generate8HourSchedule(
        gammaTeam, gammaMembers, QDate::currentDate(), QTime(8, 0), 40);
    assert(sched40.slotMinutes == 40);
    assert(sched40.rotationSlots.size() == 20); // 20 slots of 40 mins
    assert(sched40.rotationSlots[0].startTime == QTime(8, 0));
    assert(sched40.rotationSlots[0].endTime == QTime(8, 40));
    assert(sched40.rotationSlots[1].startTime == QTime(8, 40));
    assert(sched40.rotationSlots[1].endTime == QTime(9, 20));

    // Verify each slot has 4 tables and 1 break
    for (int s = 0; s < sched40.rotationSlots.size(); ++s) {
        const RotationSlot& slot = sched40.rotationSlots[s];
        assert(slot.tableAssignments.size() == 4);
        assert(slot.onBreakEmployeeNames.size() >= 1);
    }

    // Verify 16 tables per employee & 4 breaks
    int testEmpId = gammaMembers[0].getId();
    int tablesWorked = 0;
    int breaksCount = 0;
    int maxConsecutive = 0;
    int curConsecutive = 0;
    for (int s = 0; s < sched40.rotationSlots.size(); ++s) {
        const RotationSlot& slot = sched40.rotationSlots[s];
        if (slot.onBreakEmployeeIds.contains(testEmpId)) {
            breaksCount++;
            curConsecutive = 0;
        } else {
            tablesWorked++;
            curConsecutive++;
            if (curConsecutive > maxConsecutive) maxConsecutive = curConsecutive;
        }
        assert(curConsecutive <= 4 && "No employee should exceed 4 consecutive tables without break");
    }
    assert(tablesWorked == 16 && "Employee must work 16 tables");
    assert(breaksCount == 4 && "Employee must have 4 breaks");
    std::cout << "[PASS] 40 Mins Rotation Type verified (20 slots, 16 tables worked, 4 breaks, max consecutive tables: "
              << maxConsecutive << ")." << std::endl;

    std::cout << "\nALL TEST SUITE CHECKS COMPLETED SUCCESSFULLY!" << std::endl;
    db.closeDatabase();
    QFile::remove(testDbName);
    return 0;
}
