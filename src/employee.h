#ifndef EMPLOYEE_H
#define EMPLOYEE_H

#include <QString>

class Employee
{
public:
    Employee();
    Employee(int id, const QString& name, const QString& department);

    // Getters
    int getId() const;
    QString getName() const;
    QString getDepartment() const;

    // Setters
    void setId(int id);
    void setName(const QString& name);
    void setDepartment(const QString& department);

private:
    int m_id;
    QString m_name;
    QString m_department;
};

#endif // EMPLOYEE_H
