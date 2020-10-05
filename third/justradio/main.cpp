#include "dialog.h"
#include <QApplication>
#include <GL/glut.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    glutInit(&argc, argv);
    Dialog w;
    w.show();
    w.run();
    return a.exec();
}
