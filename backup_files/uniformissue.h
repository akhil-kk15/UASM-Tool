#ifndef UNIFORMISSUE_H
#define UNIFORMISSUE_H

#include <QString>
#include <QDate>

class UniformIssue
{
public:
    UniformIssue();
    UniformIssue(int id, int employeeId, int uniformId, int quantity, 
                 const QDate& issueDate, const QString& status);

    // Getters
    int getId() const;
    int getEmployeeId() const;
    int getUniformId() const;
    int getQuantity() const;
    QDate getIssueDate() const;
    QDate getReturnDate() const;
    QString getStatus() const;  // "Issued", "Returned", "Pending"

    // Setters
    void setId(int id);
    void setEmployeeId(int employeeId);
    void setUniformId(int uniformId);
    void setQuantity(int quantity);
    void setIssueDate(const QDate& date);
    void setReturnDate(const QDate& date);
    void setStatus(const QString& status);

private:
    int m_id;
    int m_employeeId;
    int m_uniformId;
    int m_quantity;
    QDate m_issueDate;
    QDate m_returnDate;
    QString m_status;
};

#endif // UNIFORMISSUE_H
