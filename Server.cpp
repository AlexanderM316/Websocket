#include "Server.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

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
    QJsonDocument doc = QJsonDocument::fromJson(message.toUtf8());
    QJsonObject json = doc.object();

    QString method = json["method"].toString();
    QJsonObject params = json["params"].toObject();
    QJsonObject result;

    if (method == "createObject") {
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
    }
    // Здесь добавьте обработку других методов (updateObject, deleteObject и т.д.)

    // Отправка ответа
    QWebSocket *client = qobject_cast<QWebSocket*>(sender());
    if (client) {
        client->sendTextMessage(QJsonDocument(result).toJson(QJsonDocument::Compact));
    }
}

void Server::onSocketDisconnected() {
    QWebSocket *socket = qobject_cast<QWebSocket *>(sender());
    if (socket) {
        socket->deleteLater();
    }
}
