#include "calculator.h"
#include <QApplication>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QList>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();

}
