#include "databasemanager.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
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
    m_database = QSqlDatabase::addDatabase("QSQLITE");
    m_database.setDatabaseName(dbPath.isEmpty() ? "uniformmanagement.db" : dbPath);

    if (!m_database.open()) {
        m_lastError = "Failed to open database: " + m_database.lastError().text();
        return false;
    }

    if (!createTables()) {
        m_lastError = "Failed to create tables: " + m_lastError;
        return false;
    }

    upgradeTablesIfNeeded();
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
                    "role_type TEXT DEFAULT 'Dealer'"
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
                    "target_staff INTEGER DEFAULT 5,"
                    "min_staff INTEGER DEFAULT 4,"
                    "max_staff INTEGER DEFAULT 7,"
                    "team_type TEXT DEFAULT 'Standard'"
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
    query.exec("ALTER TABLE uniforms ADD COLUMN team TEXT DEFAULT '';");
    query.exec("ALTER TABLE teams ADD COLUMN team_type TEXT DEFAULT 'Standard';");
    return true;
}

void DatabaseManager::seedDefaultTeamsIfEmpty()
{
    QSqlQuery checkQuery("SELECT COUNT(*) FROM teams");
    if (checkQuery.next() && checkQuery.value(0).toInt() == 0) {
        addTeam(Team(1, "Team Alpha", "Alpha Uniform", 5, 4, 7, "Standard"));
        addTeam(Team(2, "Team Beta", "Beta Uniform", 5, 4, 7, "Standard"));
        addTeam(Team(3, "Team Gamma", "Gamma Uniform", 5, 4, 7, "Standard"));
        addTeam(Team(4, "Studio Live", "Studio Gold", 5, 4, 7, "Game Show Host"));
    }
}

// Employee operations
bool DatabaseManager::addEmployee(const Employee& employee)
{
    QSqlQuery query;
    query.prepare("INSERT INTO employees (name, department, designation, uniform_size, email, phone, role_type) "
                  "VALUES (:name, :department, :designation, :uniform_size, :email, :phone, :role_type)");
    query.bindValue(":name", employee.getName());
    query.bindValue(":department", employee.getDepartment());
    query.bindValue(":designation", employee.getDesignation());
    query.bindValue(":uniform_size", employee.getUniformSize().isEmpty() ? "M" : employee.getUniformSize());
    query.bindValue(":email", employee.getEmail());
    query.bindValue(":phone", employee.getPhone());
    query.bindValue(":role_type", employee.getRoleType().isEmpty() ? "Dealer" : employee.getRoleType());

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
                  "uniform_size=:uniform_size, email=:email, phone=:phone, role_type=:role_type WHERE id=:id");
    query.bindValue(":name", employee.getName());
    query.bindValue(":department", employee.getDepartment());
    query.bindValue(":designation", employee.getDesignation());
    query.bindValue(":uniform_size", employee.getUniformSize().isEmpty() ? "M" : employee.getUniformSize());
    query.bindValue(":email", employee.getEmail());
    query.bindValue(":phone", employee.getPhone());
    query.bindValue(":role_type", employee.getRoleType().isEmpty() ? "Dealer" : employee.getRoleType());
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
            employees.append(employee);
        }
    }
    return employees;
}

// Team operations
bool DatabaseManager::addTeam(const Team& team)
{
    QSqlQuery query;
    query.prepare("INSERT INTO teams (name, uniform_type, target_staff, min_staff, max_staff, team_type) "
                  "VALUES (:name, :uniform_type, :target_staff, :min_staff, :max_staff, :team_type)");
    query.bindValue(":name", team.getName());
    query.bindValue(":uniform_type", team.getUniformType());
    query.bindValue(":target_staff", team.getTargetStaff());
    query.bindValue(":min_staff", team.getMinStaff());
    query.bindValue(":max_staff", team.getMaxStaff());
    query.bindValue(":team_type", team.getTeamType().isEmpty() ? "Standard" : team.getTeamType());

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
                  "min_staff=:min_staff, max_staff=:max_staff, team_type=:team_type WHERE id=:id");
    query.bindValue(":name", team.getName());
    query.bindValue(":uniform_type", team.getUniformType());
    query.bindValue(":target_staff", team.getTargetStaff());
    query.bindValue(":min_staff", team.getMinStaff());
    query.bindValue(":max_staff", team.getMaxStaff());
    query.bindValue(":team_type", team.getTeamType().isEmpty() ? "Standard" : team.getTeamType());
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
