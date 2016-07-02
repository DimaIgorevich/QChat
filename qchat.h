#ifndef QCHAT_H
#define QCHAT_H

//constant path to directory
#define PATH ".//system//history//"
#define YES true
#define NO false


//custom Class
#include "cryptalgorithm.h"
#include "contactparser.h"

#include <iostream>

//UI Library
#include <QMainWindow>
#include <QBrush>
#include <QPalette>
#include <QColor>
#include <QFrame>
#include <QLabel>
#include <QVBoxLayout>
#include <QMessageBox>

//media library
#include <QtMultimedia/QSound>


//data source
#include <QFile>
#include <QMap>
#include <QDebug>
#include <QDataStream>
#include <QByteArray>

//server library
#include <QTcpServer>
#include <QTcpSocket>
#include <QTime>


//myHFile
#include "ex_qlabel.h"

namespace Ui {
class QChat;
}

class QChat : public QMainWindow
{
    Q_OBJECT

public:
    explicit QChat(QWidget *parent = 0);
    ~QChat();

private:
    //DataSource
    QMap<QString,QString> dataFriends;
    bool *isPress;
    int start;
    QFile *fileFriends;
    QFile *fileMessage;
    int countFriends;
    int currentFriendIndex;
    QString currentIp;
    QString dataHistory;
    QString *listIp;

    //Parser
    ContactParser *parser;

    //Dynamic UI
    QFrame *friends;
    Ex_QLabel *labelFriends;
    QVBoxLayout *labelLayout;

    Ui::QChat *ui;

    //method class:

    //UI
    void setSettingWindow();
    void insertElementsToScroll();
    void changeCurrentLabel(int);

    //DataSource methods
    void createListIp();

    //Part methods which work with file
    void readFile();
    void loadMessageHistory(int);
    void saveMessageHistory();
    void saveHistory(QString);

    //TCP/IP
    QSound *sound;
    bool statusConnection;

public:

    QTcpServer *serverC;//client part
    QTcpSocket *server;
    QTcpSocket *socket;//coonect for server
    quint16 rb;//read data

    //void sendToClient(QTcpSocket *socket, const QString str);

public slots:
    //UI events
    void mousePressed(int);

    //Server part
    void slotNewConnection();
    void slotReadClient();

    //Client part
    void errorProc(QAbstractSocket::SocketError er);
    void sendData();
    void connectServer();

private slots:
    //UI Event
    void on_pushButton_clicked();
};

#endif // QCHAT_H
