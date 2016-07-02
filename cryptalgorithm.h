#ifndef CRYPTALGORITHM_H
#define CRYPTALGORITHM_H

#include <iostream>
#include <qmath.h>
#include <QObject>
#include <QString>
#include <QDebug>
#include <string.h>
#include <cstdio>
#include <limits.h>
#include <stdio.h>
using namespace std;

class MixBlock{
    string segmentDigest;
public:
    static int count;
    MixBlock(){
        segmentDigest = "";
        count++;
    }
    void setNewDataBlock(string buff){
        segmentDigest = buff;
    }
    void setBlock(string buff){
        segmentDigest.append(buff);
    }
    string getSegmentDigest(){
        return segmentDigest;
    }
    ~MixBlock(){
        count--;
    }
};

class CryptAlgorithm : public QObject
{
    Q_OBJECT
public:
    static QString cryptAlgorithm(QString,bool);
};

#endif // CRYPTALGORITHM_H
