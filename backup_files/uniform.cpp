#include "uniform.h"
#include <QDate>

Uniform::Uniform()
    : m_id(-1), m_type(""), m_quantity(0), m_size(""), m_description(""), m_color("")
{
    m_purchaseDate = QDate::currentDate();
}

Uniform::Uniform(int id, const QString& type, int quantity, const QString& size, 
                 const QDate& purchaseDate)
    : m_id(id), m_type(type), m_quantity(quantity), m_size(size), 
      m_purchaseDate(purchaseDate), m_description(""), m_color("")
{
}

int Uniform::getId() const { return m_id; }
QString Uniform::getType() const { return m_type; }
int Uniform::getQuantity() const { return m_quantity; }
QString Uniform::getSize() const { return m_size; }
QDate Uniform::getPurchaseDate() const { return m_purchaseDate; }
QString Uniform::getDescription() const { return m_description; }
QString Uniform::getColor() const { return m_color; }

void Uniform::setId(int id) { m_id = id; }
void Uniform::setType(const QString& type) { m_type = type; }
void Uniform::setQuantity(int quantity) { m_quantity = quantity; }
void Uniform::setSize(const QString& size) { m_size = size; }
void Uniform::setPurchaseDate(const QDate& date) { m_purchaseDate = date; }
void Uniform::setDescription(const QString& description) { m_description = description; }
void Uniform::setColor(const QString& color) { m_color = color; }
