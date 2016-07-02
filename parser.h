#ifndef PARSER_H
#define PARSER_H
#include <QString>
#include <QMap>

class Parser
{
public:
     void parseContactFile(QMap<QString, QString> &, QString);
};

#endif // PARSER_H
