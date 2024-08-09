#include "Server.h"

Server::Server(quint16 port, QObject *parent)
    : QObject(parent), m_pWebSocketServer(new QWebSocketServer(QStringLiteral("JSON-RPC Server"), QWebSocketServer::NonSecureMode, this)),
      objectManager(new ObjectManager()) {
    if (m_pWebSocketServer->listen(QHostAddress::Any, port)) {
        connect(m_pWebSocketServer, &QWebSocketServer::newConnection, this, &Server::onNewConnection);
        qDebug() << "Server listening on port:" << port;
    } else {
         qDebug() << "Server failed to start:" << m_pWebSocketServer->errorString();
    }
}

void Server::onNewConnection() {
    QWebSocket *socket = m_pWebSocketServer->nextPendingConnection();
    connect(socket, &QWebSocket::textMessageReceived, this, &Server::onMessageReceived);
    connect(socket, &QWebSocket::disconnected, this, &Server::onSocketDisconnected);
}

void Server::onMessageReceived(const QString &message) {
    QWebSocket *client = qobject_cast<QWebSocket *>(sender());
    if (!client) {
        return;
    }
    QJsonDocument doc = QJsonDocument::fromJson(message.toUtf8());
    if (doc.isNull() || !doc.isObject()) {
        return sendError(client, -32700, "Parse error", QJsonValue());
    }

    QJsonObject json = doc.object();
    QString jsonrpc = json["jsonrpc"].toString();
    QString method = json["method"].toString();
    QJsonValue params = json["params"];
    QJsonValue id = json["id"];

    if (jsonrpc != "2.0") {
        return sendError(client, -32600, "Invalid Request", id);
    }

    QJsonObject result;

    if (method == "createObject") {
        if (!params.isObject() || !params["name"].isString()) {
            return sendError(client, -32602, "Invalid params", id);
        }
        QString name = params["name"].toString();
        objectManager->createObject(name);
        result["result"] = "Object created";
    } else if (method == "listObjects") {
        QJsonArray objects;
        for (Object *obj : objectManager->listObjects()) {
            QJsonObject objJson;
            objJson["id"] = obj->getId();
            objJson["name"] = obj->getName();
            objects.append(objJson);
        }
        result["result"] = objects;
    } else if (method == "updateObject") {
        if (!params.isObject() || !params["id"].isDouble() || !params["name"].isString()) {
            return sendError(client, -32602, "Invalid params", id);
        }
        int id = params["id"].toInt();
        QString name = params["name"].toString();
        objectManager->updateObject(id, name);
        result["result"] = "Object updated";
    } else if (method == "deleteObject") {
        if (!params.isObject() || !params["id"].isDouble()) {
            return sendError(client, -32602, "Invalid params", id);
        }
        int id = params["id"].toInt();
        objectManager->deleteObject(id);
        result["result"] = "Object deleted";
    } else if (method == "addDynamicProperty") {
        if (!params.isObject() || !params["id"].isDouble() || !params["key"].isString()) {
            return sendError(client, -32602, "Invalid params", id);
        }
        int id = params["id"].toInt();
        QString key = params["key"].toString();
        QVariant value = params["value"].toVariant();
        objectManager->addDynamicProperty(id, key, value);
        result["result"] = "Dynamic property added";
    } else if (method == "addChildObject") {
        if (!params.isObject() || !params["parentId"].isDouble() || !params["childName"].isString()) {
            return sendError(client, -32602, "Invalid params", id);
        }
        int parentId = params["parentId"].toInt();
        QString childName = params["childName"].toString();
        objectManager->addChildObject(parentId, childName);
        result["result"] = "Child object added";
    } else {
        return sendError(client, -32601, "Method not found", id);
    }

    result["jsonrpc"] = "2.0";
    result["id"] = id;

    client->sendTextMessage(QJsonDocument(result).toJson(QJsonDocument::Compact));
}

void Server::sendError(QWebSocket *client, int code, const QString &message, const QJsonValue &id) {
    QJsonObject error;
    error["jsonrpc"] = "2.0";
    error["error"] = QJsonObject{{"code", code}, {"message", message}};
    if (!id.isNull()) {
        error["id"] = id;
    }
    client->sendTextMessage(QJsonDocument(error).toJson(QJsonDocument::Compact));
}

void Server::onSocketDisconnected() {
    QWebSocket *socket = qobject_cast<QWebSocket *>(sender());
    if (socket) {
        socket->deleteLater();
    }
}
