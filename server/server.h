#ifndef SERVER_H
#define SERVER_H

#include <QObject>

#include <QMap>
#include <QHash>
#include <QProcess>

class Server : public QObject {
  Q_OBJECT
public:
  Server(QObject *parent = 0L);
  virtual ~Server();

  Q_PID createSForm();

private slots:
  void readyReadStandardError();
  void readyReadStandardOutput();
  void sformDied(int exitCode, QProcess::ExitStatus exitStatus);

private:
  QMap<Q_PID, QProcess*> _sformMap;
  QHash<QProcess*, Q_PID> _sformHash;
};


#endif
