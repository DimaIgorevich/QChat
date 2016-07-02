#include "cryptalgorithm.h"
#include <string>

int MixBlock::count = 0;

void roundKey(char *msg,char keySymb, int n){
    cout<<"Input msg: "<<msg<<'\n';
    for(int i=n;i<strlen(msg);i++)
        if(i==n){
            cout<<"current symb:"<<(int)keySymb<<", i - "<<i<<'\n';
            if(msg[i] ^ keySymb)
                msg[i] ^= keySymb;
        }
        else {
            if(msg[i] ^ msg[i-1])
                msg[i] ^= msg[i-1];
        }
    cout<<"Res msg: "<<msg<<"\n\n";
    cout<<"res mes in int: ";
    for(int i=0; i<strlen(msg); i++){
        cout<<(int)msg[i]<<"\n";
    }
}

void roundKeyDescrypt(char *msg,char keySymb, int n){
    cout<<"Res msg: "<<msg<<'\n';
    for(int i = (int)strlen(msg) - 1; i>=n;i--){
        if(i != n){
            if(msg[i] ^ msg[i-1])
                msg[i] ^= msg[i-1];
        }
        else {
            cout<<"i: "<<i<<'\n';
            if(msg[i] ^ keySymb)
                msg[i] ^= keySymb;
            cout<<"Res msg: "<<msg<<'\n'<<"///////////////////////////\n\n";
            break;
        }
    }

}

char *toChar(string str){
    char *res = new char[str.length()+1];
    for(int i=0;i<str.length();i++)
        res[i] = str[i];
    res[str.length()] = '\0';
    return res;
}

string mid(string str, int pos, int n){
    string temp = "";
    for(int i=pos;i<pos+n;i++)
        temp+=str.at(i);
    return temp;
}

bool hasSuffix(char suffix, string str){
    if(str.at(str.size()-1) == suffix)
        return true;
    return false;
}

QString CryptAlgorithm::cryptAlgorithm(QString str, bool status){
    //variable
    char *msg;
    string line = "";
    MixBlock *blocks;
    int index;

    //set Key Words
    string keyWord = "dd";

    for(int i=0;i<keyWord.size();i++)
        keyWord[i] = (char)((int)keyWord[i]+100);

    for(int i=0;i<keyWord.size();i++)
        cout<<keyWord[i]<<'\n';

    line = str.toLocal8Bit().data();


    if(status){
    //setup for encryption
    if(line.size()<keyWord.size())
        for(int i=0;i<keyWord.size()-line.size();i++)
            line.append("$");
    if(line.size()%4 != 0)
        for(int i=0;i<line.size()%4;i++)
            line.append("$");

    //module enctiption

    blocks = new MixBlock[4];

    for(int i=0;i<line.size()/4;i++){
        string blockStr = mid(line, i*4,4);
        for(int j=0; j<blocks->count;j++){
            string buffer = "";
            buffer += blockStr.at(j);
            blocks[j].setBlock(buffer);
        }
    }

    line = "";

    for(int i=0;i<blocks->count;i++)
        line += blocks[i].getSegmentDigest();

    delete []blocks;
    blocks = new MixBlock[line.size()/4];

    for(int i=0;i<blocks->count;i++)
        blocks[i].setBlock(mid(line,i*4,4));

    index = 0;

    for(int i=0;i<blocks->count;i++){
        char *buffer = toChar(blocks[i].getSegmentDigest());
        for(int j=0;j<strlen(buffer);j++){
            roundKey(buffer,keyWord[index%keyWord.size()],j);
            index++;
        }
        string buff = "";
        buff += buffer;
        blocks[i].setNewDataBlock(buff);
    }
    line = "";
    for(int i=0;i<blocks->count;i++)
        line += blocks[i].getSegmentDigest();

    delete []blocks;

    cout<<"Encrypt line: "<<line<<'\n';

    str = QString::fromLocal8Bit(line.c_str());

    //str = QString::fromStdString(line.c_str());

    qDebug()<<"Str ecntrypt: "<<str<<'\n';
    qDebug()<<"Send message";
    } else {
        qDebug()<<"GEt message";

        qDebug()<<line.size();
        qDebug()<<"size: "<<line.size()/4;
        blocks = new MixBlock[line.size()/4];

        cout<<"Descrypt line: "<<line<<'\n';

        for(int i=0;i<blocks->count;i++)
            blocks[i].setBlock(mid(line,i*4,4));

        line = "";

        for(int i=0;i<blocks->count;i++)
            line +=blocks[i].getSegmentDigest();

        index = (int)line.size()-1;

        for(int j=blocks->count-1;j>=0;j--){
            msg = toChar(blocks[j].getSegmentDigest());
            for(int i=(int)strlen(msg)-1;i>=0;i--){
                roundKeyDescrypt(msg,keyWord[index%keyWord.size()],i);
                index--;
            }
            string buff = "";
            buff += msg;
            blocks[j].setNewDataBlock(buff);
        }

        line = "";

        for(int i=0;i<(int)blocks->count;i++)
            line += blocks[i].getSegmentDigest();

        delete []blocks;

        blocks = new MixBlock[4];

        for(int i=0;i<blocks->count;i++){
            string blockStr = mid(line,i*(int)line.size()/4,(int)line.size()/4);
            blocks[i].setNewDataBlock(blockStr);
        }
        string resultLineD = "";

        for(int i=0;i<(int)line.size()/4;i++)
            for(int j=0;j<blocks->count;j++)
                resultLineD += blocks[j].getSegmentDigest().at(i);

        while(hasSuffix('$',resultLineD))
            resultLineD = mid(resultLineD,0,(int)(resultLineD.size()-1));

        cout<<"Result descrypt line: "<<resultLineD<<'\n';

        str = QString::fromLocal8Bit(resultLineD.c_str());

        //str = QString::fromStdString(resultLineD.c_str());
        delete[]blocks;
        qDebug()<<"String line: "<<str<<'\n';
    }

    return str;
}


