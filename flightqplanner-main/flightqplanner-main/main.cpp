#include "mainwindow.h"
#include "qdir.h"

#include <QApplication>
#include <QStyleFactory>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QFile file(":/modern-default.css");
    if (file.open(QFile::ReadOnly | QFile::Text)) {
        qDebug() << "read.";
        QTextStream ts(&file);
        QString styleSheet = ts.readAll();
        a.setStyleSheet(styleSheet);
    }

    MainWindow w;
    w.show();
    return a.exec();
}
