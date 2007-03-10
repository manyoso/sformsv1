#include "sform.h"

#include "code.h"

#include <QDir>
#include <QFile>
#include <QTimer>
#include <QDebug>
#include <QProcess>
#include <QCoreApplication>

#include <iostream>

static const int REPRODUCTIONRATE = 50; //Common bacteria reproduce around every twenty minutes
static const int MORBIDITYRATE = 90; //Pre-programmed morbidity rate...??

SForm::SForm(QObject *parent)
    : QObject(parent), _numChildren(0) {

  QTimer *timer = new QTimer(this);
  connect(timer, SIGNAL(timeout()), this, SLOT(reproduce()));
  timer->start(REPRODUCTIONRATE);

  //QTimer::singleShot(MORBIDITYRATE, QCoreApplication::instance(), SLOT(quit()));
}

SForm::~SForm() {}

void SForm::reproduce() {
  //1. Copy all resources out to disk in new directory

  QDir current(QCoreApplication::applicationDirPath());
  Q_ASSERT(current.exists());

  const QString name = QString::number(getpid());
  current.mkdir(name);

  QDir childrensDir(current.absolutePath() + QDir::separator() + name);
  Q_ASSERT(childrensDir.exists());

  const QString childName = QString::number(_numChildren++);
  childrensDir.mkdir(childName);

  qDebug() << name << " reproducing child " << childName << endl;

  QDir childDir(childrensDir.absolutePath() + QDir::separator() + childName);
  Q_ASSERT(childDir.exists());

  QStringList files = Code::files();
  QStringList::ConstIterator it = files.begin();
  for (; it != files.end(); ++it) {
    QFile f((*it));
    Q_ASSERT(f.exists());
    Q_ASSERT(f.copy(childDir.absolutePath() + QDir::separator() + QFileInfo(f).fileName()));
  }

  //2. Compile them into new executable

  QStringList e = QProcess::systemEnvironment();

  QProcess *qmake = new QProcess(this);
  qmake->setEnvironment(e);
  qmake->setWorkingDirectory(childDir.absolutePath());
//   qmake->setProcessChannelMode(QProcess::ForwardedChannels);
  qmake->start("qmake");
  Q_ASSERT(qmake->waitForFinished(-1));
  delete qmake;
  qmake = 0;

  QProcess *make = new QProcess(this);
  make->setEnvironment(e);
  make->setWorkingDirectory(childDir.absolutePath());
//   make->setProcessChannelMode(QProcess::ForwardedChannels);
  make->start("make");
  Q_ASSERT(make->waitForFinished(-1));
  delete make;
  make = 0;

  //3. Ask server to execute them? or execute ourself and fork?
  QProcess *child = new QProcess(this);
  child->setEnvironment(e);
  child->setWorkingDirectory(childDir.absolutePath());
  child->setProcessChannelMode(QProcess::ForwardedChannels);
  child->start("./sform");
}
