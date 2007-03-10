#include <QCoreApplication>

#include "filehandler.h"

#include <QDebug>
#include <QStringList>

int main(int argc, char **argv) {
  QCoreApplication *a = new QCoreApplication(argc, argv);
  QCoreApplication::setOrganizationDomain("treat.org");
  QCoreApplication::setApplicationName("cpp2asg");

  FileHandler fileHandler;
  QStringList args = QCoreApplication::arguments();
  for (int i = 1; i < argc; ++i) {
    qDebug() << "handling " << args[i] << endl;
    fileHandler.handleFile(args[i]);
  }

  return 0;
}
