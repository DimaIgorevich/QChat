#ifndef CONTACTPARSER_H
#define CONTACTPARSER_H

#include "abstractparser.h"

class ContactParser: public AbstractParser{
public:
    void parseContactFile(QMap<QString, QString> &dataFriends, QString line);
};

#endif // CONTACTPARSER_H
