#include "c3.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    c3 w;
    w.show();

    return a.exec();
}
