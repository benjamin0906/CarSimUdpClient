#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    if(argc == 2)
    {
        QString StrIp(argv[1]);
        QHostAddress *Ip = new QHostAddress(StrIp);
        if(StrIp == Ip->toString())
        {
            QApplication a(argc, argv);
            MainWindow w(nullptr, Ip);
            w.show();
            return a.exec();
        }
        else
        {
            qDebug() << "The argument is not an IP";
            return -2;
        }
    }
    else
    {
        qDebug() << "One and only one argument is needed which is the IP of the server";
        return -1;
    }
}
