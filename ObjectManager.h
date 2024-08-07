#ifndef OBJECTMANAGER_H
#define OBJECTMANAGER_H

#include "Object.h"
#include <QObject>
#include <QList>
#include <QJsonDocument>
#include <QFile>
#include <QJsonObject>
#include <QJsonArray>

class ObjectManager : public QObject {
    Q_OBJECT
public:
    ObjectManager() : nextId(1) {
        loadObjects(); // Load objects from file at startup
    }

    void createObject(const QString &name) {
        Object *obj = new Object(nextId++, name);
        objects.append(obj);
        saveObjects(); // Save after creating an object
    }

    QList<Object*> listObjects() const { return objects; }

    Object* getObject(int id) {
        for (auto obj : objects) {
            if (obj->getId() == id) return obj;
        }
        return nullptr;
    }

    void updateObject(int id, const QString &name) {
        Object *obj = getObject(id);
        if (obj) {
            obj->setName(name);
            saveObjects(); // Save after updating
        }
    }

    void deleteObject(int id) {
        for (int i = 0; i < objects.size(); ++i) {
            if (objects[i]->getId() == id) {
                delete objects[i];
                objects.removeAt(i);
                saveObjects(); // Save after deleting
                break;
            }
        }
    }

    void addDynamicProperty(int id, const QString &key, const QVariant &value) {
        Object *obj = getObject(id);
        if (obj) {
            obj->setProperty(key, value);
            saveObjects(); // Save after adding property
        }
    }

    void addChildObject(int parentId, const QString &childName) {
        Object *parent = getObject(parentId);
        if (parent) {
            Object *child = new Object(nextId++, childName);
            parent->addChild(child);
            objects.append(child);
            saveObjects(); // Save after adding child object
        }
    }

private:
    QList<Object*> objects;
    int nextId;

    void saveObjects() {
        QJsonArray jsonArray;
        for (auto obj : objects) {
            QJsonObject jsonObj;
            jsonObj["id"] = obj->getId();
            jsonObj["name"] = obj->getName();
            jsonObj["properties"] = QJsonObject::fromVariantMap(obj->getProperties());

            QJsonArray childArray;
            for (auto child : obj->getChildren()) {
                childArray.append(child->getId());
            }
            jsonObj["children"] = childArray;

            jsonArray.append(jsonObj);
        }
        QJsonDocument jsonDoc(jsonArray);
        QFile file("objects.json");
        if (file.open(QFile::WriteOnly)) {
            file.write(jsonDoc.toJson());
            file.close();
        }
    }

    void loadObjects() {
        QFile file("objects.json");
        if (file.open(QFile::ReadOnly)) {
            QByteArray data = file.readAll();
            QJsonDocument jsonDoc(QJsonDocument::fromJson(data));
            if (jsonDoc.isArray()) {
                QJsonArray jsonArray = jsonDoc.array();
                for (const auto &jsonValue : jsonArray) {
                    if (jsonValue.isObject()) {
                        QJsonObject jsonObj = jsonValue.toObject();
                        createObject(jsonObj["name"].toString()); // Create object
                        int id = jsonObj["id"].toInt();
                        auto obj = getObject(id);

                        // Set dynamic properties
                        QJsonObject props = jsonObj["properties"].toObject();
                        for (const QString &key : props.keys()) {
                            obj->setProperty(key, props[key].toVariant());
                        }

                        // Set children
                        QJsonArray childArray = jsonObj["children"].toArray();
                        for (const auto &childId : childArray) {
                            obj->addChild(getObject(childId.toInt()));
                        }
                    }
                }
            }
            file.close();
        }
    }
};

#endif // OBJECTMANAGER_H
