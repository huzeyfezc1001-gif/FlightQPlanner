#include "mainwindow.h"
#include <QApplication>
#include <QStyleFactory>
#include <QFile>
#include <QTextStream>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    a.setStyle(QStyleFactory::create("Fusion"));

    QFile file(":/modern-default.css");
    if (file.open(QFile::ReadOnly | QFile::Text)) {
        qDebug() << "Stil dosyasi okundu.";
        QTextStream ts(&file);
        QString styleSheet = ts.readAll();
        a.setStyleSheet(styleSheet);
    }

    MainWindow w;
    w.show();
    return a.exec();
}