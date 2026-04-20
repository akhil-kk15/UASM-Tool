#include "uniformissue.h"
#include <QDate>

UniformIssue::UniformIssue()
    : m_id(-1), m_employeeId(-1), m_uniformId(-1), m_quantity(0), m_status("Pending")
{
    m_issueDate = QDate::currentDate();
}

UniformIssue::UniformIssue(int id, int employeeId, int uniformId, int quantity, 
                           const QDate& issueDate, const QString& status)
    : m_id(id), m_employeeId(employeeId), m_uniformId(uniformId), 
      m_quantity(quantity), m_issueDate(issueDate), m_status(status)
{
}

int UniformIssue::getId() const { return m_id; }
int UniformIssue::getEmployeeId() const { return m_employeeId; }
int UniformIssue::getUniformId() const { return m_uniformId; }
int UniformIssue::getQuantity() const { return m_quantity; }
QDate UniformIssue::getIssueDate() const { return m_issueDate; }
QDate UniformIssue::getReturnDate() const { return m_returnDate; }
QString UniformIssue::getStatus() const { return m_status; }

void UniformIssue::setId(int id) { m_id = id; }
void UniformIssue::setEmployeeId(int employeeId) { m_employeeId = employeeId; }
void UniformIssue::setUniformId(int uniformId) { m_uniformId = uniformId; }
void UniformIssue::setQuantity(int quantity) { m_quantity = quantity; }
void UniformIssue::setIssueDate(const QDate& date) { m_issueDate = date; }
void UniformIssue::setReturnDate(const QDate& date) { m_returnDate = date; }
void UniformIssue::setStatus(const QString& status) { m_status = status; }
