#ifndef EMPLOYEE_H
#define EMPLOYEE_H

#include <QString>

class Employee
{
public:
    Employee();
    Employee(int id, const QString& name, const QString& department, 
             const QString& designation = "", const QString& uniformSize = "M",
             const QString& email = "", const QString& phone = "");

    // Getters
    int getId() const;
    QString getName() const;
    QString getDepartment() const;
    QString getTeam() const; // Alias for department
    QString getDesignation() const;
    QString getUniformSize() const;
    QString getEmail() const;
    QString getPhone() const;

    // Setters
    void setId(int id);
    void setName(const QString& name);
    void setDepartment(const QString& department);
    void setTeam(const QString& team);
    void setDesignation(const QString& designation);
    void setUniformSize(const QString& uniformSize);
    void setEmail(const QString& email);
    void setPhone(const QString& phone);

private:
    int m_id;
    QString m_name;
    QString m_department;
    QString m_designation;
    QString m_uniformSize;
    QString m_email;
    QString m_phone;
};

#endif // EMPLOYEE_H
