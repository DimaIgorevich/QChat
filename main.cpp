#include "qchat.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QChat w;
    w.show();

    return a.exec();
}
