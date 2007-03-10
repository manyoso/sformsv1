#include <QCoreApplication>

#include "sform.h"

int main(int argc, char **argv) {
  QCoreApplication *a = new QCoreApplication(argc, argv);
  QCoreApplication::setOrganizationDomain("treat.org");
  QCoreApplication::setApplicationName("sform");

  SForm sform;

  int rc = a->exec();

  return rc;
}
