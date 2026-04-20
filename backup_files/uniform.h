#ifndef UNIFORM_H
#define UNIFORM_H

#include <QString>
#include <QDate>

class Uniform
{
public:
    Uniform();
    Uniform(int id, const QString& type, int quantity, const QString& size, 
            const QDate& purchaseDate, double cost);

    // Getters
    int getId() const;
    QString getType() const;
    int getQuantity() const;
    QString getSize() const;
    QDate getPurchaseDate() const;
    QString getDescription() const;
    QString getColor() const;

    // Setters
    void setId(int id);
    void setType(const QString& type);
    void setQuantity(int quantity);
    void setSize(const QString& size);
    void setPurchaseDate(const QDate& date);
    void setDescription(const QString& description);
    void setColor(const QString& color);

private:
    int m_id;
    QString m_type;
    int m_quantity;
    QString m_size;
    QDate m_purchaseDate;
    QString m_description;
    QString m_color;
};

#endif // UNIFORM_H
