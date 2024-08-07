#include "Server.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QWebSocket>

Server::Server(quint16 port, QObject *parent)
    : QObject(parent),
      m_pWebSocketServer(new QWebSocketServer(QStringLiteral("JSON-RPC Server"), QWebSocketServer::NonSecureMode, this)),
      objectManager(new ObjectManager()) {

    if (m_pWebSocketServer->listen(QHostAddress::Any, port)) {
        connect(m_pWebSocketServer, &QWebSocketServer::newConnection, this, &Server::onNewConnection);
        qDebug() << "Server listening on port:" << port;
    }
}

void Server::onNewConnection() {
    QWebSocket *socket = m_pWebSocketServer->nextPendingConnection();

    connect(socket, &QWebSocket::textMessageReceived, this, &Server::onMessageReceived);
    connect(socket, &QWebSocket::disconnected, this, &Server::onSocketDisconnected);
}

void Server::onMessageReceived(const QString &message) {
    QWebSocket *client = qobject_cast<QWebSocket*>(sender());
    if (!client) {
        return; // If sender is not a QWebSocket, exit the function.
    }

    QJsonDocument doc = QJsonDocument::fromJson(message.toUtf8());
    if (doc.isNull() || !doc.isObject()) {
        return sendError(client, -32700, "Parse error", QJsonValue()); // Invalid JSON
    }

    QJsonObject json = doc.object();
    QString jsonrpc = json["jsonrpc"].toString();
    QString method = json["method"].toString();
    QJsonValue params = json["params"];
    QJsonValue id = json["id"];

    // Check if jsonrpc is correct
    if (jsonrpc != "2.0") {
        return sendError(client, -32600, "Invalid Request", id); // Invalid JSON-RPC request
    }

    QJsonObject result;

    // Handle methods
    if (method == "createObject") {
        if (!params.isObject() || !params["name"].isString()) {
            return sendError(client, -32602, "Invalid params", id); // Invalid params
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
    } else {
        return sendError(client, -32601, "Method not found", id); // Method not found
    }

    // Send response
    result["jsonrpc"] = "2.0";
    result["id"] = id; // Echo the request ID
    client->sendTextMessage(QJsonDocument(result).toJson(QJsonDocument::Compact));
}

void Server::sendError(QWebSocket *client, int code, const QString &message, const QJsonValue &id) {
    QJsonObject error;
    error["jsonrpc"] = "2.0";
    error["error"] = QJsonObject{
        {"code", code},
        {"message", message}
    };
    if (!id.isNull()) {
        error["id"] = id; // Include the id in the error response
    }
    client->sendTextMessage(QJsonDocument(error).toJson(QJsonDocument::Compact));
}

void Server::onSocketDisconnected() {
    QWebSocket *socket = qobject_cast<QWebSocket *>(sender());
    if (socket) {
        socket->deleteLater();
    }
}
