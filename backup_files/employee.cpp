#include "employee.h"
#include <QDate>

Employee::Employee()
    : m_id(-1), m_name(""), m_designation(""), m_department(""), 
      m_email(""), m_phone(""), m_uniformSize(""), m_active(true)
{
    m_joiningDate = QDate::currentDate();
}

Employee::Employee(int id, const QString& name, const QString& designation, 
                   const QString& department, const QString& email, 
                   const QString& phone, const QString& uniformSize)
    : m_id(id), m_name(name), m_designation(designation), 
      m_department(department), m_email(email), m_phone(phone), 
      m_uniformSize(uniformSize), m_active(true)
{
    m_joiningDate = QDate::currentDate();
}

int Employee::getId() const { return m_id; }
QString Employee::getName() const { return m_name; }
QString Employee::getDesignation() const { return m_designation; }
QString Employee::getDepartment() const { return m_department; }
QString Employee::getEmail() const { return m_email; }
QString Employee::getPhone() const { return m_phone; }
QString Employee::getUniformSize() const { return m_uniformSize; }
QDate Employee::getJoiningDate() const { return m_joiningDate; }
bool Employee::isActive() const { return m_active; }

void Employee::setId(int id) { m_id = id; }
void Employee::setName(const QString& name) { m_name = name; }
void Employee::setDesignation(const QString& designation) { m_designation = designation; }
void Employee::setDepartment(const QString& department) { m_department = department; }
void Employee::setEmail(const QString& email) { m_email = email; }
void Employee::setPhone(const QString& phone) { m_phone = phone; }
void Employee::setUniformSize(const QString& uniformSize) { m_uniformSize = uniformSize; }
void Employee::setJoiningDate(const QDate& date) { m_joiningDate = date; }
void Employee::setActive(bool active) { m_active = active; }
