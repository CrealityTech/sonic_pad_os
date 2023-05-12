#include "Widget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    qputenv("QT_IM_MODULE",QByteArray("Qt5Input"));
    QApplication a(argc, argv);
    Widget w;
    w.show();

    return a.exec();
}
