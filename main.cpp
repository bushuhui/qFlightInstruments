#include <QtCore>
#include <QtGui>
#include <QWidget>
#include <QApplication>

#include "qFlightInstruments.h"
#include "TestWin.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    TestWin testWin;

    testWin.show();

    return a.exec();
}
