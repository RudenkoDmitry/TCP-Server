#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QDate>
#include <QFile>

#include <database.h>

class Server : public QTcpServer
{
    Q_OBJECT
public:
    Server();
    QTcpSocket * socket;
    std::unique_ptr<DataBase> db;

public:
    void startServer();
    void incomingConnection(qintptr socketDescriptor);
    void socketConnect();
    void socketDisconnect();
    void startDataBase();
};

#endif // SERVER_H
