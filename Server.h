#ifndef SERVER_H
#define SERVER_H

#include <QWebSocketServer>
#include <QWebSocket>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include "ObjectManager.h"

class Server : public QObject {
    Q_OBJECT
public:
    explicit Server(quint16 port, QObject *parent = nullptr);

private slots:
    void onNewConnection();
    void onMessageReceived(const QString &message);
    void sendError(QWebSocket *client, int code, const QString &message, const QJsonValue &id);
    void onSocketDisconnected();

private:
    QWebSocketServer *m_pWebSocketServer;
    ObjectManager *objectManager;
};

#endif // SERVER_H
