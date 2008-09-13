#ifndef REAPER_H
#define REAPER_H

#include <QObject>

class QTime;
class QTimer;
class QSocketNotifier;

class Reaper : public QObject {
    Q_OBJECT
public:
    Reaper(QObject *parent = 0L);
    virtual ~Reaper();

private Q_SLOTS:
    void reap();
    void readStdin(int);

private:
    void logKill(const QStringList &killed);

private:
    QTimer *_reaper;
    QSocketNotifier *_stdinNotifier;
};

#endif
