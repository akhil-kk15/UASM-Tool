#include "employee.h"

Employee::Employee()
    : m_id(-1), m_name(""), m_department("")
{
}

Employee::Employee(int id, const QString& name, const QString& department)
    : m_id(id), m_name(name), m_department(department)
{
}

int Employee::getId() const { return m_id; }
QString Employee::getName() const { return m_name; }
QString Employee::getDepartment() const { return m_department; }

void Employee::setId(int id) { m_id = id; }
void Employee::setName(const QString& name) { m_name = name; }
void Employee::setDepartment(const QString& department) { m_department = department; }
