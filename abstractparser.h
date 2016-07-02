#ifndef ABSTRACTPARSER_H
#define ABSTRACTPARSER_H

#include <QString>
#include <QMap>

class AbstractParser{
public:
    virtual void parseContactFile(QMap<QString,QString> &dataFriends, QString line)=0;
};

#endif // ABSTRACTPARSER_H
