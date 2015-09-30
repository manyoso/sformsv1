#include <QCoreApplication>

#include "reaper.h"

int main(int argc, char **argv)
{
    QCoreApplication app(argc, argv);
    QCoreApplication::setOrganizationDomain("treat.org");
    QCoreApplication::setApplicationName("sform-reaper");

    Reaper reaper(&app);

    int rc = app.exec();
    return rc;
}
