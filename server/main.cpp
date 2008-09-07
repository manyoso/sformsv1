#include <QCoreApplication>

#include "server.h"

int main(int argc, char **argv) {
    QCoreApplication *a = new QCoreApplication(argc, argv);
    QCoreApplication::setOrganizationDomain("treat.org");
    QCoreApplication::setApplicationName("server");

    Server server;
    server.createSForm();

    int rc = a->exec();

    return rc;
}
