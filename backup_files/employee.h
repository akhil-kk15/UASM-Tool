#ifndef EMPLOYEE_H
#define EMPLOYEE_H

#include <QString>
#include <QDate>

class Employee
{
public:
    Employee();
    Employee(int id, const QString& name, const QString& designation, 
             const QString& department, const QString& email, 
             const QString& phone, const QString& uniformSize);

    // Getters
    int getId() const;
    QString getName() const;
    QString getDesignation() const;
    QString getDepartment() const;
    QString getEmail() const;
    QString getPhone() const;
    QString getUniformSize() const;
    QDate getJoiningDate() const;
    bool isActive() const;

    // Setters
    void setId(int id);
    void setName(const QString& name);
    void setDesignation(const QString& designation);
    void setDepartment(const QString& department);
    void setEmail(const QString& email);
    void setPhone(const QString& phone);
    void setUniformSize(const QString& uniformSize);
    void setJoiningDate(const QDate& date);
    void setActive(bool active);

private:
    int m_id;
    QString m_name;
    QString m_designation;
    QString m_department;
    QString m_email;
    QString m_phone;
    QString m_uniformSize;
    QDate m_joiningDate;
    bool m_active;
};

#endif // EMPLOYEE_H
