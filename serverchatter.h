#ifndef SERVERCHATTER_H
#define SERVERCHATTER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>

class serverChatter : public QObject
{
    Q_OBJECT
public://constructor
    explicit serverChatter(QObject *parent = nullptr);
    //method of rport listening
    void startServer(qint16);
    //method to get address and right socket to managed list of connections
    QString getClientName(QTcpSocket *);

private slots:
    void onNewConnection();
    void onNewDataArrives();
    void onDisconnected();

private:
    QTcpServer *server;
    QHash<QString, QTcpSocket *> connections;
};

#endif // SERVERCHATTER_H
