#ifndef OBJECT_H
#define OBJECT_H

#include <QString>
#include <QVariantMap>
#include <QList>

class Object {
public:
    Object(int id, const QString &name) : m_id(id), m_name(name) {}

    int getId() const { return m_id; }
    QString getName() const { return m_name; }
    QVariantMap getProperties() const { return m_properties; }
    QList<Object*> getChildren() const { return m_children; }

    void setName(const QString &name) { m_name = name; }
    void setProperty(const QString &key, const QVariant &value) { m_properties[key] = value; }
    void addChild(Object *child) { m_children.append(child); }

private:
    int m_id;
    QString m_name;
    QVariantMap m_properties; // Dynamic properties
    QList<Object*> m_children; // Child objects
};

#endif // OBJECT_H
