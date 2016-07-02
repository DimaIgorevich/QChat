#include "clientserverpart.h"

ClientServerPart::ClientServerPart(QChat *ui){
    rb = 0;

    statusConnection = false;
    sound = new QSound(".//message.wav");
    this->ui = ui;
    //
    serverC = new QTcpServer(this->ui);
    if(!serverC->listen(QHostAddress::Any,9999)){
         QMessageBox::information(0, "Error", "Not running");
         return;
    }
    connect(serverC,SIGNAL(newConnection()),this, SLOT(slotNewConnection()));
}

void ClientServerPart::slotNewConnection()
{
    QTcpSocket *socket = serverC->nextPendingConnection();
    connect(socket,SIGNAL(readyRead()),this,SLOT(slotReadClient()));
}

void ClientServerPart::slotReadClient()
{
    sound->play();
    socket = (QTcpSocket*)sender();
    QString str;
        QDataStream in(socket);
        in.setVersion(QDataStream::Qt_5_5);
        for (;;)
        {
            if(!rb)
            {
                if(socket->bytesAvailable()<sizeof(quint16))
                    break;
                in>>rb;
            }
            if(socket->bytesAvailable()<rb)
                break;
            QDateTime time;
            in>>time>>str;

            //str = encryptionText(str);

            qDebug()<<str;

            str = CryptAlgorithm::cryptAlgorithm(str);

            qDebug()<<str;

            QString data = time.toString()+" : "+str;

            //ui->textEdit->append(time.toString()+" : "+str);
            //saveHistory(data);

            rb=0;
        }
}

void ClientServerPart::errorProc(QAbstractSocket::SocketError er)
{
    QString strError = "Error: ";
    if(er == QAbstractSocket::HostNotFoundError)//net connect
    {
        strError+="The host was not found.";
    } else if(er == QAbstractSocket::RemoteHostClosedError)//обрыв соединения
    {
        strError+="The remote host is closed.";
    } else if(er == QAbstractSocket::ConnectionRefusedError)//У - уходи))ПРотивный
    {
        strError += "The connection was refused.";
    } else
    {
        strError +=QString(server->errorString());//Иная ошибка
    }

    //ui->textEdit->append(strError);

    statusConnection = false;
}

void ClientServerPart::sendData()
{
    QByteArray data;
    QDataStream out(&data,QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_5);

}

void ClientServerPart::connectServer()
{
    statusConnection = true;
}

/*
ClientServerPart::ClientServerPart(QObject *parent) : QObject(parent)
{

}*/



