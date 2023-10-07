#include "server.h"

#include <QJsonArray>

Server::Server()
{

}

void Server::startDataBase()
{
    db = std::make_unique<DataBase>();
    db->openDataBase();
}

void Server::startServer(){
    if(this->listen(QHostAddress::Any,8080)){
        qDebug()<<"Listening...";
    } else {
        qDebug()<<"Not listening";
    }
    startDataBase();
}

void Server::incomingConnection(qintptr socketDescriptor){
    socket = new QTcpSocket(this);
    socket->setSocketDescriptor(socketDescriptor);

    connect(socket,&QTcpSocket::readyRead,this,&Server::socketConnect);
    connect(socket,&QTcpSocket::disconnected,this,&Server::socketDisconnect);

    qDebug()<<socketDescriptor<<" Client connected";
}

void Server::socketConnect()
{
    QByteArray data = socket->readAll();
    QJsonParseError docError;
    QJsonDocument doc = QJsonDocument::fromJson(data, &docError);

    if(docError.errorString().toInt()!=QJsonParseError::NoError)
        return;

    switch (doc.object().value("type").toInt())
    {
        case SELECT_TASK:
        {
            QJsonObject result;
            result.insert("type", SELECT_TASK);
            QJsonArray data = db->getTaskTableData();

            result.insert("result", data);

            socket->write(QJsonDocument(result).toJson());
            socket->waitForBytesWritten(1000);
            break;
        }

        case SELECT_OVERDUE_TASK:
        {
            QJsonObject result;
            result.insert("type", SELECT_OVERDUE_TASK);
            QJsonArray data = db->getTaskTableData(QDate::currentDate());

            result.insert("result", data);

            socket->write(QJsonDocument(result).toJson());
            socket->waitForBytesWritten(1000);
            break;
        }

        case INSERT_TASK:
        {
            QJsonObject result;
            result.insert("type", INSERT_TASK);
            result.insert("result", db->insertFromJson(doc.object().value("params").toObject()));
            socket->write(QJsonDocument(result).toJson());
        }
            break;

        case REMOVE_TASK:
        {
            QJsonObject result;
            result.insert("type", REMOVE_TASK);
            result.insert("result",db->deleteFromJson(doc.object().value("params").toObject()));
            socket->write(QJsonDocument(result).toJson());
            break;
        }
        default:
        {
            qDebug()<<"Неправильный формат данных";
        }
    }
}

void Server::socketDisconnect()
{
    qDebug()<<"Diconected";
    socket->deleteLater();
}
