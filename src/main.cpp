#include "mainwidget.h"

#include <QApplication>

/// entry point into programm
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWidget w;
    // set main window size
    w.resize(1300, 700);
    w.show();
    return a.exec();
}
