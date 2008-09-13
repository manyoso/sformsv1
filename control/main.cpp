#include <QApplication>

#include "control.h"

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    QCoreApplication::setOrganizationDomain("treat.org");
    QCoreApplication::setApplicationName("sform-control");

    Control control;
    control.show();

    int rc = app.exec();
    return rc;
}
