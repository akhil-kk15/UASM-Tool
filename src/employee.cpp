#include "employee.h"

Employee::Employee()
    : m_id(-1), m_name(""), m_department(""), m_designation(""), 
      m_uniformSize("M"), m_email(""), m_phone(""), m_roleType("Dealer")
{
}

Employee::Employee(int id, const QString& name, const QString& department,
                   const QString& designation, const QString& uniformSize,
                   const QString& email, const QString& phone,
                   const QString& roleType)
    : m_id(id), m_name(name), m_department(department), m_designation(designation),
      m_uniformSize(uniformSize.isEmpty() ? "M" : uniformSize),
      m_email(email), m_phone(phone),
      m_roleType(roleType.isEmpty() ? "Dealer" : roleType)
{
}

int Employee::getId() const { return m_id; }
QString Employee::getName() const { return m_name; }
QString Employee::getDepartment() const { return m_department; }
QString Employee::getTeam() const { return m_department; }
QString Employee::getDesignation() const { return m_designation; }
QString Employee::getUniformSize() const { return m_uniformSize; }
QString Employee::getEmail() const { return m_email; }
QString Employee::getPhone() const { return m_phone; }
QString Employee::getRoleType() const { return m_roleType; }

bool Employee::canDeal() const
{
    QString r = m_roleType.trimmed().toLower();
    return (r == "dealer" || r.contains("both") || r.isEmpty());
}

bool Employee::canHostGameShow() const
{
    QString r = m_roleType.trimmed().toLower();
    return (r.contains("game show") || r.contains("host") || r.contains("both"));
}

void Employee::setId(int id) { m_id = id; }
void Employee::setName(const QString& name) { m_name = name; }
void Employee::setDepartment(const QString& department) { m_department = department; }
void Employee::setTeam(const QString& team) { m_department = team; }
void Employee::setDesignation(const QString& designation) { m_designation = designation; }
void Employee::setUniformSize(const QString& uniformSize) { m_uniformSize = uniformSize; }
void Employee::setEmail(const QString& email) { m_email = email; }
void Employee::setPhone(const QString& phone) { m_phone = phone; }
void Employee::setRoleType(const QString& roleType) { m_roleType = roleType; }
