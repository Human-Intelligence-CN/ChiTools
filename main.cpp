#include "widget.h"

#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    w.setWindowIcon(QIcon(":/icons/video.png"));
    w.setWindowTitle("啸攻具");
    w.show();
    return a.exec();
}
