#include "databasemanager.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QDate>
#include <QDebug>

DatabaseManager::DatabaseManager() : m_lastError("")
{
}

DatabaseManager::~DatabaseManager()
{
    closeDatabase();
}

bool DatabaseManager::initializeDatabase(const QString& dbPath)
{
    const bool isDefaultDb = dbPath.isEmpty() || dbPath == QStringLiteral("uniformmanagement.db");
    QString resolvedPath = isDefaultDb ? QStringLiteral("uniformmanagement.db") : dbPath;

    if (isDefaultDb && !QFileInfo::exists(resolvedPath)) {
        const QString appDir = QCoreApplication::applicationDirPath();
        const QStringList candidatePaths = {
            QDir(appDir).filePath(QStringLiteral("uniformmanagement.db")),
            QDir(appDir).filePath(QStringLiteral("../uniformmanagement.db")),
            QDir(appDir).filePath(QStringLiteral("../../uniformmanagement.db")),
            QDir::current().filePath(QStringLiteral("../uniformmanagement.db")),
            QDir::current().filePath(QStringLiteral("../../uniformmanagement.db"))
        };

        bool foundExisting = false;
        for (const QString& candidate : candidatePaths) {
            QFileInfo info(candidate);
            if (info.exists() && info.isFile()) {
                foundExisting = true;
                const QString targetInAppDir = QDir(appDir).filePath(QStringLiteral("uniformmanagement.db"));
                if (!appDir.isEmpty() && QFileInfo(info.canonicalFilePath()) != QFileInfo(targetInAppDir)) {
                    QFile::copy(info.canonicalFilePath(), targetInAppDir);
                }
                if (QFileInfo::exists(targetInAppDir)) {
                    resolvedPath = targetInAppDir;
                } else {
                    resolvedPath = info.canonicalFilePath();
                }
                break;
            }
        }

        if (!foundExisting && !appDir.isEmpty()) {
            resolvedPath = QDir(appDir).filePath(QStringLiteral("uniformmanagement.db"));
        }
    }

    if (QSqlDatabase::contains(QSqlDatabase::defaultConnection)) {
        m_database = QSqlDatabase::database(QSqlDatabase::defaultConnection, false);
        if (m_database.isOpen()) {
            m_database.close();
        }
    } else {
        m_database = QSqlDatabase::addDatabase("QSQLITE");
    }
    m_database.setDatabaseName(resolvedPath);

    if (!m_database.open()) {
        m_lastError = "Failed to open database: " + m_database.lastError().text();
        return false;
    }

    if (!createTables()) {
        m_lastError = "Failed to create tables: " + m_lastError;
        return false;
    }

    upgradeTablesIfNeeded();

    if (isDefaultDb) {
        int uniformCount = 0;
        int teamCount = 0;
        int minEmpId = 0;
        QSqlQuery countQuery("SELECT COUNT(*) FROM uniforms");
        if (countQuery.next()) uniformCount = countQuery.value(0).toInt();
        QSqlQuery teamCountQuery("SELECT COUNT(*) FROM teams");
        if (teamCountQuery.next()) teamCount = teamCountQuery.value(0).toInt();
        QSqlQuery minEmpQuery("SELECT COALESCE(MIN(id), 0) FROM employees");
        if (minEmpQuery.next()) minEmpId = minEmpQuery.value(0).toInt();

        if (uniformCount == 0 || teamCount < 15 || (minEmpId > 0 && minEmpId < 10000)) {
            const QString appDir = QCoreApplication::applicationDirPath();
            const QStringList sqlCandidates = {
                QDir::current().filePath(QStringLiteral("seed_test_data.sql")),
                QDir::current().filePath(QStringLiteral("../seed_test_data.sql")),
                QDir::current().filePath(QStringLiteral("../../seed_test_data.sql")),
                QDir(appDir).filePath(QStringLiteral("seed_test_data.sql")),
                QDir(appDir).filePath(QStringLiteral("../seed_test_data.sql")),
                QDir(appDir).filePath(QStringLiteral("../../seed_test_data.sql"))
            };
            for (const QString& sqlPath : sqlCandidates) {
                QFile sqlFile(sqlPath);
                if (sqlFile.exists() && sqlFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
                    const QString sqlContent = QString::fromUtf8(sqlFile.readAll());
                    sqlFile.close();
                    QStringList cleanedLines;
                    for (const QString& line : sqlContent.split('\n')) {
                        const QString trimmed = line.trimmed();
                        if (!trimmed.startsWith(QStringLiteral("--"))) {
                            cleanedLines.append(line);
                        }
                    }
                    const QStringList statements = cleanedLines.join('\n').split(';', Qt::SkipEmptyParts);
                    QSqlQuery seedQuery;
                    for (const QString& stmt : statements) {
                        const QString s = stmt.trimmed();
                        if (!s.isEmpty()) {
                            seedQuery.exec(s);
                        }
                    }
                    break;
                }
            }
        }
    }

    seedDefaultTeamsIfEmpty();

    return true;
}

void DatabaseManager::closeDatabase()
{
    if (m_database.isOpen()) {
        m_database.close();
    }
}

bool DatabaseManager::createTables()
{
    QSqlQuery query;

    // Create Employees table
    if (!query.exec("CREATE TABLE IF NOT EXISTS employees ("
                    "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                    "name TEXT NOT NULL,"
                    "department TEXT,"
                    "designation TEXT,"
                    "uniform_size TEXT DEFAULT 'M',"
                    "email TEXT,"
                    "phone TEXT,"
                    "role_type TEXT DEFAULT 'Dealer',"
                    "game_knowledge TEXT DEFAULT ''"
                    ")")) {
        m_lastError = query.lastError().text();
        return false;
    }

    // Create Uniforms table
    if (!query.exec("CREATE TABLE IF NOT EXISTS uniforms ("
                    "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                    "type TEXT NOT NULL,"
                    "quantity INTEGER,"
                    "size TEXT,"
                    "color TEXT,"
                    "description TEXT,"
                    "team TEXT DEFAULT ''"
                    ")")) {
        m_lastError = query.lastError().text();
        return false;
    }

    // Create Uniform Issues table
    if (!query.exec("CREATE TABLE IF NOT EXISTS uniform_issues ("
                    "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                    "employee_id INTEGER NOT NULL,"
                    "uniform_id INTEGER NOT NULL,"
                    "quantity INTEGER,"
                    "issue_date DATE,"
                    "return_date DATE,"
                    "status TEXT DEFAULT 'Issued',"
                    "FOREIGN KEY(employee_id) REFERENCES employees(id),"
                    "FOREIGN KEY(uniform_id) REFERENCES uniforms(id)"
                    ")")) {
        m_lastError = query.lastError().text();
        return false;
    }

    // Create Teams table
    if (!query.exec("CREATE TABLE IF NOT EXISTS teams ("
                    "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                    "name TEXT UNIQUE NOT NULL,"
                    "uniform_type TEXT,"
                    "target_staff INTEGER DEFAULT 7,"
                    "min_staff INTEGER DEFAULT 5,"
                    "max_staff INTEGER DEFAULT 7,"
                    "team_type TEXT DEFAULT 'Standard',"
                    "table_games TEXT DEFAULT 'Blackjack, Blackjack, Blackjack, Blackjack'"
                    ")")) {
        m_lastError = query.lastError().text();
        return false;
    }

    // Create Shift Schedules table
    if (!query.exec("CREATE TABLE IF NOT EXISTS team_schedules ("
                    "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                    "team_name TEXT NOT NULL,"
                    "shift_date DATE NOT NULL,"
                    "slot_index INTEGER NOT NULL,"
                    "start_time TEXT,"
                    "end_time TEXT,"
                    "table_assignments TEXT,"
                    "break_employees TEXT"
                    ")")) {
        m_lastError = query.lastError().text();
        return false;
    }

    return true;
}

bool DatabaseManager::upgradeTablesIfNeeded()
{
    QSqlQuery query;
    // Safely check and add columns if upgrading from old schema
    query.exec("ALTER TABLE employees ADD COLUMN designation TEXT;");
    query.exec("ALTER TABLE employees ADD COLUMN uniform_size TEXT DEFAULT 'M';");
    query.exec("ALTER TABLE employees ADD COLUMN email TEXT;");
    query.exec("ALTER TABLE employees ADD COLUMN phone TEXT;");
    query.exec("ALTER TABLE employees ADD COLUMN role_type TEXT DEFAULT 'Dealer';");
    query.exec("ALTER TABLE employees ADD COLUMN game_knowledge TEXT DEFAULT '';");
    query.exec("ALTER TABLE uniforms ADD COLUMN team TEXT DEFAULT '';");
    query.exec("ALTER TABLE teams ADD COLUMN team_type TEXT DEFAULT 'Standard';");
    query.exec("ALTER TABLE teams ADD COLUMN table_games TEXT DEFAULT 'Blackjack, Blackjack, Blackjack, Blackjack';");
    return true;
}

void DatabaseManager::seedDefaultTeamsIfEmpty()
{
    QSqlQuery checkQuery("SELECT COUNT(*) FROM teams");
    if (checkQuery.next() && checkQuery.value(0).toInt() == 0) {
        addTeam(Team(1,  "Team Alpha",    "Team Alpha Vest",    7, 5, 7, "Dealer",         "Blackjack, Blackjack, Blackjack, Blackjack"));
        addTeam(Team(2,  "Team Bravo",    "Team Bravo Vest",    7, 5, 7, "Dealer",         "Blackjack, Blackjack, Blackjack, Blackjack"));
        addTeam(Team(3,  "Team Charlie",  "Team Charlie Vest",  7, 5, 7, "Dealer",         "Blackjack, Blackjack, Roulette, Roulette"));
        addTeam(Team(4,  "Team Delta",    "Team Delta Vest",    7, 5, 7, "Dealer",         "Blackjack, Roulette, Blackjack, Roulette"));
        addTeam(Team(5,  "Team Echo",     "Team Echo Vest",     7, 5, 7, "Dealer",         "Blackjack, Roulette, Baccarat, Baccarat"));
        addTeam(Team(6,  "Team Foxtrot",  "Team Foxtrot Vest",  7, 5, 7, "Dealer",         "Baccarat, Baccarat, Roulette, Blackjack"));
        addTeam(Team(7,  "Team Golf",     "Team Golf Vest",     7, 5, 7, "Dealer",         "Poker, Poker, Blackjack, Roulette"));
        addTeam(Team(8,  "Team Hotel",    "Team Hotel Vest",    7, 5, 7, "Dealer",         "Blackjack, Baccarat, Poker, Roulette"));
        addTeam(Team(9,  "Team India",    "Team India Vest",    7, 5, 7, "Dealer",         "Craps, Craps, Blackjack, Roulette"));
        addTeam(Team(10, "Team Juliet",   "Team Juliet Vest",   7, 5, 7, "Dealer",         "Baccarat, Poker, Craps, Blackjack"));
        addTeam(Team(11, "Team Kilo",     "Team Kilo Vest",     7, 5, 7, "Dealer",         "Roulette, Roulette, Baccarat, Poker"));
        addTeam(Team(12, "Team Lima",     "Team Lima Vest",     7, 5, 7, "Dealer",         "Poker, Craps, Baccarat, Roulette"));
        addTeam(Team(13, "Team Mike",     "Team Mike Vest",     7, 5, 7, "Dealer",         "Blackjack, Roulette, Poker, Craps"));
        addTeam(Team(14, "Studio Live",   "Studio Live Suit",   7, 5, 7, "Game Show Host", "Game Show, Game Show, Game Show, Game Show"));
        addTeam(Team(15, "Studio Royale", "Studio Royale Suit", 7, 5, 7, "Game Show Host", "Game Show, Game Show, Game Show, Game Show"));
    }
}

// Employee operations
bool DatabaseManager::addEmployee(const Employee& employee)
{
    int nextId = employee.getId();
    if (nextId < 10000 || nextId > 99999) {
        QSqlQuery maxQuery("SELECT COALESCE(MAX(id), 10000) + 1 FROM employees");
        if (maxQuery.next()) {
            nextId = maxQuery.value(0).toInt();
        }
        if (nextId < 10001) {
            nextId = 10001;
        }
    }

    QSqlQuery query;
    query.prepare("INSERT INTO employees (id, name, department, designation, uniform_size, email, phone, role_type, game_knowledge) "
                  "VALUES (:id, :name, :department, :designation, :uniform_size, :email, :phone, :role_type, :game_knowledge)");
    query.bindValue(":id", nextId);
    query.bindValue(":name", employee.getName());
    query.bindValue(":department", employee.getDepartment());
    query.bindValue(":designation", employee.getDesignation());
    query.bindValue(":uniform_size", employee.getUniformSize().isEmpty() ? "M" : employee.getUniformSize());
    query.bindValue(":email", employee.getEmail());
    query.bindValue(":phone", employee.getPhone());
    query.bindValue(":role_type", employee.getRoleType().isEmpty() ? "Dealer" : employee.getRoleType());
    query.bindValue(":game_knowledge", employee.getGameKnowledge());

    if (!query.exec()) {
        m_lastError = query.lastError().text();
        return false;
    }
    return true;
}

bool DatabaseManager::updateEmployee(const Employee& employee)
{
    QSqlQuery query;
    query.prepare("UPDATE employees SET name=:name, department=:department, designation=:designation, "
                  "uniform_size=:uniform_size, email=:email, phone=:phone, role_type=:role_type, "
                  "game_knowledge=:game_knowledge WHERE id=:id");
    query.bindValue(":name", employee.getName());
    query.bindValue(":department", employee.getDepartment());
    query.bindValue(":designation", employee.getDesignation());
    query.bindValue(":uniform_size", employee.getUniformSize().isEmpty() ? "M" : employee.getUniformSize());
    query.bindValue(":email", employee.getEmail());
    query.bindValue(":phone", employee.getPhone());
    query.bindValue(":role_type", employee.getRoleType().isEmpty() ? "Dealer" : employee.getRoleType());
    query.bindValue(":game_knowledge", employee.getGameKnowledge());
    query.bindValue(":id", employee.getId());

    if (!query.exec()) {
        m_lastError = query.lastError().text();
        return false;
    }
    return true;
}

bool DatabaseManager::updateEmployeeTeam(int employeeId, const QString& newTeam)
{
    QSqlQuery query;
    query.prepare("UPDATE employees SET department=:department WHERE id=:id");
    query.bindValue(":department", newTeam);
    query.bindValue(":id", employeeId);

    if (!query.exec()) {
        m_lastError = query.lastError().text();
        return false;
    }
    return true;
}

bool DatabaseManager::deleteEmployee(int employeeId)
{
    QSqlQuery query;
    query.prepare("DELETE FROM employees WHERE id=:id");
    query.bindValue(":id", employeeId);

    if (!query.exec()) {
        m_lastError = query.lastError().text();
        return false;
    }
    return true;
}

Employee DatabaseManager::getEmployee(int employeeId)
{
    QSqlQuery query;
    query.prepare("SELECT * FROM employees WHERE id=:id");
    query.bindValue(":id", employeeId);

    if (query.exec() && query.next()) {
        Employee employee;
        employee.setId(query.value("id").toInt());
        employee.setName(query.value("name").toString());
        employee.setDepartment(query.value("department").toString());
        employee.setDesignation(query.value("designation").toString());
        employee.setUniformSize(query.value("uniform_size").toString());
        employee.setEmail(query.value("email").toString());
        employee.setPhone(query.value("phone").toString());
        QString role = query.value("role_type").toString();
        employee.setRoleType(role.isEmpty() ? "Dealer" : role);
        employee.setGameKnowledge(query.value("game_knowledge").toString());
        return employee;
    }
    m_lastError = query.lastError().text();
    return Employee();
}

QList<Employee> DatabaseManager::getAllEmployees()
{
    QList<Employee> employees;
    QSqlQuery query("SELECT * FROM employees");

    while (query.next()) {
        Employee employee;
        employee.setId(query.value("id").toInt());
        employee.setName(query.value("name").toString());
        employee.setDepartment(query.value("department").toString());
        employee.setDesignation(query.value("designation").toString());
        employee.setUniformSize(query.value("uniform_size").toString());
        employee.setEmail(query.value("email").toString());
        employee.setPhone(query.value("phone").toString());
        QString role = query.value("role_type").toString();
        employee.setRoleType(role.isEmpty() ? "Dealer" : role);
        employee.setGameKnowledge(query.value("game_knowledge").toString());
        employees.append(employee);
    }

    return employees;
}

QList<Employee> DatabaseManager::getEmployeesByTeam(const QString& teamName)
{
    QList<Employee> employees;
    QSqlQuery query;
    query.prepare("SELECT * FROM employees WHERE department=:dept");
    query.bindValue(":dept", teamName);

    if (query.exec()) {
        while (query.next()) {
            Employee employee;
            employee.setId(query.value("id").toInt());
            employee.setName(query.value("name").toString());
            employee.setDepartment(query.value("department").toString());
            employee.setDesignation(query.value("designation").toString());
            employee.setUniformSize(query.value("uniform_size").toString());
            employee.setEmail(query.value("email").toString());
            employee.setPhone(query.value("phone").toString());
            QString role = query.value("role_type").toString();
            employee.setRoleType(role.isEmpty() ? "Dealer" : role);
            employee.setGameKnowledge(query.value("game_knowledge").toString());
            employees.append(employee);
        }
    }
    return employees;
}

// Team operations
bool DatabaseManager::addTeam(const Team& team)
{
    QSqlQuery query;
    query.prepare("INSERT INTO teams (name, uniform_type, target_staff, min_staff, max_staff, team_type, table_games) "
                  "VALUES (:name, :uniform_type, :target_staff, :min_staff, :max_staff, :team_type, :table_games)");
    query.bindValue(":name", team.getName());
    query.bindValue(":uniform_type", team.getUniformType());
    query.bindValue(":target_staff", team.getTargetStaff());
    query.bindValue(":min_staff", team.getMinStaff());
    query.bindValue(":max_staff", team.getMaxStaff());
    query.bindValue(":team_type", team.getTeamType().isEmpty() ? "Standard" : team.getTeamType());
    query.bindValue(":table_games", team.getTableGames());

    if (!query.exec()) {
        m_lastError = query.lastError().text();
        return false;
    }
    return true;
}

bool DatabaseManager::updateTeam(const Team& team)
{
    QSqlQuery query;
    query.prepare("UPDATE teams SET name=:name, uniform_type=:uniform_type, target_staff=:target_staff, "
                  "min_staff=:min_staff, max_staff=:max_staff, team_type=:team_type, table_games=:table_games WHERE id=:id");
    query.bindValue(":name", team.getName());
    query.bindValue(":uniform_type", team.getUniformType());
    query.bindValue(":target_staff", team.getTargetStaff());
    query.bindValue(":min_staff", team.getMinStaff());
    query.bindValue(":max_staff", team.getMaxStaff());
    query.bindValue(":team_type", team.getTeamType().isEmpty() ? "Standard" : team.getTeamType());
    query.bindValue(":table_games", team.getTableGames());
    query.bindValue(":id", team.getId());

    if (!query.exec()) {
        m_lastError = query.lastError().text();
        return false;
    }
    return true;
}

bool DatabaseManager::deleteTeam(int teamId)
{
    QSqlQuery query;
    query.prepare("DELETE FROM teams WHERE id=:id");
    query.bindValue(":id", teamId);

    if (!query.exec()) {
        m_lastError = query.lastError().text();
        return false;
    }
    return true;
}

Team DatabaseManager::getTeam(int teamId)
{
    QSqlQuery query;
    query.prepare("SELECT * FROM teams WHERE id=:id");
    query.bindValue(":id", teamId);

    if (query.exec() && query.next()) {
        Team team;
        team.setId(query.value("id").toInt());
        team.setName(query.value("name").toString());
        team.setUniformType(query.value("uniform_type").toString());
        team.setTargetStaff(query.value("target_staff").toInt());
        team.setMinStaff(query.value("min_staff").toInt());
        team.setMaxStaff(query.value("max_staff").toInt());
        QString type = query.value("team_type").toString();
        team.setTeamType(type.isEmpty() ? "Standard" : type);
        team.setTableGames(query.value("table_games").toString());
        return team;
    }
    m_lastError = query.lastError().text();
    return Team();
}

Team DatabaseManager::getTeamByName(const QString& teamName)
{
    QSqlQuery query;
    query.prepare("SELECT * FROM teams WHERE name=:name");
    query.bindValue(":name", teamName);

    if (query.exec() && query.next()) {
        Team team;
        team.setId(query.value("id").toInt());
        team.setName(query.value("name").toString());
        team.setUniformType(query.value("uniform_type").toString());
        team.setTargetStaff(query.value("target_staff").toInt());
        team.setMinStaff(query.value("min_staff").toInt());
        team.setMaxStaff(query.value("max_staff").toInt());
        QString type = query.value("team_type").toString();
        team.setTeamType(type.isEmpty() ? "Standard" : type);
        team.setTableGames(query.value("table_games").toString());
        return team;
    }
    m_lastError = query.lastError().text();
    return Team();
}

QList<Team> DatabaseManager::getAllTeams()
{
    QList<Team> teams;
    QSqlQuery query("SELECT * FROM teams ORDER BY id ASC");

    while (query.next()) {
        Team team;
        team.setId(query.value("id").toInt());
        team.setName(query.value("name").toString());
        team.setUniformType(query.value("uniform_type").toString());
        team.setTargetStaff(query.value("target_staff").toInt());
        team.setMinStaff(query.value("min_staff").toInt());
        team.setMaxStaff(query.value("max_staff").toInt());
        QString type = query.value("team_type").toString();
        team.setTeamType(type.isEmpty() ? "Standard" : type);
        team.setTableGames(query.value("table_games").toString());
        teams.append(team);
    }
    return teams;
}

int DatabaseManager::getTeamMemberCount(const QString& teamName)
{
    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM employees WHERE department=:dept");
    query.bindValue(":dept", teamName);
    if (query.exec() && query.next()) {
        return query.value(0).toInt();
    }
    return 0;
}

// Uniform operations
bool DatabaseManager::addUniform(const Uniform& uniform)
{
    QSqlQuery query;
    query.prepare("INSERT INTO uniforms (type, quantity, size, color, description, team) "
                  "VALUES (:type, :quantity, :size, :color, :description, :team)");
    query.bindValue(":type", uniform.getType());
    query.bindValue(":quantity", uniform.getQuantity());
    query.bindValue(":size", uniform.getSize());
    query.bindValue(":color", uniform.getColor());
    query.bindValue(":description", uniform.getDescription());
    query.bindValue(":team", uniform.getTeam());

    if (!query.exec()) {
        m_lastError = query.lastError().text();
        return false;
    }
    return true;
}

bool DatabaseManager::updateUniform(const Uniform& uniform)
{
    QSqlQuery query;
    query.prepare("UPDATE uniforms SET type=:type, quantity=:quantity, size=:size, "
                  "color=:color, description=:description, team=:team WHERE id=:id");
    query.bindValue(":type", uniform.getType());
    query.bindValue(":quantity", uniform.getQuantity());
    query.bindValue(":size", uniform.getSize());
    query.bindValue(":color", uniform.getColor());
    query.bindValue(":description", uniform.getDescription());
    query.bindValue(":team", uniform.getTeam());
    query.bindValue(":id", uniform.getId());

    if (!query.exec()) {
        m_lastError = query.lastError().text();
        return false;
    }
    return true;
}

bool DatabaseManager::deleteUniform(int uniformId)
{
    QSqlQuery query;
    query.prepare("DELETE FROM uniforms WHERE id=:id");
    query.bindValue(":id", uniformId);

    if (!query.exec()) {
        m_lastError = query.lastError().text();
        return false;
    }
    return true;
}

Uniform DatabaseManager::getUniform(int uniformId)
{
    QSqlQuery query;
    query.prepare("SELECT * FROM uniforms WHERE id=:id");
    query.bindValue(":id", uniformId);

    if (query.exec() && query.next()) {
        Uniform uniform;
        uniform.setId(query.value("id").toInt());
        uniform.setType(query.value("type").toString());
        uniform.setQuantity(query.value("quantity").toInt());
        uniform.setSize(query.value("size").toString());
        uniform.setColor(query.value("color").toString());
        uniform.setDescription(query.value("description").toString());
        uniform.setTeam(query.value("team").toString());
        return uniform;
    }
    m_lastError = query.lastError().text();
    return Uniform();
}

QList<Uniform> DatabaseManager::getAllUniforms()
{
    QList<Uniform> uniforms;
    QSqlQuery query("SELECT * FROM uniforms");

    while (query.next()) {
        Uniform uniform;
        uniform.setId(query.value("id").toInt());
        uniform.setType(query.value("type").toString());
        uniform.setQuantity(query.value("quantity").toInt());
        uniform.setSize(query.value("size").toString());
        uniform.setColor(query.value("color").toString());
        uniform.setDescription(query.value("description").toString());
        uniform.setTeam(query.value("team").toString());
        uniforms.append(uniform);
    }
    return uniforms;
}

int DatabaseManager::getIssuedUniformCount(int uniformId)
{
    QSqlQuery query;
    query.prepare("SELECT SUM(quantity) FROM uniform_issues WHERE uniform_id=:uid AND status='Issued'");
    query.bindValue(":uid", uniformId);
    if (query.exec() && query.next()) {
        return query.value(0).toInt();
    }
    return 0;
}

int DatabaseManager::getAvailableStockForUniform(int uniformId)
{
    Uniform uni = getUniform(uniformId);
    if (uni.getId() <= 0) return 0;
    int issued = getIssuedUniformCount(uniformId);
    int available = uni.getQuantity() - issued;
    return (available > 0) ? available : 0;
}

int DatabaseManager::getAvailableUniformCountForTeam(const QString& teamName, const QString& uniformType, const QString& size)
{
    QSqlQuery query;
    query.prepare("SELECT id, quantity FROM uniforms WHERE (team=:team OR type=:type OR team='' OR team='All') AND size=:size");
    query.bindValue(":team", teamName);
    query.bindValue(":type", uniformType);
    query.bindValue(":size", size);

    int totalAvailable = 0;
    if (query.exec()) {
        while (query.next()) {
            int uid = query.value("id").toInt();
            int total = query.value("quantity").toInt();
            int issued = getIssuedUniformCount(uid);
            int avail = total - issued;
            if (avail > 0) {
                totalAvailable += avail;
            }
        }
    }
    return totalAvailable;
}

Uniform DatabaseManager::findAvailableUniformForTeam(const QString& teamName, const QString& uniformType, const QString& size)
{
    QSqlQuery query;
    query.prepare("SELECT id, type, quantity, size, color, description, team FROM uniforms "
                  "WHERE (team=:team OR type=:type OR team='' OR team='All') AND size=:size");
    query.bindValue(":team", teamName);
    query.bindValue(":type", uniformType);
    query.bindValue(":size", size);

    if (query.exec()) {
        while (query.next()) {
            int uid = query.value("id").toInt();
            int total = query.value("quantity").toInt();
            int issued = getIssuedUniformCount(uid);
            if (total - issued > 0) {
                Uniform u;
                u.setId(uid);
                u.setType(query.value("type").toString());
                u.setQuantity(total);
                u.setSize(query.value("size").toString());
                u.setColor(query.value("color").toString());
                u.setDescription(query.value("description").toString());
                u.setTeam(query.value("team").toString());
                return u;
            }
        }
    }
    return Uniform();
}

// Uniform Issue operations
bool DatabaseManager::issueUniform(const UniformIssue& issue)
{
    QSqlQuery query;
    query.prepare("INSERT INTO uniform_issues (employee_id, uniform_id, quantity, issue_date, status) "
                  "VALUES (:employee_id, :uniform_id, :quantity, :issue_date, :status)");
    query.bindValue(":employee_id", issue.getEmployeeId());
    query.bindValue(":uniform_id", issue.getUniformId());
    query.bindValue(":quantity", issue.getQuantity());
    query.bindValue(":issue_date", issue.getIssueDate());
    query.bindValue(":status", issue.getStatus());

    if (!query.exec()) {
        m_lastError = query.lastError().text();
        return false;
    }
    return true;
}

bool DatabaseManager::returnUniform(int issueId, const QString& returnDate)
{
    QSqlQuery query;
    query.prepare("UPDATE uniform_issues SET return_date=:return_date, status='Returned' WHERE id=:id");
    query.bindValue(":return_date", returnDate);
    query.bindValue(":id", issueId);

    if (!query.exec()) {
        m_lastError = query.lastError().text();
        return false;
    }
    return true;
}

QList<UniformIssue> DatabaseManager::getIssueHistory(int employeeId)
{
    QList<UniformIssue> issues;
    QSqlQuery query;
    query.prepare("SELECT * FROM uniform_issues WHERE employee_id=:employee_id");
    query.bindValue(":employee_id", employeeId);

    if (query.exec()) {
        while (query.next()) {
            UniformIssue issue;
            issue.setId(query.value("id").toInt());
            issue.setEmployeeId(query.value("employee_id").toInt());
            issue.setUniformId(query.value("uniform_id").toInt());
            issue.setQuantity(query.value("quantity").toInt());
            issue.setIssueDate(query.value("issue_date").toDate());
            issue.setReturnDate(query.value("return_date").toDate());
            issue.setStatus(query.value("status").toString());
            issues.append(issue);
        }
    } else {
        m_lastError = query.lastError().text();
    }
    return issues;
}

QList<UniformIssue> DatabaseManager::getPendingIssues()
{
    QList<UniformIssue> issues;
    QSqlQuery query("SELECT * FROM uniform_issues WHERE status='Issued'");

    while (query.next()) {
        UniformIssue issue;
        issue.setId(query.value("id").toInt());
        issue.setEmployeeId(query.value("employee_id").toInt());
        issue.setUniformId(query.value("uniform_id").toInt());
        issue.setQuantity(query.value("quantity").toInt());
        issue.setIssueDate(query.value("issue_date").toDate());
        issue.setReturnDate(query.value("return_date").toDate());
        issue.setStatus(query.value("status").toString());
        issues.append(issue);
    }
    return issues;
}

QList<UniformIssue> DatabaseManager::getAllIssues()
{
    QList<UniformIssue> issues;
    QSqlQuery query("SELECT * FROM uniform_issues");

    while (query.next()) {
        UniformIssue issue;
        issue.setId(query.value("id").toInt());
        issue.setEmployeeId(query.value("employee_id").toInt());
        issue.setUniformId(query.value("uniform_id").toInt());
        issue.setQuantity(query.value("quantity").toInt());
        issue.setIssueDate(query.value("issue_date").toDate());
        issue.setReturnDate(query.value("return_date").toDate());
        issue.setStatus(query.value("status").toString());
        issues.append(issue);
    }
    return issues;
}

// Shift schedule persistence
bool DatabaseManager::clearShiftSchedule(const QString& teamName, const QDate& date)
{
    QSqlQuery query;
    query.prepare("DELETE FROM team_schedules WHERE team_name=:team AND shift_date=:date");
    query.bindValue(":team", teamName);
    query.bindValue(":date", date);
    return query.exec();
}

bool DatabaseManager::saveScheduleSlot(const QString& teamName, const QDate& date, int slotIndex,
                                      const QString& startTime, const QString& endTime,
                                      const QString& tableAssignments, const QString& breakEmployees)
{
    QSqlQuery query;
    query.prepare("INSERT INTO team_schedules (team_name, shift_date, slot_index, start_time, end_time, table_assignments, break_employees) "
                  "VALUES (:team, :date, :slot, :st, :et, :tables, :breaks)");
    query.bindValue(":team", teamName);
    query.bindValue(":date", date);
    query.bindValue(":slot", slotIndex);
    query.bindValue(":st", startTime);
    query.bindValue(":et", endTime);
    query.bindValue(":tables", tableAssignments);
    query.bindValue(":breaks", breakEmployees);
    return query.exec();
}

QString DatabaseManager::getLastError() const
{
    return m_lastError;
}
