#include "contactparser.h"

void ContactParser::parseContactFile(QMap<QString, QString> &dataFriends, QString line){
    bool fName = false;
    QString ipAdress="";
    QString name = "";
    for(int i=0;i<line.length();i++)
    {
        if (!fName)
            if(line.at(i)!=';')
                ipAdress += line.at(i);
            else {
                fName = !fName;continue;
            }
        if(fName)
            name += line.at(i);
    }
    dataFriends[ipAdress] = name;
}

