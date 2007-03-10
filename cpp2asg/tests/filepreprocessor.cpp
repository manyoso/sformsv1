#include "filepreprocessor.h"

#include <QDir>
#include <QFile>
#include <QDebug>

#include "pp-stream.h"

FilePreprocessor::FilePreprocessor()
    : rpp::Preprocessor() {

  _includeDirs/* << "/home/adam/project/cpp2asg/app"
               << "/home/adam/project/cpp2asg"
               << "/home/kde/install/qt/mkspecs/linux-g++"
               << "/home/kde/install/qt/include/QtCore"
               << "/home/kde/install/qt/include/QtGui"
               << "/home/kde/install/qt/include"
               << "/home/adam/project/cpp2asg/parser"
               << "/home/adam/project/cpp2asg/parser/rpp"
               << "/home/adam/project/cpp2asg/duchain"
               << "/home/adam/project/cpp2asg/duchain/editor"
               << "/home/adam/project/cpp2asg/duchain/util"
               << "/usr/include"
               << "/usr/local/include"
               << "/home/adam/dev/sforms/cpp2asg/tests/files"*/;
}

FilePreprocessor::~FilePreprocessor() {}

rpp::Stream* FilePreprocessor::sourceNeeded(QString& fileName, IncludeType type, int sourceLine) {

  foreach (QString dirs, _includeDirs) {
    QString potential = dirs + QDir::separator() + fileName;
    QFile file(potential);
    if (file.exists()) {
      if (file.open(QIODevice::ReadOnly)) {
/*        qDebug() << "Found include" << fileName << " = " << potential << " at " << sourceLine << endl;*/
        fileName = potential; //so we mark the full path in the output
        QByteArray contents = file.readAll();
        rpp::Stream *s = new rpp::Stream(new QString(QString::fromUtf8(contents)));
        s->setInputLineNumber( sourceLine );
        return s;
      }
    }
  }

  return 0;
}
