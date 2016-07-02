#ifndef CLIENTSERVERPART_H
#define CLIENTSERVERPART_H

//main class
#include "qchat.h"

//QtClass
#include <QObject>

//server library
#include <QTcpServer>
#include <QTcpSocket>
#include <QTime>

//media library
#include <QtMultimedia/QSound>

class ClientServerPart : public QObject
{
    Q_OBJECT

    QChat *ui;


    quint16 rb;//read data

    //TCP/IP
    QSound *sound;
    bool statusConnection;

public:

    //explicit ClientServerPart(QObject *parent = 0);

    ClientServerPart(QChat *ui);

    QTcpServer *serverC;//client part
    QTcpSocket *server;
    QTcpSocket *socket;//coonect for server

    //
    //void sendToClient(QTcpSocket *socket, const QString str);

signals:

public slots:
    //Server part
    void slotNewConnection();
    void slotReadClient();

    //Client part
    void errorProc(QAbstractSocket::SocketError er);
    void sendData();
    void connectServer();
};

#endif // CLIENTSERVERPART_H
