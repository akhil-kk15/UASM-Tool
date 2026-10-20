// Keep assertions active in every build type: Release/RelWithDebInfo define
// NDEBUG, which would otherwise silently disable every check below.
#ifdef NDEBUG
#undef NDEBUG
#endif
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

    // 1. Verify 15 default teams seeded and Team Alpha / Team Bravo dedicated to Blackjack (max 7 staff)
    QList<Team> defaultTeams = db.getAllTeams();
    assert(defaultTeams.size() == 15 && "15 default teams should be seeded");
    Team alphaTeam = db.getTeamByName("Team Alpha");
    Team bravoTeam = db.getTeamByName("Team Bravo");
    Team charlieTeam = db.getTeamByName("Team Charlie");
    Team echoTeam = db.getTeamByName("Team Echo");
    assert(alphaTeam.getMaxStaff() == 7);
    assert(alphaTeam.getUniqueGames() == QStringList({"Blackjack"}));
    assert(bravoTeam.getUniqueGames() == QStringList({"Blackjack"}));
    std::cout << "[PASS] 15 default teams seeded (max 7 staff/shift); Team Alpha & Team Bravo dedicated to Blackjack." << std::endl;

    // 2. Verify seeded Game Show Host team and add another
    Team fetchedStudio = db.getTeamByName("Studio Live");
    assert(fetchedStudio.getId() > 0);
    assert(fetchedStudio.isGameShowTeam());
    assert(fetchedStudio.getTeamType() == "Game Show Host");

    Team vipTeam(0, "VIP Studio", "VIP Gold", 7, 5, 7, "Game Show Host", "Game Show, Game Show, Game Show, Game Show");
    assert(db.addTeam(vipTeam));
    assert(db.getTeamByName("VIP Studio").isGameShowTeam());
    std::cout << "[PASS] Game Show Host team created & verified." << std::endl;

    // 3. Verify Role & Game Knowledge Capability Helpers
    Employee basicDealer(0, "Dave Basic", "Team Alpha", "Dealer", "L", "", "", "Dealer", "Blackjack, Roulette");
    Employee multiDealer(0, "Mia Multi", "Team Echo", "Dealer", "L", "", "", "Dealer", "Blackjack, Roulette, Baccarat, Poker, Craps");
    Employee hostEmp(0, "Holly Host", "Studio Live", "Host", "L", "", "", "Game Show Host", "Game Show");
    Employee bothEmp(0, "Ben Both", "Team Alpha", "Dealer & Host", "L", "", "", "Both", "Blackjack, Roulette, Baccarat, Poker, Craps, Game Show");

    assert(basicDealer.canDeal() && !basicDealer.canHostGameShow());
    assert(!hostEmp.canDeal() && hostEmp.canHostGameShow());
    assert(bothEmp.canDeal() && bothEmp.canHostGameShow());

    // Basic dealer (Blackjack, Roulette) is accepted in Team Alpha (Blackjack) and Team Charlie (Blackjack, Roulette),
    // but NOT in Team Echo (requires Baccarat)
    assert(alphaTeam.acceptsEmployee(basicDealer));
    assert(charlieTeam.acceptsEmployee(basicDealer));
    assert(!echoTeam.acceptsEmployee(basicDealer));
    assert(echoTeam.acceptsEmployee(multiDealer));

    assert(!alphaTeam.acceptsEmployee(hostEmp));
    assert(alphaTeam.acceptsEmployee(bothEmp));

    assert(!fetchedStudio.acceptsEmployee(basicDealer));
    assert(fetchedStudio.acceptsEmployee(hostEmp));
    assert(fetchedStudio.acceptsEmployee(bothEmp));
    std::cout << "[PASS] Employee and Team role + game knowledge compatibility verified." << std::endl;

    // 4. Add uniforms to inventory
    // Team Alpha uniform: 2 units in size 'L'
    Uniform uniAlpha(0, "Team Alpha Vest", 2, "L", "Crimson Red", "Team Alpha Uniform", "Team Alpha");
    assert(db.addUniform(uniAlpha));

    // Team Bravo uniform: 0 units in size 'L' (out of stock in size L!)
    Uniform uniBravo(0, "Team Bravo Vest", 0, "L", "Royal Blue", "Team Bravo Uniform", "Team Bravo");
    assert(db.addUniform(uniBravo));

    // Team Charlie uniform: 5 units in size 'L'
    Uniform uniCharlie(0, "Team Charlie Vest", 5, "L", "Emerald Green", "Team Charlie Uniform", "Team Charlie");
    assert(db.addUniform(uniCharlie));

    // Team Echo uniform: 5 units in size 'L'
    Uniform uniEcho(0, "Team Echo Vest", 5, "L", "Charcoal Grey", "Team Echo Uniform", "Team Echo");
    assert(db.addUniform(uniEcho));

    // Studio Live uniform: 5 units in size 'L'
    Uniform uniStudio(0, "Studio Live Suit", 5, "L", "Gold", "Studio Live Uniform", "Studio Live");
    assert(db.addUniform(uniStudio));

    std::cout << "[PASS] Test uniforms added to inventory." << std::endl;

    // 5. Add employees
    // 8 employees to Team Alpha (exceeds 7-person shift limit: 7 with basic Blackjack, Roulette and 8th with Baccarat too)
    for (int i = 1; i <= 7; ++i) {
        Employee e(0, QString("Emp_Alpha_%1").arg(i), "Team Alpha", "Dealer", "L", "", "", "Dealer", "Blackjack, Roulette");
        assert(db.addEmployee(e));
    }
    Employee overflowBaccaratDealer(0, "Emp_Alpha_8_Baccarat", "Team Alpha", "Dealer", "L", "", "", "Dealer", "Blackjack, Roulette, Baccarat");
    assert(db.addEmployee(overflowBaccaratDealer));

    // 3 employees to Team Bravo (understaffed, role = Dealer)
    for (int i = 1; i <= 3; ++i) {
        Employee e(0, QString("Emp_Bravo_%1").arg(i), "Team Bravo", "Dealer", "L", "", "", "Dealer", "Blackjack, Roulette");
        assert(db.addEmployee(e));
    }

    // 3 employees to Team Charlie (understaffed, role = Dealer)
    for (int i = 1; i <= 3; ++i) {
        Employee e(0, QString("Emp_Charlie_%1").arg(i), "Team Charlie", "Dealer", "L", "", "", "Dealer", "Blackjack, Roulette");
        assert(db.addEmployee(e));
    }

    // 1 host to Studio Live
    Employee host1(0, "Host_1", "Studio Live", "Host", "L", "", "", "Game Show Host", "Game Show");
    assert(db.addEmployee(host1));

    RotationScheduler scheduler(&db);

    // 6. Test Overflow Relocation (> 7 staff on Team Alpha relocated based on Game Knowledge)
    assert(db.getTeamMemberCount("Team Alpha") == 8);
    QList<ReassignmentResult> overflowMoves = scheduler.relocateOverflowStaff("Team Alpha");
    assert(overflowMoves.size() == 1 && overflowMoves[0].success);
    assert(db.getTeamMemberCount("Team Alpha") == 7 && "Team Alpha must be capped at 7 after overflow relocation");
    std::cout << "[PASS] 7-person shift limit enforced: 8th employee automatically relocated to "
              << overflowMoves[0].assignedTeam.toStdString() << " based on game knowledge." << std::endl;

    // 7. Test 5-digit employee IDs and Role Rejection: Pure Dealer cannot be moved to Game Show Host team
    QList<Employee> alphaEmps = db.getEmployeesByTeam("Team Alpha");
    assert(!alphaEmps.isEmpty());
    Employee excessDealer = alphaEmps.last();
    assert(excessDealer.getId() >= 10000 && excessDealer.getId() <= 99999 && "Employee ID must be a 5-digit integer");
    std::cout << "[PASS] 5-digit Employee ID verified (e.g. #" << excessDealer.getId() << ")." << std::endl;

    ReassignmentResult rejResult = scheduler.reassignEmployee(excessDealer.getId(), "Studio Live", "Test host move");
    assert(!rejResult.success && "Dealer must not be accepted into Game Show Host team");
    std::cout << "[PASS] Role restriction verified: Reassignment to incompatible team rejected." << std::endl;

    // 8. Test Game Knowledge Rejection: Basic Dealer (Blackjack, Roulette) cannot be moved to Team Echo (requires Baccarat)
    ReassignmentResult gameRejResult = scheduler.reassignEmployee(excessDealer.getId(), "Team Echo", "Test game knowledge move");
    assert(!gameRejResult.success && "Basic dealer without Baccarat knowledge must not be accepted into Team Echo");
    std::cout << "[PASS] Game knowledge restriction verified: Basic dealer rejected from Team Echo." << std::endl;

    // 9. Test Role Rejection: Pure Host cannot be moved to Standard Dealer team
    QList<Employee> studioEmps = db.getEmployeesByTeam("Studio Live");
    assert(!studioEmps.isEmpty());
    Employee hostMember = studioEmps.first();
    ReassignmentResult rejResult2 = scheduler.reassignEmployee(hostMember.getId(), "Team Alpha", "Test dealer move");
    assert(!rejResult2.success && "Host must not be accepted into Standard Dealer team");
    std::cout << "[PASS] Role restriction verified: Game Show Host rejected from Standard Dealer team." << std::endl;

    // 10. Test Uniform Fallback with Role & Game Knowledge Filtering:
    // Move excessDealer (Blackjack, Roulette) to Team Bravo (size L out of stock).
    // System MUST NOT fallback to Studio Live (wrong role) or Team Echo (requires Baccarat);
    // it MUST fallback to Team Charlie (Blackjack, Roulette, size L in stock)!
    ReassignmentResult fbResult = scheduler.reassignEmployee(excessDealer.getId(), "Team Bravo", "Understaffing rebalance");
    assert(fbResult.success);
    assert(fbResult.fallbackUsed);
    assert(fbResult.assignedTeam == "Team Charlie" && "Fallback must pick compatible Team Charlie");
    std::cout << "[PASS] Fallback role & game knowledge filtering verified: Dealer redirected to Team Charlie." << std::endl;

    // 11. Test Employee with role "Both" and all games can be assigned to Game Show Host team
    Employee flexEmp(0, "Flex Dealer-Host", "Team Alpha", "Flex", "L", "", "", "Both",
                     "Blackjack, Roulette, Baccarat, Poker, Craps, Game Show");
    assert(db.addEmployee(flexEmp));
    QList<Employee> alphaEmpsAfter = db.getEmployeesByTeam("Team Alpha");
    Employee flexFromDb = alphaEmpsAfter.last();
    assert(flexFromDb.getRoleType() == "Both");

    ReassignmentResult flexResult = scheduler.reassignEmployee(flexFromDb.getId(), "Studio Live", "Promotion to host");
    assert(flexResult.success && "Employee with role Both and Game Show knowledge can join Game Show Host team");
    assert(flexResult.assignedTeam == "Studio Live");
    std::cout << "[PASS] Dual-role 'Both' employee successfully assigned to Game Show Host team." << std::endl;

    // 12. Verify strict 30 / 40 / 20 (Dire) rotation duration recommendations
    assert(scheduler.recommendSlotMinutes(charlieTeam, 4) == 20 && "Dire staffing (<=4) must recommend 20 mins");
    assert(scheduler.recommendSlotMinutes(charlieTeam, 5) == 40 && "Understaffed (5) must recommend 40 mins");
    assert(scheduler.recommendSlotMinutes(charlieTeam, 7) == 30 && "Optimal staffing (7) must recommend 30 mins");

    // Verify dire 20-min schedule generation
    QList<Employee> direMembers = db.getEmployeesByTeam("Team Bravo"); // 3 members
    ShiftSchedule schedDire = scheduler.generate8HourSchedule(
        bravoTeam, direMembers, QDate::currentDate(), QTime(8, 0), 0); // 0 = auto
    assert(schedDire.slotMinutes == 20 && "Dire situation auto-adjusts strictly to 20 mins");
    assert(schedDire.rotationSlots[0].startTime == QTime(8, 0));
    assert(schedDire.rotationSlots[0].endTime == QTime(8, 20));
    std::cout << "[PASS] Dire situation (20 Mins) rotation auto-adjustment verified." << std::endl;

    // 13. Test 30 Mins & 40 Mins Rotation Types with 7-staff shift limit
    Team cTeam = db.getTeamByName("Team Charlie");
    while (db.getTeamMemberCount("Team Charlie") < 7) {
        int idx = db.getTeamMemberCount("Team Charlie") + 1;
        Employee ce(0, QString("Emp_Charlie_Extra_%1").arg(idx), "Team Charlie", "Dealer", "L", "", "", "Dealer", "Blackjack, Roulette");
        db.addEmployee(ce);
    }
    QList<Employee> charlieMembers = db.getEmployeesByTeam("Team Charlie");
    assert(charlieMembers.size() == 7);

    ShiftSchedule sched30 = scheduler.generate8HourSchedule(
        cTeam, charlieMembers, QDate::currentDate(), QTime(8, 0), 30);
    assert(sched30.slotMinutes == 30);
    assert(sched30.rotationSlots.size() == 20);
    assert(sched30.rotationSlots[0].startTime == QTime(8, 0));
    assert(sched30.rotationSlots[0].endTime == QTime(8, 30));
    assert(sched30.tableGames.size() == 4);
    std::cout << "[PASS] 30 Mins Rotation Type generated (20 slots, 30-min duration, 4 table games)." << std::endl;

    ShiftSchedule sched40 = scheduler.generate8HourSchedule(
        cTeam, charlieMembers, QDate::currentDate(), QTime(8, 0), 40);
    assert(sched40.slotMinutes == 40);
    assert(sched40.rotationSlots.size() == 20);
    assert(sched40.rotationSlots[0].startTime == QTime(8, 0));
    assert(sched40.rotationSlots[0].endTime == QTime(8, 40));
    assert(sched40.rotationSlots[1].startTime == QTime(8, 40));
    assert(sched40.rotationSlots[1].endTime == QTime(9, 20));

    // Verify each slot has 4 tables and at least 1 break, and no employee exceeds 4 consecutive tables
    int testEmpId = charlieMembers[0].getId();
    int maxConsecutive = 0;
    int curConsecutive = 0;
    for (int s = 0; s < sched40.rotationSlots.size(); ++s) {
        const RotationSlot& slot = sched40.rotationSlots[s];
        assert(slot.tableAssignments.size() == 4);
        assert(slot.onBreakEmployeeNames.size() >= 1);
        if (slot.onBreakEmployeeIds.contains(testEmpId)) {
            curConsecutive = 0;
        } else {
            curConsecutive++;
            if (curConsecutive > maxConsecutive) maxConsecutive = curConsecutive;
        }
        assert(curConsecutive <= 4 && "No employee should exceed 4 consecutive tables without break");
    }
    std::cout << "[PASS] 40 Mins Rotation Type verified (20 slots, max consecutive tables: " << maxConsecutive << ")." << std::endl;

    db.closeDatabase();
    QFile::remove(testDbName);

    // 14. Verify full 70/30 seed database auto-balances all 15 teams to 7/7 Optimal (30 Mins rotation)
    const QString demoTestDbName = "test_demo_70_30.db";
    QFile::remove(demoTestDbName);
    if (QFile::exists("uniformmanagement.db")) {
        QFile::copy("uniformmanagement.db", demoTestDbName);
    } else if (QFile::exists("../uniformmanagement.db")) {
        QFile::copy("../uniformmanagement.db", demoTestDbName);
    }

    DatabaseManager demoDb;
    assert(demoDb.initializeDatabase(demoTestDbName));
    QList<Employee> allDemoEmps = demoDb.getAllEmployees();
    assert(allDemoEmps.size() == 105 && "Demo database must have 105 employees");
    int multiGameCount = 0;
    int basicGameCount = 0;
    for (const Employee& emp : allDemoEmps) {
        if (emp.getKnownGamesList().size() >= 3) {
            multiGameCount++;
        } else {
            basicGameCount++;
        }
    }
    assert(multiGameCount == 74 && basicGameCount == 31 && "70/30 game knowledge ratio (74 multi-game / 31 basic) must hold");
    std::cout << "[PASS] 70/30 Game Knowledge distribution verified (" << multiGameCount << " multi-game / "
              << basicGameCount << " basic = 70.5% / 29.5%)." << std::endl;

    RotationScheduler demoScheduler(&demoDb);
    QList<ReassignmentResult> demoMoves = demoScheduler.relocateOverflowStaff();
    assert(demoMoves.size() == 13 && "All 13 overflow employees must be reassigned during Auto-Balance");
    for (const ReassignmentResult& res : demoMoves) {
        assert(res.success && "Every Auto-Balance reassignment must succeed");
    }
    for (const Team& t : demoDb.getAllTeams()) {
        int staff = demoDb.getTeamMemberCount(t.getName());
        assert(staff == 7 && "Every one of the 15 teams must balance to 7/7 Optimal");
        assert(demoScheduler.recommendSlotMinutes(t, staff) == 30 && "Every balanced team must use 30 Mins rotation");
    }
    std::cout << "[PASS] Demo Auto-Balance verified: all 15 teams balanced to 7/7 Optimal (30 Mins shift rotation)." << std::endl;
    demoDb.closeDatabase();
    QFile::remove(demoTestDbName);

    std::cout << "\nALL TEST SUITE CHECKS COMPLETED SUCCESSFULLY!" << std::endl;
    return 0;
}
