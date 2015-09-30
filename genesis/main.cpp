#include <QCoreApplication>

#include "genesis.h"

int main(int argc, char **argv)
{
    QCoreApplication app(argc, argv);
    QCoreApplication::setOrganizationDomain("treat.org");
    QCoreApplication::setApplicationName("sform-genesis");

    Genesis genesis(&app);

    int rc = app.exec();
    return rc;
}
