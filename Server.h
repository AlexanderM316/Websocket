#ifndef SERVER_H
#define SERVER_H

#include <QWebSocketServer>
#include <QWebSocket>
#include <QJsonDocument>
#include <QJsonObject>
#include "ObjectManager.h"

class Server : public QObject {
    Q_OBJECT
public:
    Server(quint16 port, QObject *parent = nullptr);

private slots:
    void onNewConnection();
    void onMessageReceived(const QString &message);
    void onSocketDisconnected();

private:
    QWebSocketServer *m_pWebSocketServer;
    QHash<QString, QWebSocket*> m_clients;
    ObjectManager *objectManager;
};

#endif // SERVER_H
