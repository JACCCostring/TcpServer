#include "serverchatter.h"

serverChatter::serverChatter(QObject *parent) : QObject(parent)
{
    //init... server obj instance
    server = new QTcpServer(this);
    //connecting signals when new connections arrives
    connect(server, &QTcpServer::newConnection, this, &serverChatter::onNewConnection);
}

void serverChatter::startServer(qint16 port)
{
    if(server->isListening()){
        qDebug()<<"server already listening ...";
        return;
    }

    server->listen(QHostAddress::Any, port);
    qDebug()<<"server is listening ...";
}

QString serverChatter::getClientName(QTcpSocket *socket)
{
    QString clientName = socket->peerAddress().toString();
    return clientName;
}

void serverChatter::onNewConnection()
{   //getting socket reference from QObject::caller
    QTcpSocket *newClient = server->nextPendingConnection();
    //adding new client to connection list
    connections[getClientName(newClient)] = newClient;
    //connecting signals when new data is available
    connect(newClient, &QTcpSocket::readyRead, this, &serverChatter::onNewDataArrives);
    connect(newClient, &QTcpSocket::disconnected, this, &serverChatter::onDisconnected);
    //warning all clients connected to server of new connectios
    for(QTcpSocket *client: qAsConst(connections))
        client->write(getClientName(newClient).toUtf8()+" : connected to server!");
    //console output client connected
    qDebug()<<"new connection client: "<<getClientName(newClient);
}

void serverChatter::onNewDataArrives()
{   //getting socket reference from QObject::caller
    QTcpSocket *newClient = qobject_cast<QTcpSocket *>(sender());
    //getting client content in a QByteArray to avoid data lost, remember QTcpSocket is async
    QByteArray clientMessage = getClientName(newClient).toUtf8()+" : "+newClient->readAll();
    //looping through all the clients in connection obj
    for(QTcpSocket *client: qAsConst(connections))
        client->write(clientMessage);
}

void serverChatter::onDisconnected()
{
    QTcpSocket *newClient = qobject_cast<QTcpSocket *>(sender());
    //remove client from connection list
    qDebug()<<getClientName(newClient)<<" disconnected from server!\nremoving from connection list ...";
    connections.remove(getClientName(newClient));
    //looping through all the clients in connection obj
    for(QTcpSocket *client: qAsConst(connections))
        client->write(getClientName(newClient).toUtf8()+" disconnected from server!");
}
