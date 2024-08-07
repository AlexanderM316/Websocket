#ifndef OBJECTMANAGER_H
#define OBJECTMANAGER_H

#include "Object.h"
#include <QObject>
#include <QList>

class ObjectManager : public QObject {
    Q_OBJECT
public:
    ObjectManager() : nextId(1) {}

    void createObject(const QString& name) {
        objects.append(new Object(nextId++, name));
    }

    QList<Object*> listObjects() {
        return objects;
    }

    Object* getObject(int id) {
        for (auto obj : objects) {
            if (obj->getId() == id)
                return obj;
        }
        return nullptr;
    }

    void updateObject(int id, const QString& name) {
        Object* obj = getObject(id);
        if (obj) {
            obj->setName(name);
        }
    }

    void deleteObject(int id) {
        for (int i = 0; i < objects.size(); ++i) {
            if (objects[i]->getId() == id) {
                delete objects[i];
                objects.removeAt(i);
                break;
            }
        }
    }

private:
    QList<Object*> objects;
    int nextId;
};

#endif // OBJECTMANAGER_H
