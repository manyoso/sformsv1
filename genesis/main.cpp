#include <QCoreApplication>

#include "genesis.h"

int main(int argc, char **argv)
{
    QCoreApplication *a = new QCoreApplication(argc, argv);
    QCoreApplication::setOrganizationDomain("treat.org");
    QCoreApplication::setApplicationName("sform-genesis");

    Genesis genesis;

    int rc = a->exec();

    return rc;
}
