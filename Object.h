#ifndef OBJECT_H
#define OBJECT_H

#include <QString>
#include <QMap>

class Object {
public:
    Object(int id, const QString& name) : id(id), name(name) {}

    int getId() const { return id; }
    QString getName() const { return name; }
    void setName(const QString& newName) { name = newName; }

private:
    int id;
    QString name;
};

#endif // OBJECT_H
