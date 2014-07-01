/*
 * Copyright © 2014 Kier Dugan.  All Rights Reserved.
 *
 * This source code is covered by the MIT licence as set out in the LICENSE
 * file at the root of this repository.
 */

#include "mainwindow.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
