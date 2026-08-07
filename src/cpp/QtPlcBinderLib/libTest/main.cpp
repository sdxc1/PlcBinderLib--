#include "libTest.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    libTest window;
    window.show();
    return app.exec();
}
