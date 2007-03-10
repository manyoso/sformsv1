#include "server.h"

#include <QTime>
#include <QDebug>

static const QString SFORMDIR = "/home/adam/project/sform/";
static const QString SFORMBIN = SFORMDIR + "sform";
static const int MAXIMUM = 100;

Server::Server(QObject *parent)
    : QObject(parent) {

  srandom(QTime::currentTime().msec());
 }


Server::~Server() {}

Q_PID Server::createSForm() {

  qDebug() << _sformMap.count() << endl;

  if (_sformMap.count() >= MAXIMUM || _sformHash.count() >= MAXIMUM) {
    //kill off a number of the sforms...
    uint r = random() % MAXIMUM - 1;
/*    qDebug() << "Kill off some random number of the oldest lived organisms = " << r << endl;*/
    for (uint i = 0; i < r; ++i) {
      if (QProcess *marked = _sformMap.begin().value()) {
        Q_PID pid = _sformMap.begin().key();

        marked->disconnect();
        marked->kill();
        marked->waitForFinished();

//         qDebug() << "sform " << pid << " killed" << endl;

        _sformMap.remove(pid);
        _sformHash.remove(marked);
        delete marked;
        marked = 0;
      }
    }
  }

  QProcess *sform = new QProcess(this);

  sform->setWorkingDirectory(SFORMDIR);

  connect(sform, SIGNAL(readyReadStandardOutput()),
          this, SLOT(readyReadStandardOutput()));
  connect(sform, SIGNAL(readyReadStandardError()),
          this, SLOT(readyReadStandardError()));
  connect(sform, SIGNAL(finished(int, QProcess::ExitStatus)),
          this, SLOT(sformDied(int, QProcess::ExitStatus)));

  sform->start(SFORMBIN);
  if (sform->waitForStarted()) {
/*    qDebug() << "started sform pid=" << sform->pid() << endl;*/
    _sformMap.insert(sform->pid(), sform);
    _sformHash.insert(sform, sform->pid());
  } else {
/*    qDebug() << "sform error " << sform->errorString() << endl;*/
  }

  return 0;
}

void Server::readyReadStandardError() {
  QProcess *process = qobject_cast<QProcess*>(sender());
  QByteArray out = process->readAllStandardError();
  qDebug() << out << endl;
}

void Server::readyReadStandardOutput() {
  QProcess *process = qobject_cast<QProcess*>(sender());
  QByteArray out = process->readAllStandardOutput();
//   qDebug() << out << endl;

  //Assume that the output was a call to reproduce for now...  if so, we create two new sforms
  createSForm();
  createSForm();
}

void Server::sformDied(int exitCode, QProcess::ExitStatus exitStatus) {
  QProcess *process = qobject_cast<QProcess*>(sender());
//   qDebug() << "sform " << process->pid() << " died with exitCode=" << exitCode << " and exitStatus=" << exitStatus << endl;

  Q_PID pid = _sformHash.value(process);
  _sformMap.remove(pid);
  _sformHash.remove(process);
  delete process;
  process = 0;
}
