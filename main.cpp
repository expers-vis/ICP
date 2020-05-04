/*
 * Projekt ICP
 * Autori: Adam Ševčík - xsevci64, Martin Hiner - xhiner00
*/

#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
