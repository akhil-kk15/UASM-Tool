#ifndef UNIFORM_H
#define UNIFORM_H

#include <QString>

class Uniform
{
public:
    Uniform();
    Uniform(int id, const QString& type, int quantity, const QString& size, 
            const QString& color, const QString& description, const QString& team = "");

    // Getters
    int getId() const;
    QString getType() const;
    int getQuantity() const;
    QString getSize() const;
    QString getDescription() const;
    QString getColor() const;
    QString getTeam() const;

    // Setters
    void setId(int id);
    void setType(const QString& type);
    void setQuantity(int quantity);
    void setSize(const QString& size);
    void setDescription(const QString& description);
    void setColor(const QString& color);
    void setTeam(const QString& team);

private:
    int m_id;
    QString m_type;
    int m_quantity;
    QString m_size;
    QString m_description;
    QString m_color;
    QString m_team;
};

#endif // UNIFORM_H
