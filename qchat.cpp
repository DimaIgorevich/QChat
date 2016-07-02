#include "qchat.h"
#include "ui_qchat.h"

int Ex_QLabel::count = -1;//count of elements for friends

QChat::QChat(QWidget *parent) ://Constructor
    QMainWindow(parent),rb(0),
    ui(new Ui::QChat){

    ui->setupUi(this);
    setSettingWindow();
    readFile();
    insertElementsToScroll();
    //
    serverC = new QTcpServer(this);
    if(!serverC->listen(QHostAddress::Any,9999)){
         QMessageBox::information(0, "Error", "Not running");
         return;
    }
    connect(serverC,SIGNAL(newConnection()),this, SLOT(slotNewConnection()));

    connect(ui->lineEdit,SIGNAL(returnPressed()),this,SLOT(on_pushButton_clicked()));
}

QChat::~QChat(){//Destructor
    if(start)
        if(!fileMessage->open(QIODevice::WriteOnly))
            qDebug()<<"Erorr open mes";
        else saveMessageHistory();
    delete ui;
}

void QChat::setSettingWindow(){//set setting for Chat Windows (UI)
    start = 0;

    statusConnection = false;
    sound = new QSound("message.wav");

    currentFriendIndex = -1;
    dataHistory = "";
    ui->textEdit->clear();
    ui->textEdit->setReadOnly(true);
    ui->textEdit->setStyleSheet("QTextEdit { font-size:12px; color : white; }");
    ui->frameContacts->setAutoFillBackground(true);
    QPalette pal = ui->frameContacts->palette();
    pal.setColor(QPalette::Background, QColor("#F08080"));
    ui->frameContacts->setPalette(pal);
    pal = this->palette();
    pal.setColor(QPalette::Background, QColor("#668B8B"));
    this->setPalette(pal);
    pal = ui->scrollArea->palette();
    pal.setColor(QPalette::Background, QColor("#63b8ff"));
    ui->scrollArea->setPalette(pal);
}
//Data Source
void QChat::readFile(){//method for read file with contact(friend list)
    parser = new ContactParser;
    fileFriends = new QFile(".//contact.txt");
    if(fileFriends->open(QIODevice::ReadOnly|QIODevice::Text))
        while(!fileFriends->atEnd())
            parser->parseContactFile(dataFriends, fileFriends->readLine());
    else qDebug()<<"Error open file";
    fileFriends->close();
    countFriends = dataFriends.count();
    createListIp();
    //delete parser;
}
//UI
void QChat::insertElementsToScroll(){
    int index = 0;
    friends = new QFrame[countFriends];
    isPress = new bool[countFriends];
    labelFriends = new Ex_QLabel[countFriends];
    labelLayout = new QVBoxLayout();
    labelLayout->setMargin(2);
    QMap<QString,QString>::iterator it = dataFriends.begin();
    while(it!=dataFriends.end())
    {
        labelFriends[index].setText(it.value()+it.key());
        labelFriends[index].setStyleSheet("QLabel { font-size:12px; background-color : #F08080; color : white; }");
        labelFriends[index].setFixedHeight(40);
        labelFriends[index].setMargin(5);
        connect(&labelFriends[index],SIGNAL(mousePresseed(int)),this,SLOT(mousePressed(int)));
        labelLayout->addWidget(labelFriends[index].topLevelWidget());
        isPress[index] = false;
        it++,index++;
    }
    labelLayout->setAlignment(Qt::AlignTop);
    ui->scrollArea->setWidgetResizable(true);
    ui->scrollArea->setLayout(labelLayout);
}
//Events UI
void QChat::changeCurrentLabel(int index){
    for(int i=0;i<countFriends;i++)
        if(!isPress[i]) labelFriends[i].setStyleSheet("QLabel { font-size:12px; background-color : #F08080; color : white; }");
    if(index!=currentFriendIndex)
    {
        if(start)
            if(!fileMessage->open(QIODevice::WriteOnly))
                qDebug()<<"Erorr open mes";
            else saveMessageHistory();
        start++;
        currentFriendIndex = index;
        ui->textEdit->clear();
        loadMessageHistory(index);
        currentIp = listIp[index];
    }

}
//UI
void QChat::loadMessageHistory(int index){
    int pos = 0;
    QString fileName="";
    QMap<QString,QString>::iterator it = dataFriends.begin();
    while(it != dataFriends.end())
    {
        fileName = it.key()+".txt";
        if(pos==index) break;
        pos++;
        it++;
    }
    if(!QFile::exists(PATH+fileName))
    {
        fileMessage = new QFile(PATH+fileName);
        if(!fileMessage->open(QIODevice::WriteOnly))
            qDebug()<<"Error open mes";
    } else {
        fileMessage = new QFile(PATH+fileName);
        if(!fileMessage->open(QIODevice::ReadOnly))
            qDebug()<<"Erorr open mes";
        else {
            dataHistory = fileMessage->readAll();
            if(!dataHistory.isEmpty())
                ui->textEdit->append(dataHistory);
            fileMessage->close();
        }
    }
}
//UI
void QChat::saveMessageHistory(){
    QTextStream out(fileMessage);
    out.setCodec("UTF-8");
    if(!dataHistory.isEmpty())
        out<<dataHistory.toUtf8();
    fileMessage->close();
}
//UI
void QChat::saveHistory(QString data){
    dataHistory += data;
    dataHistory += '\n';
}
//UI
void QChat::createListIp(){
    int index = 0;
    listIp = new QString[countFriends];
    QMap<QString,QString>::iterator it = dataFriends.begin();
    while(it != dataFriends.end())
    {
        listIp[index] = it.key();
        index++;
        it++;
    }
}
//UI
void QChat::mousePressed(int index){
    isPress[index] = true;
    for(int i=0;i<countFriends;i++)
        if(i!=index) isPress[i] = false;
    changeCurrentLabel(index);
}

//Server - ready
void QChat::slotNewConnection(){
    QTcpSocket *socket = serverC->nextPendingConnection();
    connect(socket,SIGNAL(readyRead()),this,SLOT(slotReadClient()));
}

//Server
void QChat::slotReadClient(){
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

            qDebug()<<"size: "<<str.size();
            str = CryptAlgorithm::cryptAlgorithm(str,false);

            QString data = time.toString()+" : "+str;

            ui->textEdit->append(time.toString()+" : "+str);
            saveHistory(data);

            rb=0;
        }
}
//ServerError
void QChat::errorProc(QAbstractSocket::SocketError er){
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
    ui->textEdit->append(strError);
    statusConnection = false;
}
//Server
void QChat::sendData(){
    QByteArray data;
    QDataStream out(&data,QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_5);

    qDebug()<<ui->lineEdit->text();

    QString encrText = CryptAlgorithm::cryptAlgorithm(ui->lineEdit->text(),true);

    qDebug()<<"size: "<<encrText.size();

    out<<quint16(0)<<QDateTime::currentDateTime()<<encrText;

    //out<<quint16(0)<<QDateTime::currentDateTime()<<ui->lineEdit->text();

    out.device()->seek(0);
    out<<(quint16)(data.size() - sizeof(quint16));

    //encyption
    //server->write(encryptionText(data));

    server->write(data);

    saveHistory(data);

    ui->lineEdit->setText("");
}
//Server
void QChat::connectServer(){
     statusConnection = true;
}
//UI events
void QChat::on_pushButton_clicked(){
    if(!ui->lineEdit->text().isEmpty())
    {
        qDebug()<<currentIp;
        server = new QTcpSocket(this);
        server->connectToHost(currentIp,9999);
        //server->connectToHost("169.254.130.42",9999);
        if(!statusConnection)
        {
            connect(server,SIGNAL(connected()),this,SLOT(connectServer()));
            connect(server,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(errorProc(QAbstractSocket::SocketError)));
        }
        if(statusConnection)
        {
            QString message = "";
            message += QDateTime::currentDateTime().toString()+" Я: "+ui->lineEdit->text();

            dataHistory +=message;

            ui->textEdit->append(message);
            sendData();
        }
    } else {
        QMessageBox::information(0,"Warning","Warning!\nLine edit is empty!");
    }
}
